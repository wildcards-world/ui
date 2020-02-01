import Web3 from "web3";

const getInjectedWeb3NoLoad = () => {
  if (!!window.ethereum && !!window.ethereum.addListener) {
    const { ethereum } = window;
    return ethereum;
  } else if (typeof window.web3 !== "undefined") {
    const { currentProvider } = window.web3.currentProvider;
    return currentProvider;
  }
  try {
    const provider = new Web3.providers.WebsocketProvider(
      "wss://mainnet.infura.io/ws/v3/e8fe0574d5124106b82126c48b689bd9"
    );

    return provider;
  } catch (error) {
    console.error(error);
    // Out of web3 options; throw.
    throw new Error("Cannot find injected web3 or valid fallback.");
  }
};
class Web3ProviderSwitcher {
  static instance;
  provider = null;

  constructor() {
    if (Web3ProviderSwitcher.instance) {
      return Web3ProviderSwitcher.instance;
    }
    Web3ProviderSwitcher.instance = this;
    this.provider = getInjectedWeb3NoLoad();
  }

  async setExternalProvider(provider) {
    this.provider = provider;
  }

  createSwitchableWeb3() {
    const web3RequestHandler = {
      get: (obj, prop) => this.provider[prop]
    };

    return new Proxy({}, web3RequestHandler);
  }
}

export default new Web3ProviderSwitcher();
