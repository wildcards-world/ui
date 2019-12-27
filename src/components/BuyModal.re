// open Providers.RootProvider;
open Belt;
open Accounting;

module BuyInput = {
  [@bs.module "./BuyModelInput"] [@react.component]
  external make:
    (
      ~onSubmitBuy: unit => unit=?,
      ~setNewPrice: string => unit=?,
      ~newPrice: string=?,
      ~depositTimeInSeconds: int,
      ~setDeposit: string => unit=?,
      ~maxAvailableDeposit: string,
      ~priceSliderInitialMax: string=?,
      ~deposit: string=?,
      ~patronage: string=?,
      ~updatePatronage: string => unit=?,
      ~animalName: string,
      ~depositForAYear: string
    ) =>
    // ~depositError: option(string)=?
    React.element =
    "default";
};

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

module Transaction = {
  [@react.component]
  let make = (~animal: Animal.t) => {
    let currentUser = RootProviderNew.useCurrentUser();
    let (buyFunc, txObjects) = Animal.useBuy(animal);
    let userBalance =
      Belt.Option.mapWithDefault(
        RootProviderNew.useEthBalance(), BN.new_("0"), a =>
        a
      );

    let (numerator, denominator, ratio, ratioInverse) =
      Animal.pledgeRate(animal);
    let currentPriceWei =
      QlHooks.usePrice(animal)
      ->Belt.Option.mapWithDefault(BN.new_("0"), a => a);

    let animalName = Animal.getName(animal);

    let maxAvailableDepositBN =
      userBalance
      ->BN.subGet(. BN.new_("3000000000000000")) // 0.003 eth as gas
      ->BN.subGet(. currentPriceWei);
    let maxAvailableDeposit =
      maxAvailableDepositBN->BN.toStringGet(.)->Web3Utils.fromWeiToEth;

    let isAbleToBuy = maxAvailableDepositBN->BN.gtGet(. BN.new_("0"));

    let currentPriceEth = Web3Utils.fromWeiBNToEth(currentPriceWei);
    let currentPriceFloat = Float.fromString(currentPriceEth)->defaultZeroF;
    let getMax = [%bs.raw {| (first, second) => Math.max(first,second) |}];
    let currentPriceFloatWithMinimum = getMax(. currentPriceFloat, 0.005);
    let defaultPriceValue =
      Js.Float.toPrecisionWithPrecision(
        currentPriceFloatWithMinimum *. 1.5,
        ~digits=2,
      );
    let defaultMonthlyPatronage =
      Js.Float.toPrecisionWithPrecision(
        currentPriceFloatWithMinimum *. 1.5 *. ratio,
        ~digits=3,
      );
    let priceSliderInitialMax =
      Js.Float.toPrecisionWithPrecision(
        currentPriceFloatWithMinimum *. 3.,
        ~digits=3,
      );
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
    let (deposit, setInitialDeposit) = React.useState(() => defaultDeposit);
    let (depositTimeInSeconds, setDepositTimeInSeconds) =
      React.useState(() => defaultDepositTime);

    let onSubmitBuy = () => {
      // TODO: Abstract this better into a utility library of sorts.
      let setFunctionObj = [%bs.raw {| (value, from) => ({ value, from }) |}];
      let amountToSend =
        BN.new_(newPrice)
        ->BN.addGet(. currentPriceWei)
        ->BN.addGet(. BN.new_(Web3Utils.toWei(deposit, "ether")))
        ->BN.toStringGet(.);
      buyFunc(newPrice, setFunctionObj(. amountToSend, currentUser));
    };

    let setNewPrice = value => {
      let (value, didUpdate) =
        InputHelp.onlyUpdateValueIfPositiveFloat(
          newPrice,
          setInitialPrice,
          value,
        );
      if (didUpdate) {
        // TODO: Add error checking here, - `float_of_string` is an unsafe operation in Ocaml (it can throw an error)
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

    let updatePatronage = value => {
      let (value, didUpdate) =
        InputHelp.onlyUpdateValueIfPositiveFloat(
          patronage,
          setPatronage,
          value,
        );
      if (didUpdate) {
        // TODO: Add error checking here, - `float_of_string` is an unsafe operation in Ocaml (it can throw an error)
        let price =
          Js.Float.toString(
            Float.fromString(value)->defaultZeroF *. ratioInverse,
          );
        setInitialPrice(_ => price);

        let timeInSeconds =
          calculateDepositDuration(
            deposit->Web3Utils.toWeiFromEth,
            price->Web3Utils.toWeiFromEth,
            numerator,
            denominator,
          );
        setDepositTimeInSeconds(_ => timeInSeconds);
      } else {
        ();
      };
    };
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

    <TxTemplate txObjects>
      {isAbleToBuy
         ? <BuyInput
             onSubmitBuy
             setNewPrice
             newPrice
             deposit
             depositTimeInSeconds
             setDeposit
             patronage
             updatePatronage
             priceSliderInitialMax
             maxAvailableDeposit
             animalName
             depositForAYear
           />
         : <Rimble.Box>
             <p className=Styles.textOnlyModalText>
               {React.string(
                  "You do not have enough ether to buy " ++ animalName ++ ".",
                )}
             </p>
           </Rimble.Box>}
    </TxTemplate>;
  };
};

[@react.component]
let make = (~animal: Animal.t) => {
  let (isModalOpen, setModalOpen) = React.useState(() => false);
  let isProviderSelected = RootProviderNew.useIsProviderSelected();

  // let onUnlockMetamaskAndOpenModal = () => {
  //   setModalOpen(_ => true);
  // };
  let onOpenModal = event => {
    ReactEvent.Form.preventDefault(event);
    ReactEvent.Form.stopPropagation(event);
    setModalOpen(_ => true);
  };

  let currentPriceWei = QlHooks.usePrice(animal);
  let setLogin = RootProviderNew.useSetLogin();
  let setBuyView = RootProviderNew.useSetBuyView();

  // TODO:: check if foreclosed!!
  let buttonText =
    switch (currentPriceWei) {
    | Some(price) => price->BN.gtGet(. BN.new_("0")) ? "Buy" : "Claim"
    | None => "loading"
    };

  <React.Fragment>
    {if (isProviderSelected) {
       <Rimble.Button onClick={_e => {setBuyView(true)}}>
         {React.string(buttonText)}
       </Rimble.Button>;
     } else {
       <Rimble.Button
         onClick={_e => {
           setLogin(true);
           setBuyView(true);
         }}>
         {React.string(buttonText)}
       </Rimble.Button>;
     }}
    <Rimble.Modal isOpen=isModalOpen>
      <Rimble.Card width={Rimble.AnyStr("70%")} p=0>
        <Rimble.Button.Text
          icononly=true
          icon="Close"
          color="moon-gray"
          position="absolute"
          top=0
          right=0
          m=3
          onClick={_ => setModalOpen(_ => false)}
        />
        <Transaction animal />
      </Rimble.Card>
    </Rimble.Modal>
  </React.Fragment>;
};
