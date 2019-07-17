open DrizzleReact.Hooks;
open Rimble;

[@react.component]
let make = (~requireSmartContractsLoaded: bool=false, ~children) => {
  let state = useDrizzleState(a => a);
  let web3Status = state##web3##status;
  let vitalikStewardInitialized =
    state##contracts##_VitalikSteward##initialized;
  let erc721FullInitialized = state##contracts##_ERC721Full##initialized;

  let smartContractsLoaded =
    vitalikStewardInitialized && erc721FullInitialized;

  if (web3Status == "initialized") {
    if (!requireSmartContractsLoaded || smartContractsLoaded) {
      <React.Fragment> children </React.Fragment>;
    } else {
      <div>
        <Loader />
        <p> {React.string("Web3 loaded")} </p>
        <p> {React.string("Connecting to SmartContracts...")} </p>
      </div>;
    };
  } else {
    <div> <Loader /> <p> {React.string("Connecting to Web3...")} </p> </div>;
  };
};
