type injectedType = {isAuthorized: unit => Promise.promise<bool>}

type connectorObj = {
  name: string,
  connector: injectedType,
  img: string,
  connectionPhrase: string,
}

// TODO: move this config to a global file

module InjectedConnector = {
  type connectorOptions = {supportedChainIds: array<int>}
  @module("@web3-react/injected-connector") @new
  external make: connectorOptions => injectedType = "InjectedConnector"
}
module WalletConnectConnector = {
  type connectorOptions<'a> = {
    rpc: 'a,
    bridge: string,
    qrcode: bool,
    pollingInterval: int,
  }
  @module("@web3-react/walletconnect-connector") @new
  external make: connectorOptions<'a> => injectedType = "WalletConnectConnector"
}
module TorusConnector = {
  type connectorOptions = {chainId: int}
  @module("@web3-react/torus-connector") @new
  external make: connectorOptions => injectedType = "TorusConnector"
}
module PortisConnector = {
  type connectorOptions = {
    dAppId: string,
    networks: array<int>,
  }
  @module("@web3-react/portis-connector") @new
  external make: connectorOptions => injectedType = "PortisConnector"
}
module FortmaticConnector = {
  type connectorOptions = {
    apiKey: string,
    chainId: int,
  }
  @module("@web3-react/fortmatic-connector") @new
  external make: connectorOptions => injectedType = "FortmaticConnector"
}
module SquarelinkConnector = {
  type connectorOptions = {
    clientId: string,
    networks: array<int>,
  }
  @module("@web3-react/squarelink-connector") @new
  external make: connectorOptions => injectedType = "SquarelinkConnector"
}

let injected = InjectedConnector.make({supportedChainIds: Config.availableNetworkIds})
