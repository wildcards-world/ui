module UsdPriceProvider = {
  [@bs.module "./UsdPriceProvider"] [@react.component]
  external make: (~children: React.element) => React.element = "default";
  [@bs.module "./UsdPriceProvider"]
  external useUsdPrice: unit => option(float) = "useUsdPrice";
  [@bs.module "./UsdPriceProvider"]
  external useUsdPrice: unit => option(float) = "useUsdPrice";
};

type connector;

module RootProvider = {
  [@bs.module "./RootProvider"] [@react.component]
  external make: (~children: React.element) => React.element = "default";

  [@bs.module "./RootProvider"]
  external useAppStatus: unit => 'a = "useAppStatus";
  let useIsProviderSelected: unit => bool =
    () =>
      switch (useAppStatus()##chainId) {
      | Some(_) => true
      | None => false
      };
  // let useSetProvider: (unit, Web3.provider) => unit =
  //   () => useAppStatus()##setProvider;
  let useCurrentUser: unit => option(string) = () => useAppStatus()##account;
  let useIsActive: unit => bool = () => useAppStatus()##active;
  let useLogin: unit => bool = () => useAppStatus()##login;
  let useSetLogin: (unit, bool) => unit = () => useAppStatus()##setLogin;
  let useBuyView: unit => bool = () => useAppStatus()##buyView;
  let useSetBuyView: (unit, bool) => unit = () => useAppStatus()##setBuyView;
  let useEthBalance: unit => option(BN.bn) = () => useAppStatus()##ethBalance;
  let useActivateConnector: (unit, connector) => unit =
    () => {
      let appStatus = useAppStatus();
      connector => {
        let () = appStatus##activate(connector);
        appStatus##setLogin(false);
      };
    };
};
