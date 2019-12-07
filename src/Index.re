open Providers;

[%bs.raw {|require("./custom.css")|}];

// module Test = {
//   [@react.component]
//   let make = () => {
//     let gContext = React.useContext(GlobalContext.context);
//     switch (gContext) {
//     | Loading => <p> "loading - from internet"->React.string </p>
//     | Error(_error) => <p> "error"->React.string </p>
//     | Loaded(isLoaded) => <p> isLoaded->React.string </p>
//     };
//   };
// };

module Router = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();
    switch (url.path) {
    | ["new-data"] => <QlHooks />
    | [_] => <p> {React.string("Unknown page")} </p>
    | _ => <Layout />
    };
  };
};

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=Client.instance>
    <ReasonApolloHooks.ApolloProvider client=Client.instance>
      <DrizzleProvider> <Router /> </DrizzleProvider>
    </ReasonApolloHooks.ApolloProvider>
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
