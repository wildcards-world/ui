open DrizzleReact.Hooks;
open Rimble;

module NoInjectedWeb3 = {
  [@react.component]
  let make = () =>
    <React.Fragment>
      <h3> {React.string("Attempting to connect to Ethereum.")} </h3>
    </React.Fragment>;
  // <p>
  //   <a href="https://metamask.io"> {React.string("Metamask")} </a>
  //   {React.string(" to BUY Vitalik.")}
  // </p>
};

module Loader = {
  module SmartContracts = {
    [@react.component]
    let make = () => <Loader className=Styles.centerItems />;
  };
  module Web3 = {
    [@react.component]
    let make = () => <Loader className=Styles.centerItems />;
  };
};

[@react.component]
let make =
    (
      ~requireSmartContractsLoaded: bool=false,
      ~children,
      ~alternateNoWeb3=<NoInjectedWeb3 />,
      ~alternateLoaderSmartContracts=<Loader.SmartContracts />,
      ~alternateLoaderWeb3=<Loader.Web3 />,
    ) => {
  let state = useDrizzleState(a => a);
  let web3Status = state##web3##status;
  let vitalikStewardInitialized =
    state##contracts##_VitalikSteward##initialized;
  let erc721FullInitialized = state##contracts##_ERC721Full##initialized;
  let erc721PatronageInitialized =
    state##contracts##_ERC721Patronage_v0##initialized;
  let wildcardStewardInitialized =
    state##contracts##_WildcardSteward_v0##initialized;

  let smartContractsLoaded =
    vitalikStewardInitialized
    && erc721FullInitialized
    && erc721PatronageInitialized
    && wildcardStewardInitialized;

  if (web3Status == "initialized") {
    if (!requireSmartContractsLoaded || smartContractsLoaded) {
      // TODO: it may be needed to check for more than just a web3 providor
      //        eg. what if the provider has no ethereum accounts?
      // let isWeb3Enabled = Web3Unlocked.isUsingProvider();
      // if (isWeb3Enabled) {children} else {AlternateNoWeb3};
      children;
    } else {
      alternateLoaderSmartContracts;
    };
  } else {
    alternateLoaderWeb3;
  };
};
