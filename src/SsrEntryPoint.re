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
[@react.component]
let make = () =>
  <RootProvider> <QlStateManager> <Router /> </QlStateManager> </RootProvider>;
