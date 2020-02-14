import { InjectedConnector } from "@web3-react/injected-connector";
// import { NetworkConnector } from "@web3-react/network-connector";
import { WalletConnectConnector } from "@web3-react/walletconnect-connector";
// import { WalletLinkConnector } from "@web3-react/walletlink-connector";
// import { LedgerConnector } from "@web3-react/ledger-connector";
// import { TrezorConnector } from "@web3-react/trezor-connector";
// import { FrameConnector } from "@web3-react/frame-connector";
import { FortmaticConnector } from "@web3-react/fortmatic-connector";
import { PortisConnector } from "@web3-react/portis-connector";
import { SquarelinkConnector } from "@web3-react/squarelink-connector";
import { TorusConnector } from "@web3-react/torus-connector";
// import { AuthereumConnector } from "@web3-react/authereum-connector";

const POLLING_INTERVAL = 8000;
const RPC_URLS = {
  1: "https://mainnet.infura.io/v3/84842078b09946638c03157f83405213",
  5: "https://goerli.infura.io/v3/84842078b09946638c03157f83405213"
};

export const injected = new InjectedConnector({
  // TODO: make this only accept 2 networks , mainnet and testnet. Then display a message when people use the wrong network.
  //   supportedChainIds: [1, 5]
  supportedChainIds: [1, 3, 4, 5, 42]
});

// export const network = new NetworkConnector({
//   urls: { 1: RPC_URLS[1], 5: RPC_URLS[5] },
//   defaultChainId: 1,
//   pollingInterval: POLLING_INTERVAL
// });

export const walletconnect = new WalletConnectConnector({
  rpc: { 1: RPC_URLS[1] },
  bridge: "https://bridge.walletconnect.org",
  qrcode: true,
  pollingInterval: POLLING_INTERVAL
});

// export const walletlink = new WalletLinkConnector({
//   url: RPC_URLS[1],
//   appName: "web3-react example"
// });

// export const ledger = new LedgerConnector({
//   chainId: 1,
//   url: RPC_URLS[1],
//   pollingInterval: POLLING_INTERVAL
// });

// export const trezor = new TrezorConnector({
//   chainId: 1,
//   url: RPC_URLS[1],
//   pollingInterval: POLLING_INTERVAL,
//   manifestEmail: "dummy@abc.xyz",
//   manifestAppUrl: "https://8rg3h.csb.app/"
// });

// export const frame = new FrameConnector({ supportedChainIds: [1] });

export const fortmatic = new FortmaticConnector({
  apiKey: "pk_live_BE64CE1BB4A49C37",
  chainId: 1
});

export const portis = new PortisConnector({
  dAppId: "104b9d07-25d3-4aeb-903b-ad7452218d05", // required
  networks: [1, 100]
});

export const squarelink = new SquarelinkConnector({
  clientId: "3904cdd1b675af615ca9",
  networks: [1, 100]
});

export const torus = new TorusConnector(
  { chainId: 1 },
  { showTorusButton: false }
);

// export const authereum = new AuthereumConnector({ chainId: 42 });

export default [
  {
    name: "MetaMask",
    connector: injected,
    img: require("./icons/metamask.svg"),
    connectionPhrase: "Connect to your MetaMask Wallet"
  },
  {
    name: "WalletConnect",
    connector: walletconnect,
    connectionPhrase: "Connect via WalletConnect",
    img: require("./icons/walletConnect.svg")
  },
  {
    name: "Torus",
    connector: torus,
    connectionPhrase: "Connect via a Torus",
    img: require("./icons/torus.svg")
  },
  {
    name: "Portis",
    connector: portis,
    connectionPhrase: "Connect via a Portis",
    img: require("./icons/portis.svg")
  },
  {
    name: "Fortmatic",
    connector: fortmatic,
    connectionPhrase: "Connect via a Fortmatic",
    img: require("./icons/fortmatic.svg")
  },
  {
    name: "Squarelink",
    connector: squarelink,
    connectionPhrase: "Connect via a Squarelink",
    img: require("./icons/squarelink.svg")
  }
];
