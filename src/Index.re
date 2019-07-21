open Providers;

module Router = {
  [@react.component]
  let make = () => {
    let url = ReasonReactRouter.useUrl();
    Js.log(url);
    switch (url.hash) {
    | "/dev-only/hidden/demo" =>
      <Offline requireSmartContractsLoaded=true> <Demo /> </Offline>
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
| _ => Js.log("We are not hot")
};
