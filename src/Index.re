open Providers;

module Router = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();
    switch (url.path) {
    | ["new-data"] => <TestQlUi />
    | [_] => <p> {React.string("Unknown page")} </p>
    | [] => <Layout />
    };
  };
};
ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=Client.instance>
    <DrizzleProvider> <Router /> </DrizzleProvider>
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
