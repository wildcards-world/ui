type injectedType = {isAuthorized: unit => Promise.promise(bool)};

[@bs.module "./connectors"] external injected: injectedType = "injected";
[@bs.module "./connectors"] external network: int => injectedType = "network";
module Custom = {
  [@bs.module "./web3CustomRoot"] [@react.component]
  external make:
    (
      ~id: string,
      ~getLibrary: RootProviderTypes.rawProvider =>
                   RootProviderTypes.web3Library,
      ~children: React.element
    ) =>
    React.element =
    "default";
};
