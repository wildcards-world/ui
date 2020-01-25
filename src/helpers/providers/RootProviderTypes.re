type rawProvider;
type ethersBigNumber = {toString: (. unit) => string};
type web3Library = {
  getBalance:
    (. Web3.ethAddress) => Promise.promise(option(ethersBigNumber)),
  getSigner: (. Web3.ethAddress) => web3Library,
  // pollingInterval: ref(int),
  provider: rawProvider,
};
type injectedType = {isAuthorized: unit => Promise.promise(bool)};
type web3reactContext = {
  active: bool,
  activate: (injectedType, unit => unit, bool) => Promise.promise(unit),
  account: option(Web3.ethAddress),
  library: option(web3Library),
  chainId: option(int),
};

type rootActions =
  | GoToBuy(Animal.t)
  | GoToDepositUpdate(Animal.t)
  | GoToPriceUpdate(Animal.t)
  | GoToUserVerification
  | ClearNonUrlState
  | GoToWeb3Connect
  | Logout
  | LoadAddress(Web3.ethAddress, option(Eth.t));
type nonUrlState =
  | LoginScreen(rootActions)
  | UserVerificationScreen
  | UpdateDepositScreen(Animal.t)
  | UpdatePriceScreen(Animal.t)
  | BuyScreen(Animal.t)
  | NoExtraState;
type ethState =
  | Disconnected
  | Connected(Web3.ethAddress, option(Eth.t));

type state = {
  nonUrlState,
  ethState,
};
