open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;

module UpdateDepositInput = {
  [@bs.module "./UpdateDepositInput"] [@react.component]
  external make:
    (
      ~depositChange: string,
      ~updateDepositChange: ReactEvent.Form.t => (string, bool),
      ~isAddDeposit: bool,
      ~updateIsAddDeposit: bool => unit,
      ~onSubmitDepositChange: ReactEvent.Form.t => (string, bool)
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
    let currentUser = useCurrentUser()->mapWithDefault("", a => a);
    let tokenId = Gorilla.getId(gorilla);
    // let userBalance =
    //   DrizzleReact.Hooks.useUserBalance()->mapWithDefault("", a => a);

    let (withdrawFunc, txWithdrawObjects) =
      switch (tokenId) {
      | None =>
        let withdrawObj = useWithdrawTransaction();
        (
          (
            (depositChange, txObject) =>
              withdrawObj##send(. depositChange, txObject)
          ),
          withdrawObj##_TXObjects,
        );
      | Some(tokenIdSet) =>
        let withdrawObj = useWithdrawTransactionNew();
        (
          (
            (depositChange, txObject) =>
              withdrawObj##send(. depositChange, txObject)
          ),
          withdrawObj##_TXObjects,
        );
      };
    let (depositFunc, txDepositObjects) =
      switch (tokenId) {
      | None =>
        let depositObj = useAddDepositTransaction();
        ((txObject => depositObj##send(. txObject)), depositObj##_TXObjects);
      | Some(tokenIdSet) =>
        let depositObj = useAddDepositTransactionNew();
        ((txObject => depositObj##send(. txObject)), depositObj##_TXObjects);
      };

    let availableDeposit =
      (
        switch (gorilla) {
        | None => useDepositAbleToWithdrawWei()
        | _ => useDepositAbleToWithdrawWeiNew(currentUser)
        }
      )
      ->mapWithDefault("0", price => price);

    let onSubmitDepositChange = event => {
      ReactEvent.Form.preventDefault(event);
      let depositChangeWei = Web3Utils.toWei(depositChange, "ether");
      let setFunctionObj = [%bs.raw {| (value, from) => ({ value, from }) |}];

      if (isAddDeposit) {
        depositFunc(setFunctionObj(. depositChangeWei, currentUser));
      } else {
        withdrawFunc(depositChangeWei, setFunctionObj(. "0", currentUser));
      };
    };

    let updateDepositChange = event => {
      ReactEvent.Form.preventDefault(event);
      InputHelp.onlyUpdateIfPositiveFloat(
        depositChange,
        setDepositChange,
        event,
      );
    };
    let updateIsAddDeposit = isDeposit => {
      setIsAddDeposit(_ => isDeposit);
    };
    // <TxTemplate>
    <TxTemplate txObjects=txDepositObjects>
      <TxTemplate txObjects=txWithdrawObjects>
        <UpdateDepositInput
          depositChange
          updateDepositChange
          isAddDeposit
          updateIsAddDeposit
          onSubmitDepositChange
        />
      </TxTemplate>
    </TxTemplate>;
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
    <Rimble.Box p=2>
      <Rimble.Button onClick=onUnlockMetamaskAndOpenModal>
        {React.string("Add/Remove Deposit")}
      </Rimble.Button>
    </Rimble.Box>
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
