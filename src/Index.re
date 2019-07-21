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
