open Belt.Option;

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));

[@gentype]
[@react.component]
let make = (~tokenId: TokenId.t) => {
  let (newBuyPrice, setNewBuyPrice) = React.useState(() => "");

  let (updatePriceFunc, txState) = ContractActions.useChangePrice(tokenId);

  let onSubmitBuy = event => {
    ReactEvent.Form.preventDefault(event);

    updatePriceFunc(Web3Utils.toWei(newBuyPrice, "ether"));
  };

  <TxTemplate txState>
    <Rimble.Box p=4 mb=3>
      <Rimble.HeadingS> "Update Price" </Rimble.HeadingS>
      <Rimble.TextS>
        "Enter the desired values for the transaction."
      </Rimble.TextS>
      <Rimble.Input
        _type="number"
        placeholder="New Sale Price"
        onChange={event => {
          let value =
            ReactEvent.Form.target(event)##value->getWithDefault("");
          let _ =
            InputHelp.onlyUpdateValueIfPositiveFloat(
              newBuyPrice,
              setNewBuyPrice,
              value,
            );
          ();
        }}
        value=newBuyPrice
      />
      <br />
      <Rimble.Button onClick=onSubmitBuy>
        {React.string("Update")}
      </Rimble.Button>
    </Rimble.Box>
  </TxTemplate>;
};
