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

[@bs.val] external mainnetApi: string = "process.env.REACT_APP_MAINNET_BE";
[@bs.val] external goerliApi: string = "process.env.REACT_APP_GOERLI_BE";

// TODO: SSR doesn't work correctly here, need to use the external apollo client
[@react.component]
let make = () =>
  <WildcardsProvider
    getGraphEndpoints={(networkId, ()) => {
      switch (networkId) {
      | 5 => (
          goerliApi,
          "wss://api.thegraph.com/subgraphs/name/wildcards-world/wildcards",
        )
      | _ => (
          mainnetApi,
          "wss://api.thegraph.com/subgraphs/name/wildcards-world/wildcards-goerli",
        )
      }
    }}>
    <Router />
  </WildcardsProvider>;
