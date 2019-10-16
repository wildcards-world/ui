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
    let changePriceObj = useChangePriceTransaction();
    let changePriceObjNew = useChangePriceTransactionNew();

    let onSubmitBuy = event => {
      ReactEvent.Form.preventDefault(event);

      // TODO: Abstract this better into a utility library of sorts.
      let setFunctionObj = [%bs.raw {| (from) => ({ from }) |}];
      // TODO: this `##` on the send is clunky, and not so type safe. Find ways to improve it.
      switch (Gorilla.getId(gorilla)) {
      | None =>
        changePriceObj##send(.
          newBuyPrice->Web3Utils.toWeiFromEth,
          setFunctionObj(currentUser),
        )
      | Some(tokenIdSet) =>
        changePriceObjNew##send(.
          tokenIdSet,
          newBuyPrice->Web3Utils.toWeiFromEth,
          setFunctionObj(currentUser),
        )
      };
    };

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
    </Rimble.Box>;
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
      {React.string("Update Price")}
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
