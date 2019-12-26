import React, { createContext, useContext, useState, useEffect } from "react";
import { theme as rimbleTheme } from "rimble-ui";
import { ThemeProvider } from "styled-components";
import TranslationProvider from "./TranslationProvider";
import UserProvider from "./UserProvider";
import {
  Web3ReactProvider,
  useWeb3React,
  UnsupportedChainIdError
} from "@web3-react/core";
import {
  NoEthereumProviderError,
  UserRejectedRequestError as UserRejectedRequestErrorInjected
} from "@web3-react/injected-connector";
import {
  URI_AVAILABLE,
  UserRejectedRequestError as UserRejectedRequestErrorWalletConnect
} from "@web3-react/walletconnect-connector";
import { UserRejectedRequestError as UserRejectedRequestErrorFrame } from "@web3-react/frame-connector";
import { Web3Provider } from "@ethersproject/providers";
import { formatEther } from "@ethersproject/units";
// import {
//   injected,
//   network,
//   walletconnect,
//   walletlink,
//   ledger,
//   trezor,
//   frame,
//   fortmatic,
//   portis,
//   squarelink,
//   torus,
//   authereum
// } from "./web3-react/connectors";
import { useEagerConnect, useInactiveListener } from "./web3-react/hooks";
console.log(require("./web3-react/connectors"));

// const connectorsByName = {
//   Injected: injected,
//   Network: network,
//   WalletConnect: walletconnect,
//   WalletLink: walletlink,
//   Ledger: ledger,
//   Trezor: trezor,
//   Frame: frame,
//   Fortmatic: fortmatic,
//   Portis: portis,
//   Squarelink: squarelink,
//   Torus: torus,
//   Authereum: authereum
// };

function getErrorMessage(error) {
  if (error instanceof NoEthereumProviderError) {
    return "No Ethereum browser extension detected, install MetaMask on desktop or visit from a dApp browser on mobile.";
  } else if (error instanceof UnsupportedChainIdError) {
    return "You're connected to an unsupported network.";
  } else if (
    error instanceof UserRejectedRequestErrorInjected ||
    error instanceof UserRejectedRequestErrorWalletConnect ||
    error instanceof UserRejectedRequestErrorFrame
  ) {
    return "Please authorize this website to access your Ethereum account.";
  } else {
    console.error(error);
    return "An unknown error occurred. Check the console for more details.";
  }
}

function getLibrary(provider) {
  const library = new Web3Provider(provider);
  library.pollingInterval = 8000;
  return library;
}

const ProviderContext = createContext("");
const theme = {
  ...rimbleTheme,
  colors: {
    ...rimbleTheme.colors,
    primary: "rgb(107, 173, 62)" // This is the primary green used by wildcards theme
  },
  buttons: {
    primary: {
      borderRadius: 0
    }
  }
};

export const Web3Parent = ({ children }) => {
  // const [isProviderSelected, setIsProviderSelected] = useState(false);
  // const setProvider = provider => {
  //   console.log("This is not implemented!!");
  //   // web3ProvideSwitcher.setExternalProvider(provider);
  //   // setIsProviderSelected(true);
  // };

  // const resetApp = () => {
  //   // TODO
  //   console.log("reset the app");
  // };

  const context = useWeb3React();
  const {
    connector,
    library,
    chainId,
    account,
    activate,
    deactivate,
    active,
    error
  } = context;

  // handle logic to recognize the connector currently being activated
  const [activatingConnector, setActivatingConnector] = React.useState();
  React.useEffect(() => {
    if (activatingConnector && activatingConnector === connector) {
      setActivatingConnector(undefined);
    }
  }, [activatingConnector, connector]);

  // handle logic to eagerly connect to the injected ethereum provider, if it exists and has granted access already
  const triedEager = useEagerConnect();

  // handle logic to connect in reaction to certain events on the injected ethereum provider, if it exists
  useInactiveListener(!triedEager || !!activatingConnector);

  // fetch eth balance of the connected account
  const [ethBalance, setEthBalance] = React.useState();
  React.useEffect(() => {
    if (library && account) {
      let stale = false;

      library
        .getBalance(account)
        .then(balance => {
          if (!stale) {
            setEthBalance(balance);
          }
        })
        .catch(() => {
          if (!stale) {
            setEthBalance(null);
          }
        });

      return () => {
        stale = true;
        setEthBalance(undefined);
      };
    }
  }, [library, account, chainId]);

  // log the walletconnect URI
  // React.useEffect(() => {
  //   const logURI = uri => {
  //     console.log("WalletConnect URI", uri);
  //   };
  //   walletconnect.on(URI_AVAILABLE, logURI);

  //   return () => {
  //     walletconnect.off(URI_AVAILABLE, logURI);
  //   };
  // }, []);

  const [login, setLogin] = useState(false);
  const [buyView, setBuyView] = useState(false);

  const providedValues = {
    // isProviderSelected,
    // setProvider,
    account,
    ethBalance,
    chainId,
    active,
    login,
    setLogin,
    activate,
    buyView,
    setBuyView
  };
  console.log({ ethBalance: !!ethBalance ? ethBalance.toString() : "loading" });

  return (
    // <drizzleReactHooks.RootProvider drizzle={drizzle}>
    <ProviderContext.Provider value={providedValues}>
      <ThemeProvider theme={theme}>
        <UserProvider>
          <TranslationProvider>{children}</TranslationProvider>
        </UserProvider>
      </ThemeProvider>
    </ProviderContext.Provider>
    // </drizzleReactHooks.RootProvider>
  );
};
export default ({ children }) => {
  return (
    <Web3ReactProvider getLibrary={getLibrary}>
      <Web3Parent>{children}</Web3Parent> />
    </Web3ReactProvider>
  );
};

export const useAppStatus = () => useContext(ProviderContext);
