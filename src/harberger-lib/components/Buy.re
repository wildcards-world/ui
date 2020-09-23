open Globals;
open Accounting;

let calcPricePerSecond = (price, numerator, denominator) => {
  let priceBn = BN.new_(price);
  let numeratorBn = BN.new_(numerator);
  let denominatorBn = BN.new_(denominator);
  let fullYearSeconds = BN.new_("31536000");

  priceBn
  ->BN.mulGet(. numeratorBn)
  ->BN.divGet(. denominatorBn)
  ->BN.divGet(. fullYearSeconds);
};

// TODO: Could cached and stored so that all values don't need to be culculated each time!
// this calculates pricePerSecondEach time.
let calculateDepositDuration = (deposit, price, numerator, denominator) => {
  let depositBn = BN.new_(deposit);
  let pricePerSecond = calcPricePerSecond(price, numerator, denominator);

  depositBn
  ->BN.divGet(.
      if (pricePerSecond->BN.gtGet(. BN.new_("0"))) {
        pricePerSecond;
      } else {
        BN.new_("1");
      },
    )
  ->BN.toStringGet(.)
  ->Int.fromString
  ->defaultZeroI;
  // Check, 9007199254740992 is the largest integer available to javascript.
};

let calcRequiredDepositForTime = (time, price, numerator, denominator) => {
  let timeBn = BN.new_(string_of_int(time));
  let pricePerSecond = calcPricePerSecond(price, numerator, denominator);

  let requiredDeposit =
    timeBn->BN.mulGet(. pricePerSecond)->BN.toStringGet(.);

  requiredDeposit->Web3Utils.fromWeiToEth;
};

[@gentype]
[@react.component]
let make = (~tokenId: TokenId.t) => {
  let (buyFunc, txBuyState) = ContractActions.useBuy(tokenId, false);
  let (buyFuncAuction, txBuyAuctionState) =
    ContractActions.useBuyAuction(tokenId, false);
  let userBalance =
    Belt.Option.mapWithDefault(RootProvider.useEthBalance(), BN.new_("0"), a =>
      a
    );

  let (numerator, denominator, ratio, _ratioInverse) =
    QlHooks.usePledgeRateDetailed(tokenId);
  let priceStatus = QlHooks.usePrice(tokenId);
  let isOnAuction = Animal.useIsOnAuction(tokenId);
  let launchTimeOpt = QlHooks.useLaunchTimeBN(tokenId);
  let currentPriceWei =
    Animal.useAuctionPriceWei(
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
    ->BN.subGet(. BN.new_("3000000000000000")) // 0.003 eth as gas
    ->BN.subGet(. currentPriceWei);
  let maxAvailableDeposit =
    maxAvailableDepositBN->BN.toStringGet(.)->Web3Utils.fromWeiToEth;

  let isAbleToBuy = maxAvailableDepositBN->BN.gtGet(. BN.new_("0"));

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
    if (depositForAYear->float_of_string < maxAvailableDeposit->float_of_string) {
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
      currentPriceWei->BN.addGet(.
        BN.new_(Web3Utils.toWei(deposit, "ether")),
      );
    switch (priceStatus) {
    | Foreclosed(_)
    | Loading =>
      buyFuncAuction(
        newPrice,
        "150000",
        amountToSend
        // Add 0.001 ETH as a buffer...
        ->BN.addGet(. BN.new_("1000000000000000"))
        ->BN.toStringGet(.),
      )
    | Price(price) =>
      if (price->BN.gtGet(. BN.new_("0"))) {
        buyFunc(
          newPrice,
          currentPriceWei->BN.toStringGet(.),
          "150000",
          amountToSend->BN.toStringGet(.),
        );
      } else {
        buyFuncAuction(
          newPrice,
          "150000",
          amountToSend
          // Add 0.001 ETH as a buffer...
          ->BN.addGet(. BN.new_("1000000000000000"))
          ->BN.toStringGet(.),
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

  <TxTemplate txState=txBuyAuctionState closeButtonText="Back to view Animal">
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
                  "You do not have enough ether to buy " ++ tokenIdName ++ ".",
                )}
             </p>
           </Rimble.Box>}
    </TxTemplate>
  </TxTemplate>;
};
