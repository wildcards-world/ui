module UsdPriceProvider = {
  [@bs.module "./UsdPriceProvider"] [@react.component]
  external make: (~children: React.element) => React.element = "default";

  [@bs.module "./UsdPriceProvider"]
  external useUsdPrice: unit => option(float) = "useUsdPrice";
};
