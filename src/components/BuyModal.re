open Hooks;
// open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;
open Belt;

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
      ~gorillaName: string,
      ~depositForAYear: string
    ) =>
    // ~depositError: option(string)=?
    React.element =
    "default";
};

let defaultZeroF = maybeFloat => mapWithDefault(maybeFloat, 0., a => a);
let defaultZeroI = maybeInt => mapWithDefault(maybeInt, -1, a => a);
let defaultZeroS = maybeString => mapWithDefault(maybeString, "0", a => a);

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
  let make = (~tokenId: option(string)) => {
    let currentUser = useCurrentUser();
    let (buyFunc, txObjects) =
      switch (tokenId) {
      | None =>
        let buyObj = useBuyTransaction();
        (
          (
            (newPrice, txObject) =>
              buyObj##send(. newPrice->Web3Utils.toWeiFromEth, txObject)
          ),
          buyObj##_TXObjects,
        );
      | Some(tokenIdSet) =>
        let buyObj = useBuyTransactionNew();
        (
          (
            (newPrice, txObject) =>
              buyObj##send(.
                tokenIdSet,
                newPrice->Web3Utils.toWeiFromEth,
                txObject,
              )
          ),
          buyObj##_TXObjects,
        );
      };
    let userBalance = DrizzleReact.Hooks.useUserBalance()->defaultZeroS;

    let (currentPriceWei, numerator, denominator, ratio, ratioInverse) =
      switch (tokenId) {
      | None => (useCurrentPriceWei()->defaultZeroS, "3", "10", 0.025, 40.)
      | Some(tokenIdSet) => (
          useCurrentPriceWeiNew(tokenIdSet)->defaultZeroS,
          "24",
          "10",
          0.2,
          5.,
        )
      };

    let gorillaName = Gorilla.getNameFromId(tokenId);

    let maxAvailableDeposit =
      BN.new_(userBalance)
      ->BN.subGet(. BN.new_("3000000000000000")) // 0.003 eth as gas
      ->BN.subGet(. BN.new_(currentPriceWei))
      ->BN.toStringGet(.)
      ->Web3Utils.fromWeiToEth;

    let currentPriceEth = Web3Utils.fromWeiToEth(currentPriceWei);
    let currentPriceFloat = Float.fromString(currentPriceEth)->defaultZeroF;
    let getMax = [%bs.raw {| (first, second) => Math.max(first,second) |}];
    let currentPriceFloatWithMinimum = getMax(. currentPriceFloat, 0.005);
    let defaultPriceValue =
      Js.Float.toPrecisionWithPrecision(
        currentPriceFloatWithMinimum *. 1.5,
        2,
      );
    let defaultMonthlyPatronage =
      Js.Float.toPrecisionWithPrecision(
        currentPriceFloatWithMinimum *. 1.5 *. ratio,
        3,
      );
    let priceSliderInitialMax =
      Js.Float.toPrecisionWithPrecision(
        currentPriceFloatWithMinimum *. 3.,
        3,
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
          ->string_of_float,
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
        ->BN.addGet(. BN.new_(currentPriceWei))
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
      <BuyInput
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
        gorillaName
        depositForAYear
      />
    </TxTemplate>;
  };
};

module ModalContainer = {
  [@react.component]
  let make = (~tokenId: option(string)) => {
    <Transaction tokenId />;
  };
};

[@react.component]
let make = (~tokenId: option(string)) => {
  let (isModalOpen, setModalOpen) = React.useState(() => false);
  let isProviderSelected = useIsProviderSelected();

  let onUnlockMetamaskAndOpenModal = () => {
    setModalOpen(_ => true);
  };
  let onOpenModal = event => {
    ReactEvent.Form.preventDefault(event);
    ReactEvent.Form.stopPropagation(event);
    setModalOpen(_ => true);
  };

  <React.Fragment>
    {if (isProviderSelected) {
       <Rimble.Button onClick=onOpenModal>
         {React.string("Buy")}
       </Rimble.Button>;
     } else {
       <Web3connect.CustomButton afterConnect=onUnlockMetamaskAndOpenModal>
         {React.string("Buy")}
       </Web3connect.CustomButton>;
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
        <ModalContainer tokenId />
      </Rimble.Card>
    </Rimble.Modal>
  </React.Fragment>;
};
