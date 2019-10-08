open Providers;
[%bs.raw {|require("./styles.css")|}];

module Router = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();
    switch (url.hash) {
    // | "/dev-only/hidden/demo" =>
    //   <Offline requireSmartContractsLoaded=true> <Demo /> </Offline>
    | _ => <Layout />
    };
  };
};
ReactDOMRe.renderToElementWithId(
  <DrizzleProvider> <Router /> </DrizzleProvider>,
  "root",
);

type parcelModule;
type hot;
[@bs.val] external parcelModule: parcelModule = "module";
[@bs.get] external hot: parcelModule => Js.nullable(hot) = "hot";
[@bs.send.pipe: hot] external accept: unit => unit = "accept";

switch (Js.Nullable.toOption(parcelModule |> hot)) {
| Some(h) => h |> accept()
| _ => Js.log("Production website.")
};
