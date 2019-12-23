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
  external useAppStatus: unit => 'a = "useAppStatus";
  let useIsProviderSelected: unit => bool =
    () => useAppStatus()##isProviderSelected;
  let useSetProvider: (unit, Web3.provider) => unit =
    () => useAppStatus()##setProvider;
  let useCurrentUser: unit => option(string) = () => useAppStatus()##account;
};
