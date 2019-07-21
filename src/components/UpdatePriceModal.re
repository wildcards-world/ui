open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));
module Transaction = {
  [@react.component]
  let make = () => {
    let (newBuyPrice, setNewBuyPrice) = React.useState(() => "");
    let currentPrice = useCurrentPriceWei();
    let currentUser = useCurrentUser();
    let changePriceObj = useChangePriceTransaction();

    Js.log(changePriceObj);

    let onSubmitBuy = event => {
      ReactEvent.Form.preventDefault(event);

      // TODO: Abstract this better into a utility library of sorts.
      let setFunctionObj = [%bs.raw {| (from) => ({ from }) |}];
      // TODO: this `##` on the send is clunky, and not so type safe. Find ways to improve it.
      changePriceObj##send(.
        newBuyPrice->Web3Utils.toWeiFromEth,
        setFunctionObj(currentUser),
      );
    };

    <Rimble.Box p=4 mb=3>
      <Rimble.HeadingS> "Update Price" </Rimble.HeadingS>
      <Rimble.TextS>
        "Enter the desired values for the transaction."
      </Rimble.TextS>
      <Rimble.Input
        _type="number"
        placeholder="New Sale Price"
        onChange={InputHelp.onlyUpdateIfPositiveFloat(
          newBuyPrice,
          setNewBuyPrice,
        )}
        value=newBuyPrice
      />
      <br />
      <Rimble.Button onClick=onSubmitBuy>
        {React.string("Buy")}
      </Rimble.Button>
    </Rimble.Box>;
  };
};

module ModalContainer = {
  [@react.component]
  let make = () => {
    let isUnlocked = useIsUnlocked();

    if (isUnlocked) {
      <Transaction />;
    } else {
      <Rimble.Box p=4 mb=3>
        <Rimble.HeadingS> "NOTICE" </Rimble.HeadingS>
        <Rimble.TextS>
          "Unable to connect to metamask, so unable to sign transactions."
        </Rimble.TextS>
      </Rimble.Box>;
    };
  };
};

[@react.component]
let make = () => {
  let (isModalOpen, setModalOpen) = React.useState(() => false);
  let unlockWeb3IfNotAlready = useUnlockWeb3IfNotAlready();

  let onUnlockMetamaskAndOpenModal = event => {
    ReactEvent.Form.preventDefault(event);

    unlockWeb3IfNotAlready();
    setModalOpen(_ => true);
  };

  <React.Fragment>
    <Rimble.Button onClick=onUnlockMetamaskAndOpenModal>
      {React.string("Update Price")}
    </Rimble.Button>
    <Rimble.Modal isOpen=isModalOpen>
      <Rimble.Card width="420px" p=0>
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
        <ModalContainer />
      </Rimble.Card>
    </Rimble.Modal>
  </React.Fragment>;
};
