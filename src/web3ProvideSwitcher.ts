import Web3 from "web3"

const getInjectedWeb3 = async (): Promise<{ provider: any, providerInjected: boolean }> => {

  if ((<any>window).ethereum) {
    const { ethereum } = (<any>window)
    try {
      await ethereum.enable()
      return { provider: ethereum, providerInjected: true }
    } catch (error) {
      // User denied account access...
      console.log(error)
    }
  } else if (typeof (<any>window).web3 !== 'undefined') {
    console.log('we have the window web3')
    // Checking if Web3 has been injected by the browser (Mist/MetaMask)
    // Use Mist/MetaMask's provider.
    const { currentProvider } = new Web3((<any>window).web3.currentProvider)

    return { provider: currentProvider, providerInjected: true }
  }
  try {
    // Web3 in drizzle seems to have a bug with websockts, using http-provider so long
    var provider = new Web3.providers.WebsocketProvider(
      "wss://mainnet.infura.io/ws/v3/e811479f4c414e219e7673b6671c2aba"
    )
    return { provider, providerInjected: false }
  } catch (error) {
    // Out of web3 options; throw.
    throw new Error('Cannot find injected web3 or valid fallback.')
  }
}
const getInjectedWeb3NoLoad = (): { provider: any, providerInjected: boolean } => {

  if ((<any>window).ethereum) {
    const { ethereum } = (<any>window)
    try {
      return { provider: ethereum, providerInjected: true }
    } catch (error) {
      // User denied account access...
      console.log(error)
    }
  } else if (typeof (<any>window).web3 !== 'undefined') {
    console.log('we have the window web3')
    // Checking if Web3 has been injected by the browser (Mist/MetaMask)
    // Use Mist/MetaMask's provider.
    const { currentProvider } = new Web3((<any>window).web3.currentProvider)

    return { provider: currentProvider, providerInjected: true }
  }
  try {
    // Web3 in drizzle seems to have a bug with websockts, using http-provider so long
    const provider = new Web3.providers.WebsocketProvider(
      "wss://mainnet.infura.io/ws/v3/e811479f4c414e219e7673b6671c2aba"
    )
    return { provider, providerInjected: false }
  } catch (error) {
    // Out of web3 options; throw.
    throw new Error('Cannot find injected web3 or valid fallback.')
  }
}
class Web3ProviderSwitcher {

  public static instance: Web3ProviderSwitcher
  public injectedWeb3: any = null
  public defaultWeb3: any
  public providerInjected: boolean = false
  public providerIsUnlocked: boolean = false

  constructor() {
    if (Web3ProviderSwitcher.instance) { return Web3ProviderSwitcher.instance }
    Web3ProviderSwitcher.instance = this

    const { provider, providerInjected } = getInjectedWeb3NoLoad()
    this.providerInjected = providerInjected
    this.defaultWeb3 = provider
  }


  public async switchToInjectedWeb3() {
    if (this.providerIsUnlocked) return true

    try {
      const { provider: web3, providerInjected } = await getInjectedWeb3()
      this.injectedWeb3 = web3
      this.providerInjected = providerInjected
      this.providerIsUnlocked = true
    } catch (e) {
      console.log(e)
    }

    return this.providerIsUnlocked
  }

  public createSwitchableWeb3() {
    const web3RequestHandler = {
      get: (obj: any, prop: string) =>
        this.providerIsUnlocked ? this.injectedWeb3[prop] : this.defaultWeb3[prop]
    }

    return new Proxy({}, web3RequestHandler)
  }
}

export default new Web3ProviderSwitcher()
