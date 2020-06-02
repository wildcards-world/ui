// [%bs.raw {|require("./custom.css")|}];

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

module ApolloProvider = {
  [@react.component]
  let make = (~children, ~client) => {
    <ReasonApollo.Provider client>
      <ReasonApolloHooks.ApolloHooks.Provider client>
        children
      </ReasonApolloHooks.ApolloHooks.Provider>
    </ReasonApollo.Provider>;
  };
};
// TODO: SSR doesn't work correctly here, need to use the external apollo client
[@react.component]
let make = () =>
  <WildcardsProvider
    getGraphEndpoint={() => {
      let networkId =
        RootProvider.useNetworkId()->Belt.Option.mapWithDefault(1, a => a);
      switch (networkId) {
      | 5 => "goerli.api.wildcards.world/v1/graphql"
      | _ => "api.wildcards.world/v1/graphql"
      };
    }}>
    <Router />
  </WildcardsProvider>;
