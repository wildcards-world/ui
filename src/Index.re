[%bs.raw {|require("react-tabs/style/react-tabs.css")|}];

open Globals;

module Router = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();
    switch (url.path) {
    | [_] => <p> {React.string("Unknown page")} </p>
    | _ => <ReactTranslate> <Layout /> </ReactTranslate>
    };
  };
};

[@bs.val]
external mainnetApi: option(string) = "process.env.REACT_APP_MAINNET_BE";
[@bs.val]
external goerliApi: option(string) = "process.env.REACT_APP_GOERLI_BE";

ReactDOMRe.renderToElementWithId(
  <WildcardsProvider
    getGraphEndpoints={(networkId, ()) => {
      switch (networkId) {
      | 5 => (
          goerliApi |||| "https://api.wildcards.world/v1/graphq",
          "wss://api.thegraph.com/subgraphs/name/wildcards-world/wildcards",
        )
      | _ => (
          mainnetApi |||| "https://goerli.api.wildcards.world/v1/graphql",
          "wss://api.thegraph.com/subgraphs/name/wildcards-world/wildcards-goerli",
        )
      }
    }}>
    <UsdPriceProvider> <Router /> </UsdPriceProvider>
  </WildcardsProvider>,
  "root",
);
