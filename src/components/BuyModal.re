open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));

module BuyInput = {
  [@bs.module "./BuyModelInput"] [@react.component]
  external make:
    (
      ~onSubmitBuy: ReactEvent.Form.t => unit=?,
      ~setNewPrice: ReactEvent.Form.t => unit=?,
      ~newPrice: string=?,
      ~setDeposit: ReactEvent.Form.t => unit=?,
      ~deposit: string=?
    ) =>
    // ~depositError: option(string)=?
    React.element =
    "default";
};

module Transaction = {
  [@react.component]
  let make = (~tokenId: option(string)) => {
    let (newPrice, setInitialPrice) = React.useState(() => "");
    let (deposit, setInitialDeposit) = React.useState(() => "");
    let currentUser = useCurrentUser();
    let buyObj = useBuyTransaction();
    let buyObjNew = useBuyTransactionNew();
    let userBalance =
      DrizzleReact.Hooks.useUserBalance()->mapWithDefault("", a => a);

    let currentPrice =
      (
        switch (tokenId) {
        | None => useCurrentPriceWei()
        | Some(tokenIdSet) => useCurrentPriceWeiNew(tokenIdSet)
        }
      )
      ->mapWithDefault("0", price => price);

    let onSubmitBuy = event => {
      ReactEvent.Form.preventDefault(event);

      // TODO: Abstract this better into a utility library of sorts.
      let setFunctionObj = [%bs.raw {| (value, from) => ({ value, from }) |}];
      let amountToSend =
        BN.new_(newPrice)
        ->BN.addGet(. BN.new_(currentPrice))
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

    let setNewPrice = event => {
      ReactEvent.Form.preventDefault(event);
      InputHelp.onlyUpdateIfPositiveFloat(newPrice, setInitialPrice, event);
    };
    let setDeposit = event => {
      ReactEvent.Form.preventDefault(event);
      InputHelp.onlyUpdateIfPositiveFloat(deposit, setInitialDeposit, event);
    };

    <BuyInput onSubmitBuy setNewPrice newPrice deposit setDeposit />;
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
