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
  external useWeb3Setup: unit => 'a = "useWeb3Setup";
  let useIsProviderSelected: unit => bool =
    () => useWeb3Setup()##isProviderSelected;
  let useSetProvider: (unit, Web3.provider) => unit =
    () => useWeb3Setup()##setProvider;
};
