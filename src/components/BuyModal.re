open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));

module Transaction = {
  [@react.component]
  let make = () => {
    let (initialBuyPrice, setInitialPrice) = React.useState(() => "");
    let (initialDeposit, setInitialDeposit) = React.useState(() => "");
    let currentPrice = useCurrentPriceWei();
    let currentUser = useCurrentUser();
    let buyObj = useBuyTransaction();

    let onSubmitBuy = event => {
      ReactEvent.Form.preventDefault(event);

      // TODO: Abstract this better into a utility library of sorts.
      let setFunctionObj = [%bs.raw {| (value, from) => ({ value, from }) |}];
      let amountToSend =
        BN.new_(initialBuyPrice)
        ->BN.addGet(. BN.new_(currentPrice))
        ->BN.addGet(. BN.new_(Web3Utils.toWei(initialDeposit, "ether")))
        ->BN.toStringGet(.);
      // TODO: this `##` on the send is clunky, and not so type safe. Find ways to improve it.
      buyObj##send(.
        initialBuyPrice->Web3Utils.toWeiFromEth,
        setFunctionObj(. amountToSend, currentUser),
      );
    };

    <Rimble.Box p=4 mb=3>
      <Rimble.HeadingS> "Purchase" </Rimble.HeadingS>
      <Rimble.TextS>
        "Enter the desired values for the transaction."
      </Rimble.TextS>
      <Rimble.Input
        _type="number"
        placeholder="Your Initial Sale Price"
        onChange={InputHelp.onlyUpdateIfPositiveFloat(
          initialBuyPrice,
          setInitialPrice,
        )}
        value=initialBuyPrice
      />
      <Rimble.Input
        _type="number"
        placeholder="Your Initial Deposit"
        onChange={InputHelp.onlyUpdateIfPositiveFloat(
          initialDeposit,
          setInitialDeposit,
        )}
        value=initialDeposit
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
      {React.string("Buy")}
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
        <ModalContainer />
      </Rimble.Card>
    </Rimble.Modal>
  </React.Fragment>;
};
