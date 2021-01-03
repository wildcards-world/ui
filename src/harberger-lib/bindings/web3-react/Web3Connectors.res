type injectedType = {isAuthorized: unit => Promise.promise<bool>}

@module("./connectors") external injected: injectedType = "injected"
@dead("+sideChainNetwork") @module("./connectors")
external sideChainNetwork: int => injectedType = "sideChainNetwork"
module Custom = {
  @dead("Custom.+make") @module("./web3CustomRoot") @react.component
  external make: (
    ~id: string,
    ~getLibrary: Web3.rawProvider => Web3.web3Library,
    ~children: React.element,
  ) => React.element = "default"
}