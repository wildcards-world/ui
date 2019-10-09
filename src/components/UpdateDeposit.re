open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;

module UpdateDepositInput = {
  [@bs.module "./UpdateDepositInput"] [@react.component]
  external make:
    (
      ~depositChange: string,
      ~updateDepositChange: ReactEvent.Form.t => unit,
      ~isAddDeposit: bool,
      ~updateIsAddDeposit: bool => unit,
      ~forecloseTokenSelected: bool,
      ~updateForecloseTokenSelected: bool => unit,
      ~onSubmitDepositChange: ReactEvent.Form.t => unit
    ) =>
    React.element =
    "default";
};

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));
module Transaction = {
  [@react.component]
  let make = (~gorilla: Gorilla.gorilla) => {
    let (depositChange, setDepositChange) = React.useState(() => "");
    let (isAddDeposit, setIsAddDeposit) = React.useState(() => true);
    let (forecloseTokenSelected, setForecloseTokenSelected) =
      React.useState(() => false);
    let currentUser = useCurrentUser()->mapWithDefault("", a => a);
    let tokenId = Gorilla.getId(gorilla);

    let availableDeposit =
      (
        switch (tokenId) {
        | None => useDepositAbleToWithdrawWei()
        | Some(tokenIdSet) => useDepositAbleToWithdrawWeiNew(currentUser)
        }
      )
      ->mapWithDefault("0", price => price);

    let onSubmitDepositChange = event => {
      ReactEvent.Form.preventDefault(event);
      Js.log("Buy Clicked");
    };
    let updateDepositChange = event => {
      ReactEvent.Form.preventDefault(event);
      Js.log("Set Deposit");
    };
    let updateIsAddDeposit = bool => {
      Js.log("Set Is ADD Deposit");
    };
    let updateForecloseTokenSelected = bool => {
      Js.log("Set Is ADD Deposit");
    };

    <UpdateDepositInput
      depositChange
      updateDepositChange
      isAddDeposit
      updateIsAddDeposit
      forecloseTokenSelected
      updateForecloseTokenSelected
      onSubmitDepositChange
    />;
  };
};

module ModalContainer = {
  [@react.component]
  let make = (~gorilla: Gorilla.gorilla) => {
    <Transaction gorilla />;
  };
};

[@react.component]
let make = (~gorilla: Gorilla.gorilla) => {
  let (isModalOpen, setModalOpen) = React.useState(() => false);

  let onUnlockMetamaskAndOpenModal = event => {
    ReactEvent.Form.preventDefault(event);

    setModalOpen(_ => true);
  };

  <React.Fragment>
    <Rimble.Button onClick=onUnlockMetamaskAndOpenModal>
      {React.string("Add/Remove Deposit")}
    </Rimble.Button>
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
        <ModalContainer gorilla />
      </Rimble.Card>
    </Rimble.Modal>
  </React.Fragment>;
};
