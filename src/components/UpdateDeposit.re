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
    React.useState(() => false);
    let currentUser = useCurrentUser()->mapWithDefault("", a => a);
    // let tokenId = Gorilla.getId(gorilla);
    let userBalance =
      DrizzleReact.Hooks.useUserBalance()->mapWithDefault("", a => a);
    let withdrawTxObj = useWithdrawTransaction();
    let addDepositTxObj = useAddDepositTransaction();
    let withdrawTxObjNew = useWithdrawTransactionNew();
    let addDepositTxObjNew = useAddDepositTransactionNew();

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
      switch (gorilla) {
      | None =>
        if (isAddDeposit) {
          addDepositTxObj##send(.
            setFunctionObj(. depositChangeWei, currentUser),
          );
        } else {
          withdrawTxObj##send(.
            depositChangeWei,
            setFunctionObj(. "0", currentUser),
          );
        }
      | _ =>
        if (isAddDeposit) {
          addDepositTxObjNew##send(.
            setFunctionObj(. depositChangeWei, currentUser),
          );
        } else {
          withdrawTxObjNew##send(.
            depositChangeWei,
            setFunctionObj(. "0", currentUser),
          );
        }
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

    <UpdateDepositInput
      depositChange
      updateDepositChange
      isAddDeposit
      updateIsAddDeposit
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
