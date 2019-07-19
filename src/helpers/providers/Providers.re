module UsdPriceProvider = {
  [@bs.module "./UsdPriceProvider"] [@react.component]
  external make: (~children: React.element) => React.element = "default";
  [@bs.module "./UsdPriceProvider"]
  external useUsdPrice: unit => option(float) = "useUsdPrice";
  [@bs.module "./UsdPriceProvider"]
  external useUsdPrice: unit => option(float) = "useUsdPrice";
};

module DrizzleProvider = {
  [@bs.module "./DrizzleProvider"] [@react.component]
  external make: (~children: React.element) => React.element = "default";

  [@bs.module "./DrizzleProvider"]
  external useUnlockAndCheck: unit => 'a = "useUnlockAndCheck";

  let useIsUnlocked = () => useUnlockAndCheck()##isWeb3Unlocked;
  let useUnlockWeb3IfNotAlready = () =>
    useUnlockAndCheck()##unlockWeb3IfNotAlready;
};
