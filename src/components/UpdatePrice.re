open Hooks;
open Belt.Option;

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));
module Transaction = {
  [@react.component]
  let make = (~animal: Animal.t) => {
    let (newBuyPrice, setNewBuyPrice) = React.useState(() => "");
    // let currentPrice = useCurrentPriceWei()->mapWithDefault("loading", a => a);
    let currentUser = RootProvider.useCurrentUser();
    // let changePriceObj = useChangePriceTransaction();
    // let changePriceObjNew = useChangePriceTransactionNew();
    // let userBalance =
    //   DrizzleReact.Hooks.useUserBalance()->mapWithDefault("", a => a);

    let (updatePriceFunc, txState) = {
      let tokenId = Animal.getId(animal);
      (
        // let priceChangeObj = useChangePriceTransactionNew();
        // (
        //   (priceChange, txObject) =>
        //     priceChangeObj##send(. tokenId, priceChange, txObject),
        //   priceChangeObj##_TXObjects,
        // );
        (priceChange, txObject) => {
          Js.log("TODO: implement sending price change!!");
          Js.log(priceChange);
          Js.log(txObject);
        },
        AnimalActions.UnInitialised,
      );
    };

    let onSubmitBuy = event => {
      ReactEvent.Form.preventDefault(event);

      // TODO: Abstract this better into a utility library of sorts.
      let setFunctionObj = [%bs.raw {| (from) => ({ from }) |}];

      updatePriceFunc(
        newBuyPrice->Web3Utils.toWeiFromEth,
        setFunctionObj(. currentUser),
      );
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
  let goToDepositUpdate = RootProvider.useGoToDepositUpdate();

  <React.Fragment>
    <Rimble.Box p=1>
      <Rimble.Button onClick={_e => {goToDepositUpdate(animal)}}>
        "Price"->React.string
      </Rimble.Button>
    </Rimble.Box>
  </React.Fragment>;
};
