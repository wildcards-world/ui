open Hooks;
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
  let make = (~animal: Animal.t) => {
    Js.log(animal);
    let (depositChange, setDepositChange) = React.useState(() => "");
    let (isAddDeposit, setIsAddDeposit) = React.useState(() => true);

    let (depositFunc, txWithdrawObject) = AnimalActions.useUpdateDeposit();
    let (withdrawFunc, txDepositObject) = AnimalActions.useWithdrawDeposit();

    // let _availableDeposit =
    //   useDepositAbleToWithdrawWeiNew(currentUser)
    //   ->mapWithDefault("0", price => price);

    let onSubmitDepositChange = event => {
      ReactEvent.Form.preventDefault(event);
      let depositChangeWei = Web3Utils.toWei(depositChange, "ether");

      if (isAddDeposit) {
        depositFunc(depositChangeWei);
      } else {
        withdrawFunc(depositChangeWei);
      };
      ("implement", true);
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
    <TxTemplate txState=txDepositObject>
      <TxTemplate txState=txWithdrawObject>
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
  let make = (~animal: Animal.t) => {
    <Transaction animal />;
  };
};

[@react.component]
let make = (~animal: Animal.t) => {
  // TODO: if the token is foreclosed handle that logic... (say something like -- "add deposit quick! to keep your token")
  let goToDepositUpdate = RootProvider.useGoToDepositUpdate();

  <React.Fragment>
    <Rimble.Button onClick={_e => {goToDepositUpdate(animal)}}>
      "Deposit"->React.string
    </Rimble.Button>
  </React.Fragment>;
};
