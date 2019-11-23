import web3connect from 'web3connect'

class Web3ConnectControl {

  static instance
  core = null
  afterConnectAction
  onConnect
  onDisconnect
  onError
  onClose

  constructor(providerOptions, network) {
    console.log('starting')
    if (Web3ConnectControl.instance) { return Web3ConnectControl.instance }
    Web3ConnectControl.instance = this
    console.log('getting the core')
    this.core = new web3connect.Core({
      network: network,
      // lightboxOpacity: settings.lightboxOpacity,
      providerOptions: providerOptions
    })
    this.afterConnectAction = (event) => console.log('after connection function called (but not set)', event)
    this.onConnect = (event) => console.log('on web3 connection function called (but not set)', event)
    this.onDisconnect = (event) => console.log('on web3 disconnect function called (but not set)', event)
    this.onError = (event) => console.log('There was an error with the web3 connect library', event)
    this.onClose = (event) => console.log('The web3 connection was closed', event)

    this.core.on("connect", this.callOnConnect)
    this.core.on("disconnect", this.callOnDisconnect)
    this.core.on("error", this.callOnError)
    this.core.on("close", this.callOnClose)
  }

  setAfterConnectAction = (newFunction) =>
    this.afterConnectAction = newFunction
  setOnConnect = (newFunction) =>
    this.onConnect = newFunction
  setOnDisconnect = (newFunction) =>
    this.onDisconnect = newFunction
  setOnClose = (newFunction) =>
    this.onClose = newFunction
  setOnError = (newFunction) =>
    this.onError = newFunction

  callOnConnect = (data) => {
    this.onConnect(data)
    this.afterConnectAction(data)
  }
  callOnDisconnect = (data) =>
    this.onDisconnect(data)
  callOnClose = (data) =>
    this.onClose(data)
  callOnError = (data) =>
    this.onError(data)

  toggleModal = () => this.core.toggleModal()
}

const web3connectSettings = {
  walletconnect: {
    package: require("@walletconnect/web3-provider").default, // required
    options: {
      infuraId: "e8fe0574d5124106b82126c48b689bd9" // required
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

export default (network = "mainnet") => new Web3ConnectControl(web3connectSettings, network)
