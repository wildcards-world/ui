open Globals;

module UpdateDepositInput = {
  [@bs.module "./UpdateDepositInput"] [@react.component]
  external make:
    (
      ~depositChange: string,
      ~updateDepositChange: ReactEvent.Form.t => (string, bool),
      ~isAddDeposit: bool,
      ~updateIsAddDeposit: bool => unit,
      ~onSubmitDepositChange: ReactEvent.Form.t => unit
    ) =>
    React.element =
    "default";
};

let getToDisplay = (label, value) =>
  React.string(
    label ++ ": " ++ value->Belt.Option.mapWithDefault("loading", a => a),
  );

[@react.component]
let make = (~closeButtonText, ~chain) => {
  // TODO: We must use the correct client for updating the deposit
  Js.log(
    "The chain context in update deposit" ++ chain->Client.chainContextToStr,
  );
  let (depositChange, setDepositChange) = React.useState(() => "");
  let (isAddDeposit, setIsAddDeposit) = React.useState(() => true);

  let web3Context = RootProvider.useWeb3React();

  let (depositFunc, txWithdrawObject) =
    ContractActions.useUpdateDeposit(
      ~chain,
      false,
      web3Context.library,
      web3Context.account,
      web3Context.chainId->Option.getWithDefault(1),
    );
  let (withdrawFunc, txDepositObject) =
    ContractActions.useWithdrawDeposit(
      ~chain,
      false,
      web3Context.library,
      web3Context.account,
      web3Context.chainId->Option.getWithDefault(1),
    );

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
  <TxTemplate chain txState=txDepositObject closeButtonText>
    <TxTemplate chain txState=txWithdrawObject closeButtonText>
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
