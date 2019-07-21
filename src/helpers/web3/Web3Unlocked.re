[@bs.module "./web3ProvideSwitcher"] external default: 'providerSwitcher = "";
let isUnlocked: unit => bool = () => default##providerIsUnlocked;
let isUsingProvider: unit => bool = () => default##providerInjected;
// let unlock: unit => bool = default##switchToInjectedWeb3;
