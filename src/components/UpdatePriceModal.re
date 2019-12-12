open Hooks;
open Belt.Option;

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));
module Transaction = {
  [@react.component]
  let make = (~animal: Animal.t) => {
    let (newBuyPrice, setNewBuyPrice) = React.useState(() => "");
    // let currentPrice = useCurrentPriceWei()->mapWithDefault("loading", a => a);
    let currentUser = useCurrentUser();
    // let changePriceObj = useChangePriceTransaction();
    // let changePriceObjNew = useChangePriceTransactionNew();
    let tokenId = Animal.getId(animal);
    // let userBalance =
    //   DrizzleReact.Hooks.useUserBalance()->mapWithDefault("", a => a);

    let (updatePriceFunc, txObjects) = {
      let priceChangeObj = useChangePriceTransactionNew();
      (
        (priceChange, txObject) =>
          priceChangeObj##send(. tokenId, priceChange, txObject),
        priceChangeObj##_TXObjects,
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

    <TxTemplate txObjects>
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
  let (isModalOpen, setModalOpen) = React.useState(() => false);

  let onUnlockMetamaskAndOpenModal = event => {
    ReactEvent.Form.preventDefault(event);
    ReactEvent.Form.stopPropagation(event);
    setModalOpen(_ => true);
  };

  <React.Fragment>
    <Rimble.Box p=1>
      <Rimble.Button onClick=onUnlockMetamaskAndOpenModal>
        {React.string("Price")}
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
        <ModalContainer animal />
      </Rimble.Card>
    </Rimble.Modal>
  </React.Fragment>;
};
