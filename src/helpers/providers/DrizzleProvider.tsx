import React, { createContext, useContext, useState, useEffect } from 'react';
import { drizzleReactHooks } from "drizzle-react"
import { Drizzle, generateStore } from "drizzle"

import VitalikSteward from "../../contracts/VitalikSteward.json"
import ERC721Full from "../../contracts/ERC721Full.json"
import web3ProvideSwitcher from "../web3/web3ProvideSwitcher"
import { theme as rimbleTheme } from 'rimble-ui'
import { ThemeProvider } from 'styled-components'

// todo: read env var for fallback
const fallbackUrl = "wss://mainnet.infura.io/ws/v3/e811479f4c414e219e7673b6671c2aba"
const switchableWeb3 = web3ProvideSwitcher.createSwitchableWeb3()
const ProviderContext = createContext<any>('');

interface Options {
  web3: any
  contracts: any[]
  syncAlways: boolean
  polls: any
}
const options: Options = {
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
    ERC721Full
  ],
  syncAlways: true,
  polls: {
    accounts: 1500,
  },
}

const theme = {
  ...rimbleTheme,
  colors: {
    ...rimbleTheme.colors,
    primary: 'rgb(107, 173, 62)', // This is the primary green used by wildcards theme
  }
}

const drizzleStore = generateStore(options)
const drizzle = new Drizzle(options, drizzleStore)

export default ({ children }: any) => {
  const [isWeb3Unlocked, setIsWeb3Unlocked] = useState(false);

  const unlockWeb3IfNotAlready = () => {
    if (isWeb3Unlocked) return

    web3ProvideSwitcher.unlockWeb3WithCallback(setIsWeb3Unlocked)
  }

  return (
    <drizzleReactHooks.DrizzleProvider drizzle={drizzle}>
      <ProviderContext.Provider value={{ isWeb3Unlocked, unlockWeb3IfNotAlready }}>
        <ThemeProvider theme={theme}>
          {children}
        </ThemeProvider>
      </ProviderContext.Provider>
    </drizzleReactHooks.DrizzleProvider>
  )
}

export const useUnlockAndCheck = () => useContext(ProviderContext);
