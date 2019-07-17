module DrizzleProvider = {
  [@bs.module "./DrizzleProvider"] [@react.component]
  external make: (~children: React.element) => React.element = "default";
};

ReactDOMRe.renderToElementWithId(
  <DrizzleProvider> <Layout /> </DrizzleProvider>,
  "root",
);
