[%bs.raw {|require("./custom.css")|}];

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
  <ReasonApollo.Provider client=Client.instance>
    <ReasonApolloHooks.ApolloHooks.Provider client=Client.instance>
      <RootProviderNew> <Router /> </RootProviderNew>
    </ReasonApolloHooks.ApolloHooks.Provider>
  </ReasonApollo.Provider>,
  "root",
);

type parcelModule;
type hot;
[@bs.val] external parcelModule: parcelModule = "module";
[@bs.get] external hot: parcelModule => Js.nullable(hot) = "hot";
[@bs.send.pipe: hot] external accept: unit => unit = "accept";

switch (Js.Nullable.toOption(parcelModule |> hot)) {
| Some(h) =>
  h |> accept();
  %bs.raw
  {|window.isDevelopMode = true|};
| _ => Js.log("Production website.")
};
