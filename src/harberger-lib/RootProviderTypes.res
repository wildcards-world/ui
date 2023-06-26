type injectedType = {@dead("injectedType.isAuthorized") isAuthorized: unit => Promise.promise<bool>}
type web3reactContext = {
  @dead("web3reactContext.active") active: bool,
  @dead("web3reactContext.activate")
  activate: (injectedType, unit => unit, bool) => Promise.promise<unit>,
  @dead("web3reactContext.account") account: option<Web3.ethAddress>,
  @dead("web3reactContext.library") library: option<Web3.web3Library>,
  @dead("web3reactContext.chainId") chainId: option<int>,
}

type rec rootActions =
  | NoAction
  | GoToBuy(TokenId.t)
  | GoToAuction(TokenId.t)
  | GoToDepositUpdate
  | GoToPriceUpdate(TokenId.t)
  | GoToUserVerification
  | ClearNonUrlState
  | GoToWeb3Connect(rootActions)
  | Logout
  | LoadAddress(Web3.ethAddress, option<Eth.t>)
type nonUrlState =
  | LoginScreen(rootActions)
  | UserVerificationScreen
  | UpdateDepositScreen
  | UpdatePriceScreen(TokenId.t)
  | BuyScreen(TokenId.t)
  | AuctionScreen(TokenId.t)
  | NoExtraState
type ethState =
  | Disconnected
  | Connected(Web3.ethAddress, option<Eth.t>)

type config = {
  stewardContractAddress: option<Web3.ethAddress>,
  stewardAbi: option<Web3.abi>,
}
type state = {
  nonUrlState: nonUrlState,
  ethState: ethState,
  config: config,
}
