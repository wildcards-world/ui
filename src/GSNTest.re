[@react.component]
let make = () => {
  // useMint();
  // let (mintFunc, txBuyState) = GSNActions.useMint(true);
  let (mintFunc, txBuyState) = GSNActions.useMint(true);

  <div>
    <h1> "testing"->React.string </h1>
    <button onClick={_ => mintFunc()}> "on click"->React.string </button>
    <TxTemplate txState=txBuyState closeButtonText="Back to view Animal">
      "This will show the tx"->React.string
    </TxTemplate>
  </div>;
};
