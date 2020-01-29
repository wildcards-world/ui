open Belt.Option;

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));
module Transaction = {
  [@react.component]
  let make = (~animal: Animal.t) => {
    let (newBuyPrice, setNewBuyPrice) = React.useState(() => "");

    let (updatePriceFunc, txState) = AnimalActions.useChangePrice(animal);

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
  let goToPriceUpdate = RootProvider.useGoToPriceUpdate();

  <React.Fragment>
    <Rimble.Box p=1>
      <Rimble.Button onClick={_e => {goToPriceUpdate(animal)}}>
        "Price"->React.string
      </Rimble.Button>
    </Rimble.Box>
  </React.Fragment>;
};
