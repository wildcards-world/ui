open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));
module Transaction = {
  [@react.component]
  let make = (~gorilla: Gorilla.gorilla) => {
    let (newBuyPrice, setNewBuyPrice) = React.useState(() => "");
    let currentPrice = useCurrentPriceWei();
    let currentUser = useCurrentUser();
    // let changePriceObj = useChangePriceTransaction();
    // let changePriceObjNew = useChangePriceTransactionNew();
    let tokenId = Gorilla.getId(gorilla);
    // let userBalance =
    //   DrizzleReact.Hooks.useUserBalance()->mapWithDefault("", a => a);

    let (updatePriceFunc, txObjects) =
      switch (tokenId) {
      | None =>
        let priceChangeObj = useChangePriceTransaction();
        (
          (
            (priceChange, txObject) =>
              priceChangeObj##send(. priceChange, txObject)
          ),
          priceChangeObj##_TXObjects,
        );
      | Some(tokenIdSet) =>
        let priceChangeObj = useChangePriceTransactionNew();
        (
          (
            (priceChange, txObject) =>
              priceChangeObj##send(. tokenIdSet, priceChange, txObject)
          ),
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
            InputHelp.onlyUpdateValueIfPositiveFloat(
              currentPrice,
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
  let make = (~gorilla: Gorilla.gorilla) => {
    <Transaction gorilla />;
  };
};

[@react.component]
let make = (~gorilla: Gorilla.gorilla) => {
  let (isModalOpen, setModalOpen) = React.useState(() => false);

  let onUnlockMetamaskAndOpenModal = event => {
    ReactEvent.Form.preventDefault(event);
    ReactEvent.Form.stopPropagation(event);
    setModalOpen(_ => true);
  };

  <React.Fragment>
    <Rimble.Button onClick=onUnlockMetamaskAndOpenModal>
      {React.string("Price")}
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
