open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;
open Belt;

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));

module BuyInput = {
  [@bs.module "./BuyModelInput"] [@react.component]
  external make:
    (
      ~onSubmitBuy: unit => unit=?,
      ~setNewPrice: string => unit=?,
      ~newPrice: string=?,
      ~setDeposit: string => unit=?,
      ~priceSliderInitialMax: string=?,
      ~deposit: string=?,
      ~patronage: string=?,
      ~updatePatronage: string => unit=?
    ) =>
    // ~depositError: option(string)=?
    React.element =
    "default";
};

module Transaction = {
  [@react.component]
  let make = (~tokenId: option(string)) => {
    let currentUser = useCurrentUser();
    let buyObj = useBuyTransaction();
    let buyObjNew = useBuyTransactionNew();
    let userBalance =
      DrizzleReact.Hooks.useUserBalance()->mapWithDefault("0", a => a);
    // ->Web3Utils.fromWeiToEth;

    let currentPriceWei =
      (
        switch (tokenId) {
        | None => useCurrentPriceWei()
        | Some(tokenIdSet) => useCurrentPriceWeiNew(tokenIdSet)
        }
      )
      ->mapWithDefault("0", price => price);
    let (defaultPriceValue, defaultMonthlyPatronage, priceSliderInitialMax) = {
      let currentPriceEth = Web3Utils.fromWeiToEth(currentPriceWei);
      let currentPriceFloat =
        Float.fromString(currentPriceEth)->mapWithDefault(0., a => a);
      (
        Js.Float.toPrecisionWithPrecision(currentPriceFloat *. 1.5, 2),
        Js.Float.toPrecisionWithPrecision(currentPriceFloat *. 0.15, 3),
        Js.Float.toPrecisionWithPrecision(currentPriceFloat *. 3., 3),
      );
    };

    let (newPrice, setInitialPrice) = React.useState(() => defaultPriceValue);
    let (patronage, setPatronage) =
      React.useState(() => defaultMonthlyPatronage);
    let (deposit, setInitialDeposit) = React.useState(() => "");
    // let (deposit, setInitialDeposit) = React.useState(() => "");

    let onSubmitBuy = () => {
      // TODO: Abstract this better into a utility library of sorts.
      let setFunctionObj = [%bs.raw {| (value, from) => ({ value, from }) |}];
      let amountToSend =
        BN.new_(newPrice)
        ->BN.addGet(. BN.new_(currentPriceWei))
        ->BN.addGet(. BN.new_(Web3Utils.toWei(deposit, "ether")))
        ->BN.toStringGet(.);
      // TODO: this `##` on the send is clunky, and not so type safe. Find ways to improve it.

      switch (tokenId) {
      | None =>
        buyObj##send(.
          newPrice->Web3Utils.toWeiFromEth,
          setFunctionObj(. amountToSend, currentUser),
        )
      | Some(tokenIdSet) =>
        buyObjNew##send(.
          tokenIdSet,
          newPrice->Web3Utils.toWeiFromEth,
          setFunctionObj(. amountToSend, currentUser),
        )
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
        // TODO: Add error checking here, - `float_of_string` is an unsafe operation in Ocaml (it can throw an error)
        let patronage = Js.Float.toString(float_of_string(value) *. 0.1);
        setPatronage(_ => patronage);
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
        let price = Js.Float.toString(float_of_string(value) *. 10.);
        setInitialPrice(_ => price);
        Js.log("updated price " ++ price ++ ", " ++ value);
      } else {
        ();
      };
    };
    let setDeposit = value => {
      InputHelp.onlyUpdateValueIfPositiveFloat(
        deposit,
        setInitialDeposit,
        value,
      );
      ();
    };

    <BuyInput
      onSubmitBuy
      setNewPrice
      newPrice
      deposit
      setDeposit
      patronage
      updatePatronage
      priceSliderInitialMax
    />;
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
    setModalOpen(_ => true);
  };

  <React.Fragment>
    {if (isProviderSelected) {
       <Rimble.Button onClick=onOpenModal>
         {React.string("Buy")}
       </Rimble.Button>;
     } else {
       //  onClick=openWeb3ConnectModal
       <Web3connect.CustomButton afterConnect=onUnlockMetamaskAndOpenModal>
         {React.string("Buy")}
       </Web3connect.CustomButton>;
     }}
    <Rimble.Modal isOpen=isModalOpen>
      <Rimble.Card width={Rimble.AnyStr("420px")} p=0>
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
