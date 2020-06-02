[%bs.raw {|require("react-tabs/style/react-tabs.css")|}];

module Router = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();
    switch (url.path) {
    // | ["new-data"] => <QlHooks />
    | [_] => <p> {React.string("Unknown page")} </p>
    | _ => <Layout />
    };
  };
};

ReactDOMRe.renderToElementWithId(
  // stewardAbi
  // stewardContractAddress="0x4bE0Eab8f41c8109AA134509086Cbcb18b10C0fB"
  <WildcardsProvider
    getGraphEndpoint={() => {
      let networkId =
        RootProvider.useNetworkId()->Belt.Option.mapWithDefault(1, a => a);
      switch (networkId) {
      | 5 => "goerli.api.wildcards.world/v1/graphql"
      | _ => "api.wildcards.world/v1/graphql"
      };
    }}>
    <UsdPriceProvider> <Router /> </UsdPriceProvider>
  </WildcardsProvider>,
  "root",
);

/*
 module ReasonApolloProvderNetworkSwitcher = {
   [@react.component]
   let make = (~children) => {
     let networkId =
       RootProvider.useNetworkId()->Belt.Option.mapWithDefault(1, a => a);

     let client =
       React.useMemo1(() => Client.instance(networkId), [|networkId|]);

     <ReasonApollo.Provider key={networkId->string_of_int} client>
       <ReasonApolloHooks.ApolloHooks.Provider
         key={networkId->string_of_int} client>
         children
       </ReasonApolloHooks.ApolloHooks.Provider>
     </ReasonApollo.Provider>;
   };
 };

 ReactDOMRe.renderToElementWithId(
   <RootProvider>
     <ReasonApolloProvderNetworkSwitcher>
       <QlStateManager> <Router /> </QlStateManager>
     </ReasonApolloProvderNetworkSwitcher>
   </RootProvider>,
   "root",
 );
 */
// type parcelModule;
// type hot;
// [@bs.val] external parcelModule: parcelModule = "module";
// [@bs.get] external hot: parcelModule => Js.nullable(hot) = "hot";
// [@bs.send.pipe: hot] external accept: unit => unit = "accept";

// switch (Js.Nullable.toOption(parcelModule |> hot)) {
// | Some(h) =>
//   h |> accept();
//   %bs.raw
//   {|window.isDevelopMode = true|};
// | _ => Js.log("Production website.")
// };
