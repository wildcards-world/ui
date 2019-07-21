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

    Js.log("initialDeposit");
    Js.log(initialDeposit);
    Js.log("initialBuyPrice");
    Js.log(initialBuyPrice);

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
        className="test"
        _type="number"
        placeholder="Your Initial Sale Price"
        onChange={InputHelp.onlyUpdateIfPositiveFloat(
          initialBuyPrice,
          setInitialPrice,
        )}
        value=initialBuyPrice
      />
      <Rimble.Input
        className="test"
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
  //   {this.state.connectedToInjectedWeb3 ?
  //     <Box p={4} mb={3}>{
  //       transactionProcessing ?
  //         <Fragment>
  //           <Heading.h3>Processing Transaction</Heading.h3>
  //           <p>{transactionStatus}</p>
  //           {!!txHash && <a href={'https://etherscan.io/tx/' + txHash} target="_blank">View transaction on Ethersan</a>}
  //           {!txComplete && <Loader color="red" size="80px" />}
  //         </Fragment>
  //         :
  //         <Fragment>
  //           <Heading.h3>Purchase</Heading.h3>
  //           <Text>
  //             Enter the desired values for the transaction.
  //           </Text>
  //           <form className="pure-form pure-form-stacked" onSubmit={this.handleSubmit}>
  //             <Input
  //               key='_newPrice'
  //               type='number'
  //               name='_newPrice'
  //               value={this.state.contractFunctions['_newPrice']}
  //               placeholder={"Your Initial Sale Price"}
  //               onChange={this.handleInputChange}
  //               style={{ width: '100%' }}
  //               startAdornment={<InputAdornment position="start">ETH</InputAdornment>}
  //             />
  //             <Fragment>
  //               <br />
  //               <Input
  //                 key={valueLabel}
  //                 type='number'
  //                 name='value'
  //                 value={this.state.contractFunctions[valueLabel]}
  //                 placeholder={valueLabel}
  //                 onChange={this.handleInputChange}
  //                 style={{ width: '100%' }}
  //                 startAdornment={<InputAdornment position="start">ETH</InputAdornment>} />
  //               <br />
  //               <br />
  //             </Fragment>
  //           </form>
  //           <TokenOverview />
  //         </Fragment>}
  //     </Box>
  //     :
  //     <Box p={4} mb={3}>
  //       <Heading.h3>NOTICE</Heading.h3>
  //       <Text>
  //         Unable to connect to metamask, so unable to sign transactions.
  //       </Text>
  //     </Box>
  //   }
  //   {(!transactionProcessing) && <Flex px={4} py={3} borderTop={1} borderColor={'#E8E8E8'} justifyContent={'flex-end'}>
  //     {/* <Button.Outline>Cancel</Button.Outline> In the future this could be for resetting the values or something*/}
  //     <Button
  //       mainColor="#6bad3e"
  //       ml={3}
  //       onClick={this.handleSubmit}
  //     >Buy Vitalik</Button>
  //   </Flex>}
  // <div>
  //   {getToDisplay("useCurrentPriceWei", currentPriceWei)}
  //   <br />
  //   {getToDisplay("useCurrentPriceEth", useCurrentPriceEth())}
  //   <br />
  //   {getToDisplay("useCurrentPriceUsd", useCurrentPriceUsd())}
  //   <br />
  //   {getToDisplay("useCurrentPatron", useCurrentPatron())}
  //   <br />
  //   {getToDisplay("useUsdPrice", useUsdPrice()->map(Js.Float.toString))}
  //   <br />
  //   {getToDisplay("useTotalPatronageWei", useTotalPatronageWei())}
  //   <br />
  //   {getToDisplay("useTotalPatronageEth", useTotalPatronageEth())}
  //   <br />
  //   {getToDisplay("useTotalPatronageUsd", useTotalPatronageUsd())}
  //   <br />
  //   {getToDisplay(
  //      "useDepositAbleToWithdrawWei",
  //      useDepositAbleToWithdrawWei(),
  //    )}
  //   <br />
  //   {getToDisplay(
  //      "useDepositAbleToWithdrawEth",
  //      useDepositAbleToWithdrawEth(),
  //    )}
  //   <br />
  //   {getToDisplay(
  //      "useDepositAbleToWithdrawUsd",
  //      useDepositAbleToWithdrawUsd(),
  //    )}
  //   <br />
  //   {getToDisplay(
  //      "useForeclosureTime",
  //      useForeclosureTime()->map(Js.Date.toString),
  //    )}
  //   <br />
  //   {if (isUnlocked) {
  //      <React.Fragment>
  //        <Rimble.Input
  //          className="test"
  //          _type="number"
  //          placeholder="Your Initial Sale Price"
  //          onChange=onChangeBuyEther
  //          value=buyAmountEther
  //        />
  //        <Rimble.Button onClick=onSubmitBuy>
  //          {React.string("Buy")}
  //        </Rimble.Button>
  //      </React.Fragment>;
  //    } else {
  //      <Rimble.Button onClick=unlockWeb3IfNotAlready>
  //        {React.string("Enable Metamask")}
  //      </Rimble.Button>;
  //    }}
  //   <br />
  // </div>;
  // };
  //   ();
  //   Js.log(result);
  //     );
  //       setFunctionObj(. amountToSend, currentUser),
  //       buyAmountEther->Web3Utils.toWeiFromEth,
  //     buyObj##send(.
  //   let result =
  //     ->BN.toStringGet(.);
  //     ->BN.addGet(. BN.new_(currentPrice))
  //     BN.new_(buyAmountEther)
  //   let amountToSend =
  //   let setFunctionObj = [%bs.raw {| (value, from) => ({ value, from }) |}];
  //   ReactEvent.Form.preventDefault(event);
  // let onSubmitBuy = event => {
  // }
  //   // setModalOpen(true);
  //   let isUnlocked = unlockWeb3IfNotAlready();
  //   setBuyAmountEther(_ => value);
  //   let value = ReactEvent.Form.target(event)##value->getWithDefault("");
  //   ReactEvent.Form.preventDefault(event);
  // let onChangeBuyEther = event => {
  // // TODO: Add this function as part of the "Rimble.Input" component
  // let currentPriceWei = useCurrentPriceWei();
  // let currentUser = useCurrentUser();
  // let currentPrice = useCurrentPriceWei();
  // let (buyAmountEther, setBuyAmountEther) = React.useState(() => "");
  // let isModalOpen = true;
};
