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
    <Transaction />;
  };
};

[@react.component]
let make = () => {
  let (isModalOpen, setModalOpen) = React.useState(() => false);
  let setProvider = useSetProvider();
  let isProviderSelected = useIsProviderSelected();

  let onSetProviderAndOpenModal = provider => {
    setProvider(provider);
    setModalOpen(_ => true);
  };

  let onUnlockMetamaskAndOpenModal = event => {
    ReactEvent.Form.preventDefault(event);
    setModalOpen(_ => true);
  };

  <React.Fragment>
    {if (isProviderSelected) {
       <Rimble.Button onClick=onUnlockMetamaskAndOpenModal>
         {React.string("Buy")}
       </Rimble.Button>;
     } else {
       <Web3connect.CustomButton
         onConnect=onSetProviderAndOpenModal
         providerOptions=[%bs.raw
           {|
        {
          walletconnect: {
            package: require("@walletconnect/web3-provider").default, // required
            options: {
              infuraId: "a5d64a2052ab4d1da240cdfe3a6c519b" // required
            }
          },
          portis: {
            package: require("@portis/web3"), // required
            options: {
              id: "104b9d07-25d3-4aeb-903b-ad7452218d05" // required
            }
          },
          torus: {
            package: require("@toruslabs/torus-embed").default, // required
            options: {
              enableLogging: false, // optional
              buttonPosition: "bottom-left", // optional
              buildEnv: "production", // optional
              showTorusButton: true // optional
            }
          },
          fortmatic: {
            package: require("fortmatic"), // required
            options: {
              key: "pk_live_BE64CE1BB4A49C37" // required
            }
          },
          squarelink: {
            package: require("squarelink"), // required
            options: {
              id: "3904cdd1b675af615ca9" // required
            }
          }
        }
      |}
         ]>
         {React.string("Buy")}
       </Web3connect.CustomButton>;
     }}
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
