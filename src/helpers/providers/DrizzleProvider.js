import React, { createContext, useContext, useState, useEffect } from 'react';
import { drizzleReactHooks } from "@drizzle/react-plugin"
import { Drizzle, generateStore } from "@drizzle/store"
import { theme as rimbleTheme } from 'rimble-ui'
import { ThemeProvider } from 'styled-components'
import VitalikSteward from "../../contracts/VitalikSteward.json"
import ERC721Full from "../../contracts/ERC721Full.json"
import WildcardSteward_v0 from "../../contracts/WildcardSteward_v0.json"
import ERC721Patronage_v0 from "../../contracts/ERC721Patronage_v0.json"
import web3ProvideSwitcher from "../web3/web3ProvideSwitcher"

// todo: read env var for fallback
const fallbackUrl = "wss://mainnet.infura.io/ws/v3/a5d64a2052ab4d1da240cdfe3a6c519b";
// const fallbackUrl = "wss://goerli.infura.io/ws/v3/a5d64a2052ab4d1da240cdfe3a6c519b";
const switchableWeb3 = web3ProvideSwitcher.createSwitchableWeb3()

const options = {
  web3: {
    block: false,
    fallback: {
      type: "ws",
      url: fallbackUrl,
    },
    customProvider: switchableWeb3,
    // Repeating this because there seems to be some kind of bug somewhere where web3.web3 is used instead
    web3: {
      block: false,
      fallback: {
        type: "ws",
        url: fallbackUrl,
      },
      customProvider: switchableWeb3,
    },
  },
  contracts: [
    VitalikSteward,
    ERC721Full,
    // WildcardSteward_v0,
    // ERC721Patronage_v0,
  ],
  syncAlways: true,
  polls: {
    accounts: 1500,
  },
};

const ProviderContext = createContext('');
const theme = {
  ...rimbleTheme,
  colors: {
    ...rimbleTheme.colors,
    primary: 'rgb(107, 173, 62)', // This is the primary green used by wildcards theme    
  },
  buttons: {
    primary: {
      borderRadius: 0
    }
  }
}

const drizzleStore = generateStore(options)
const drizzle = new Drizzle(options, drizzleStore)

export default ({ children }) => {
  const [isProviderSelected, setIsProviderSelected] = useState(false)
  const setProvider = (provider) => {
    console.log("web3 provider is set!!")
    web3ProvideSwitcher.setExternalProvider(provider)
    setIsProviderSelected(true)
  }

  const resetApp = () => {
    // TODO
    console.log('reset the app')
  }

  const providedValues = { isProviderSelected, setProvider }

  return (
    <drizzleReactHooks.DrizzleProvider drizzle={drizzle}>
      <ProviderContext.Provider value={providedValues}>
        <ThemeProvider theme={theme}>
          {children}
        </ThemeProvider>
      </ProviderContext.Provider>
    </drizzleReactHooks.DrizzleProvider>
  )
}

export const useWeb3Setup = () => useContext(ProviderContext);

