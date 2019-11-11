open Providers;

module Router = {
  [@react.component]
  let make = () => {
    <Layout />;
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
| Some(h) =>
  h |> accept();
  %bs.raw
  {|window.isDevelopMode = true|};
| _ => Js.log("Production website.")
};
