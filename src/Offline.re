open DrizzleReact.Hooks;
open Rimble;

module NoInjectedWeb3 = {
  [@react.component]
  let make = () =>
    <h3>
      {React.string("Install ")}
      // TODO: if the device is detected to be a mobile, it should offer an alturnative that works well on mobile
      <a href="https://metamask.io"> {React.string("Metamask")} </a>
      {React.string(" to BUY Vitalik.")}
    </h3>;
};

module Loader = {
  module SmartContracts = {
    [@react.component]
    let make = () =>
      <div>
        <Loader />
        <p> {React.string("Web3 loaded")} </p>
        <p> {React.string("Connecting to SmartContracts...")} </p>
      </div>;
  };
  module Web3 = {
    [@react.component]
    let make = () =>
      <div> <p> {React.string("Connecting to Web3...")} </p> </div>;
  };
};

[@react.component]
let make =
    (
      ~requireSmartContractsLoaded: bool=false,
      ~children,
      ~alturnateNoWeb3=<NoInjectedWeb3 />,
      ~alturnateLoaderSmartContracts=<Loader.SmartContracts />,
      ~alturnateLoaderWeb3=<Loader.Web3 />,
    ) => {
  let state = useDrizzleState(a => a);
  let web3Status = state##web3##status;
  let vitalikStewardInitialized =
    state##contracts##_VitalikSteward##initialized;
  let erc721FullInitialized = state##contracts##_ERC721Full##initialized;

  let smartContractsLoaded =
    vitalikStewardInitialized && erc721FullInitialized;

  if (web3Status == "initialized") {
    if (!requireSmartContractsLoaded || smartContractsLoaded) {
      // TODO: it may be needed to check for more than just a web3 providor
      //        eg. what if the provider has no ethereum accounts?
      let isWeb3Enabled = Web3Unlocked.isUsingProvider();
      if (isWeb3Enabled) {children} else {alturnateNoWeb3};
    } else {
      alturnateLoaderSmartContracts;
    };
  } else {
    alturnateLoaderWeb3;
  };
};
