open Globals;
open Accounting;

let calcPricePerSecond = (price, numerator, denominator) => {
  let priceBn = BN.new_(price);
  let numeratorBn = BN.new_(numerator);
  let denominatorBn = BN.new_(denominator);
  let fullYearSeconds = BN.new_("31536000");

  priceBn
  ->BN.mul(numeratorBn)
  ->BN.div(denominatorBn)
  ->BN.div(fullYearSeconds);
};

// TODO: Could cached and stored so that all values don't need to be culculated each time!
// this calculates pricePerSecondEach time.
let calculateDepositDuration = (deposit, price, numerator, denominator) => {
  let depositBn = BN.new_(deposit);
  let pricePerSecond = calcPricePerSecond(price, numerator, denominator);

  depositBn
  ->BN.div(
      if (pricePerSecond->BN.gt(BN.new_("0"))) {
        pricePerSecond;
      } else {
        BN.new_("1");
      },
    )
  ->BN.toString
  ->Int.fromString
  ->defaultZeroI;
  // Check, 9007199254740992 is the largest integer available to javascript.
};

let calcRequiredDepositForTime = (time, price, numerator, denominator) => {
  let timeBn = BN.new_(string_of_int(time));
  let pricePerSecond = calcPricePerSecond(price, numerator, denominator);

  let requiredDeposit = timeBn->BN.mul(pricePerSecond)->BN.toString;

  requiredDeposit->Web3Utils.fromWeiToEth;
};

module BuyMainnet = {
  [@react.component]
  let make = (~tokenId: TokenId.t) => {
    let chain = Client.MainnetQuery;
    let (buyFunc, txBuyState) = ContractActions.useBuy(tokenId, false);
    let (buyFuncAuction, txBuyAuctionState) =
      ContractActions.useBuyAuction(tokenId, false);
    let userBalance =
      Belt.Option.mapWithDefault(
        RootProvider.useEthBalance(), BN.new_("0"), a =>
        a
      );

    let (numerator, denominator, ratio, _ratioInverse) =
      QlHooks.usePledgeRateDetailed(~chain, tokenId);
    let priceStatus: QlHooks.animalPrice = QlHooks.usePrice(~chain, tokenId);
    let isOnAuction = Animal.useIsOnAuction(~chain, tokenId);
    let launchTimeOpt = QlHooks.useLaunchTimeBN(~chain, tokenId);
    let currentPriceWei =
      Animal.useAuctionPriceWei(
        ~chain,
        tokenId,
        launchTimeOpt->Option.getWithDefault(BN.new_("5000")),
      );

    let currentPriceWei =
      isOnAuction
        ? currentPriceWei
        : (
          switch (priceStatus) {
          | Price(price) => price
          | Loading
          | Foreclosed(_) => BN.new_("0")
          }
        );

    let tokenIdName = "token#" ++ tokenId->TokenId.toString;

    let maxAvailableDepositBN =
      userBalance
      ->BN.sub(BN.new_("3000000000000000")) // 0.003 eth as gas
      ->BN.sub(currentPriceWei);
    let maxAvailableDeposit =
      maxAvailableDepositBN->BN.toString->Web3Utils.fromWeiToEth;

    let isAbleToBuy = maxAvailableDepositBN->BN.gt(BN.new_("0"));

    let currentPriceEth = Web3Utils.fromWeiBNToEth(currentPriceWei);
    let currentPriceFloat = Float.fromString(currentPriceEth)->defaultZeroF;
    let currentPriceFloatWithMinimum =
      Js.Math.max_float(currentPriceFloat, 0.005);
    let defaultPriceValue =
      toFixedWithPrecisionNoTrailingZeros(
        currentPriceFloatWithMinimum *. 1.5,
        ~digits=2,
      );
    let defaultMonthlyPatronage =
      toFixedWithPrecisionNoTrailingZeros(
        currentPriceFloatWithMinimum *. 1.5 *. ratio,
        ~digits=3,
      );
    // let priceSliderInitialMax =
    //   toFixedWithPrecisionNoTrailingZeros(
    //     currentPriceFloatWithMinimum *. 3.,
    //     ~digits=3,
    //   );
    let defaultPriceWei = defaultPriceValue->Web3Utils.toWeiFromEth;
    let depositForAYear =
      calcRequiredDepositForTime(
        31536000,
        defaultPriceWei,
        numerator,
        denominator,
      );
    let (defaultDepositTime, defaultDeposit) =
      // TODO: these 'float_of_string' s can throw errors, rather use the Belt library.
      if (depositForAYear->float_of_string
          < maxAvailableDeposit->float_of_string) {
        (31536000, depositForAYear);
      } else {
        (
          calculateDepositDuration(
            maxAvailableDeposit->Web3Utils.toWeiFromEth,
            defaultPriceWei,
            numerator,
            denominator,
          ),
          Js.Math.max_float(0., maxAvailableDeposit->float_of_string)
          ->Js.Float.toString,
        );
      };

    let (newPrice, setInitialPrice) = React.useState(() => defaultPriceValue);
    let (patronage, setPatronage) =
      React.useState(() => defaultMonthlyPatronage);

    // TODO: this should have a minimum value. Say 0.00001 ETH? Tranaction fails if it is zero!
    let (deposit, setInitialDeposit) = React.useState(() => defaultDeposit);
    let (depositTimeInSeconds, setDepositTimeInSeconds) =
      React.useState(() => defaultDepositTime);

    let onSubmitBuy = () => {
      let amountToSend =
        currentPriceWei->BN.add(BN.new_(Web3Utils.toWei(deposit, "ether")));
      switch (priceStatus) {
      | Foreclosed(_)
      | Loading =>
        buyFuncAuction(
          newPrice,
          "150000",
          amountToSend
          // Add 0.001 ETH as a buffer...
          ->BN.add(BN.new_("1000000000000000"))
          ->BN.toString,
        )
      | Price(price) =>
        if (price->BN.gt(BN.new_("0"))) {
          buyFunc(
            newPrice,
            currentPriceWei->BN.toString,
            "150000",
            amountToSend->BN.toString,
          );
        } else {
          buyFuncAuction(
            newPrice,
            "150000",
            amountToSend
            // Add 0.001 ETH as a buffer...
            ->BN.add(BN.new_("1000000000000000"))
            ->BN.toString,
          );
        }
      };
    };

    let setNewPrice = value => {
      let (value, didUpdate) =
        InputHelp.onlyUpdateValueIfPositiveFloat(
          newPrice,
          setInitialPrice,
          value,
        );
      if (didUpdate) {
        let patronage =
          Js.Float.toString(Float.fromString(value)->defaultZeroF *. ratio);
        setPatronage(_ => patronage);
        let timeInSeconds =
          calculateDepositDuration(
            deposit->Web3Utils.toWeiFromEth,
            value->Web3Utils.toWeiFromEth,
            numerator,
            denominator,
          );
        setDepositTimeInSeconds(_ => timeInSeconds);
      } else {
        ();
      };
    };

    // let updatePatronage = value => {
    //   let (value, didUpdate) =
    //     InputHelp.onlyUpdateValueIfPositiveFloat(
    //       patronage,
    //       setPatronage,
    //       value,
    //     );
    //   if (didUpdate) {
    //     let price =
    //       Js.Float.toString(
    //         Float.fromString(value)->defaultZeroF *. ratioInverse,
    //       );
    //     setInitialPrice(_ => price);

    //     let timeInSeconds =
    //       calculateDepositDuration(
    //         deposit->Web3Utils.toWeiFromEth,
    //         price->Web3Utils.toWeiFromEth,
    //         numerator,
    //         denominator,
    //       );
    //     setDepositTimeInSeconds(_ => timeInSeconds);
    //   } else {
    //     ();
    //   };
    // };
    let setDeposit = value => {
      let (value, didUpdate) =
        InputHelp.onlyUpdateValueIfInRangeFloat(
          0.,
          float_of_string(maxAvailableDeposit),
          deposit,
          setInitialDeposit,
          value,
        );
      if (didUpdate) {
        let timeInSeconds =
          calculateDepositDuration(
            value->Web3Utils.toWeiFromEth,
            newPrice->Web3Utils.toWeiFromEth,
            numerator,
            denominator,
          );

        setDepositTimeInSeconds(_ => timeInSeconds);
      } else {
        ();
      };
    };

    <TxTemplate
      txState=txBuyAuctionState closeButtonText="Back to view Animal">
      <TxTemplate txState=txBuyState closeButtonText="Back to view Animal">
        {isAbleToBuy
           ? <BuyInput
               onSubmitBuy
               setNewPrice
               newPrice
               deposit
               depositTimeInSeconds
               setDeposit
               patronage
               tokenIdName
               //  priceSliderInitialMax
               //  depositForAYear
               maxAvailableDeposit
               //  updatePatronage
             />
           : <Rimble.Box>
               <p className=Styles.textOnlyModalText>
                 {React.string(
                    "You do not have enough ether to buy "
                    ++ tokenIdName
                    ++ ".",
                  )}
               </p>
             </Rimble.Box>}
      </TxTemplate>
    </TxTemplate>;
  };
};

module BuyMatic = {
  [@react.component]
  let make = (~tokenId: TokenId.t) => {
    let chain = Client.MaticQuery;
    // let buyTransaction =
    //   GSNActions.useSetupBuyFunction(.
    //     tokenId,
    //     "0x89e2d4628435368a7CD72611E769dDe27802b95e",
    //     "0x0099F841a6aB9A082828fac66134fD25c9d8A195",
    //     5,
    //   );

    /*TEMP CODE:*/
    let web3Context = RootProvider.useWeb3React();
    // let contextMatic = RootProvider.useWeb3ReactId("matic");

    // let web3Context = RootProvider.useWeb3React();

    // let buyTransaction =
    //   GSNActions.useSetupBuyFunction(.
    //     tokenId,
    //     "0x8aC385e66876aaA088C77912D5Bcf6Eec2072675",
    //     "0x3ABbA7caA87722F253D0e1759613105103F83529",
    //     80001,
    //   );
    let (buyTxHash, _setBuyTxHash) = React.useState(() => None);
    let (buyAuctionTxHash, _setBuyAuctionTxHash) = React.useState(() => None);
    // TODO: make this get their real balance from matic:
    let userBalance = BN.new_("100000000000000000000");

    let (numerator, denominator, ratio, _ratioInverse) =
      QlHooks.usePledgeRateDetailed(~chain, tokenId);
    let priceStatus = QlHooks.usePrice(~chain, tokenId);
    let isOnAuction = Animal.useIsOnAuction(~chain, tokenId);
    let launchTimeOpt = QlHooks.useLaunchTimeBN(~chain, tokenId);
    let currentPriceWei =
      Animal.useAuctionPriceWei(
        ~chain,
        tokenId,
        launchTimeOpt->Option.getWithDefault(BN.new_("5000")),
      );

    let currentPriceWei =
      isOnAuction
        ? currentPriceWei
        : (
          switch (priceStatus) {
          | Price(price) => price
          | Loading
          | Foreclosed(_) => BN.new_("0")
          }
        );

    let tokenIdName = "token#" ++ tokenId->TokenId.toString;

    let maxAvailableDepositBN =
      userBalance
      ->BN.sub(BN.new_("3000000000000000")) // 0.003 eth as gas
      ->BN.sub(currentPriceWei);
    // Js.log2(
    //   "Max available deposit",
    //   (userBalance->BN.toString, currentPriceWei->BN.toString),
    // );
    // Js.log2("Max available deposit", maxAvailableDepositBN->BN.toString);
    let maxAvailableDeposit =
      maxAvailableDepositBN->BN.toString->Web3Utils.fromWeiToEth;

    // let isAbleToBuy = maxAvailableDepositBN->BN.gt( BN.new_("0"));
    // TODO: check if the user has enough dai!
    let isAbleToBuy = true;

    let currentPriceEth = Web3Utils.fromWeiBNToEth(currentPriceWei);
    let currentPriceFloat = Float.fromString(currentPriceEth)->defaultZeroF;
    let currentPriceFloatWithMinimum =
      Js.Math.max_float(currentPriceFloat, 0.005);
    let defaultPriceValue =
      toFixedWithPrecisionNoTrailingZeros(
        currentPriceFloatWithMinimum *. 1.5,
        ~digits=2,
      );
    let defaultMonthlyPatronage =
      toFixedWithPrecisionNoTrailingZeros(
        currentPriceFloatWithMinimum *. 1.5 *. ratio,
        ~digits=3,
      );
    // let priceSliderInitialMax =
    //   toFixedWithPrecisionNoTrailingZeros(
    //     currentPriceFloatWithMinimum *. 3.,
    //     ~digits=3,
    //   );
    let defaultPriceWei = defaultPriceValue->Web3Utils.toWeiFromEth;
    let depositForAYear =
      calcRequiredDepositForTime(
        31536000,
        defaultPriceWei,
        numerator,
        denominator,
      );
    let (defaultDepositTime, defaultDeposit) =
      // TODO: these 'float_of_string' s can throw errors, rather use the Belt library.
      if (depositForAYear->float_of_string
          < maxAvailableDeposit->float_of_string) {
        (31536000, depositForAYear);
      } else {
        (
          calculateDepositDuration(
            maxAvailableDeposit->Web3Utils.toWeiFromEth,
            defaultPriceWei,
            numerator,
            denominator,
          ),
          Js.Math.max_float(0., maxAvailableDeposit->float_of_string)
          ->Js.Float.toString,
        );
      };

    let (newPrice, setInitialPrice) = React.useState(() => defaultPriceValue);
    let (patronage, setPatronage) =
      React.useState(() => defaultMonthlyPatronage);

    // TODO: this should have a minimum value. Say 0.00001 ETH? Tranaction fails if it is zero!
    let (deposit, setInitialDeposit) = React.useState(() => defaultDeposit);
    let (depositTimeInSeconds, setDepositTimeInSeconds) =
      React.useState(() => defaultDepositTime);

    let onSubmitBuy = () => {
      let amountToSend =
        currentPriceWei->BN.add(BN.new_(Web3Utils.toWei(deposit, "ether")));
      // switch (priceStatus) {
      // | Foreclosed(_)
      // | Loading =>
      //   GSNActions.buyAuctionFunction(
      //     newPrice,
      //     "150000",
      //     amountToSend
      //     // Add 0.001 ETH as a buffer...
      //     ->BN.add(BN.new_("1000000000000000"))
      //     ->BN.toString,
      //   )
      //   ->ignore
      // | Price(price) =>
      //   if (price->BN.gt(BN.new_("0"))) {
      //     GSNActions.buyFunction(
      //       newPrice,
      //       currentPriceWei->BN.toString,
      //       "150000",
      //       amountToSend->BN.toString,
      //     )
      //     ->ignore;
      //     // buyTransaction(.
      //     //   "65000000000000000000",
      //     //   "50000000000000000000",
      //     //   "150000",
      //     //   "95000000000000000000",
      //     // )
      //     // ->ignore;
      //   } else {
      //     GSNActions.buyAuctionFunction(
      //       newPrice,
      //       "150000",
      //       amountToSend
      //       // Add 0.001 ETH as a buffer...
      //       ->BN.add(BN.new_("1000000000000000"))
      //       ->BN.toString,
      //     )
      //     ->ignore;
      //   }
      // };
      Js.log("CLICKED BUY!!!!!");

      let nonce = "0";
      // let nonce = "1";
      // let nonce = "0x000000000000000000000000000000000000000000000000000000000000000a";
      // let deadline = "0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";

      // GOERLI:
      // let verifyingContract = "0xea9d8a947dD7eBa9cF883c4aa71f18aD5A9c06bB";
      // let spender = "0xf02Bb5b595Af96597b82f39F5de265E77Dc75CbC";

      // MUMBAI:
      let verifyingContract = "0x5bEEeb754cE511908d0374462698B05e963bF35C";
      let spender = "0xD2Dd5BEF69b07090BB183b7F856Df260d1fBf41d";

      // let holder = "0xd3Cbce59318B2E570883719c8165F9390A12BdD6";
      // let from = "0xd3Cbce59318B2E570883719c8165F9390A12BdD6";

      // Js.log2("MATIC", contextMatic);
      // Js.log3("The library", web3Context.library, contextMatic.library);

      // switch (web3Context.library, contextMatic.library) {
      // | (Some(lib), Some(maticLib)) =>
      switch (web3Context.library, web3Context.account) {
      | (Some(lib), Some(account)) =>
        // let value =
        //   DaiPermit.getNonce(
        //     verifyingContract,
        //     maticLib,
        //     web3Context.account,
        //   );
        // Js.log2("THE VALUE", value);

        // // DaiPermit.getNonce(verifyingContract, maticLib, web3Context.account)
        // value
        // ->Js.Promise.then_(
        //     result => {
        //       Js.log2("THE NONCE", result);
        //       Js.Promise.resolve();
        //     },
        //     _,
        //   )
        // ->Js.Promise.catch(
        //     e => {
        //       Js.log2("error", e);
        //       Js.Promise.resolve();
        //     },
        //     _,
        //   )
        // ->ignore;
        DaiPermit.createPermitSig(
          lib.provider,
          verifyingContract,
          nonce,
          BN.newInt_(80001),
          account,
          spender,
          account,
        )
        // Js.log(result);
        //   account,
        //   nonce,
        //   expiry,
        //   allowed,
        //   tokenId,
        //   serviceProviderPercentage,
        //   depositAmount,
        // "to stop compile"
        //       newPrice,
        //       currentPriceWei->BN.toString,
        //       "150000",
        /*       amountToSend->BN.toString*/
        ->Js.Promise.then_(
            rsvSig => {
              open DaiPermit;
              let {r, s, v} = rsvSig;
              execTestTx(.
                web3Context.library,
                web3Context.account,
                spender,
                "0",
                "0",
                true,
                v,
                r,
                s,
                tokenId->TokenId.toString,
                newPrice,
                currentPriceWei->BN.toString,
                "150000",
                amountToSend->BN.toString,
              )
              ->ignore;

              Js.Promise.resolve();
            },
            _,
          )
        ->ignore;
        ();
      | _ => ()
      };
    };

    let setNewPrice = value => {
      let (value, didUpdate) =
        InputHelp.onlyUpdateValueIfPositiveFloat(
          newPrice,
          setInitialPrice,
          value,
        );
      if (didUpdate) {
        let patronage =
          Js.Float.toString(Float.fromString(value)->defaultZeroF *. ratio);
        setPatronage(_ => patronage);
        let timeInSeconds =
          calculateDepositDuration(
            deposit->Web3Utils.toWeiFromEth,
            value->Web3Utils.toWeiFromEth,
            numerator,
            denominator,
          );
        setDepositTimeInSeconds(_ => timeInSeconds);
      } else {
        ();
      };
    };

    // let updatePatronage = value => {
    //   let (value, didUpdate) =
    //     InputHelp.onlyUpdateValueIfPositiveFloat(
    //       patronage,
    //       setPatronage,
    //       value,
    //     );
    //   if (didUpdate) {
    //     let price =
    //       Js.Float.toString(
    //         Float.fromString(value)->defaultZeroF *. ratioInverse,
    //       );
    //     setInitialPrice(_ => price);

    //     let timeInSeconds =
    //       calculateDepositDuration(
    //         deposit->Web3Utils.toWeiFromEth,
    //         price->Web3Utils.toWeiFromEth,
    //         numerator,
    //         denominator,
    //       );
    //     setDepositTimeInSeconds(_ => timeInSeconds);
    //   } else {
    //     ();
    //   };
    // };
    let setDeposit = value => {
      let (value, didUpdate) =
        InputHelp.onlyUpdateValueIfInRangeFloat(
          0.,
          float_of_string(maxAvailableDeposit),
          deposit,
          setInitialDeposit,
          value,
        );
      if (didUpdate) {
        let timeInSeconds =
          calculateDepositDuration(
            value->Web3Utils.toWeiFromEth,
            newPrice->Web3Utils.toWeiFromEth,
            numerator,
            denominator,
          );

        setDepositTimeInSeconds(_ => timeInSeconds);
      } else {
        ();
      };
    };

    <TxTemplateMatic
      txHash=buyAuctionTxHash closeButtonText="Back to view Animal">
      <TxTemplateMatic txHash=buyTxHash closeButtonText="Back to view Animal">
        {isAbleToBuy
           ? <BuyInput
               onSubmitBuy
               setNewPrice
               newPrice
               deposit
               depositTimeInSeconds
               setDeposit
               patronage
               tokenIdName
               //  priceSliderInitialMax
               //  depositForAYear
               maxAvailableDeposit
               //  updatePatronage
             />
           : <Rimble.Box>
               <p className=Styles.textOnlyModalText>
                 {React.string(
                    "You do not have enough ether to buy "
                    ++ tokenIdName
                    ++ ".",
                  )}
               </p>
             </Rimble.Box>}
      </TxTemplateMatic>
    </TxTemplateMatic>;
  };
};

[@react.component]
let make = (~chain, ~tokenId) => {
  // Js.log2(chain, tokenId);
  switch (chain) {
  | Client.Neither
  | Client.MainnetQuery => <BuyMainnet tokenId />
  | Client.MaticQuery => <BuyMatic tokenId />
  // <BuyMatic tokenId />;
  };
};
