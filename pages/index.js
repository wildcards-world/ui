import { withApollo } from "../apollo-lib/apollo";
import React from "react";
const MyComponent = require("../src/SsrEntryPoint.bs").make;

const IndexPage = () => {
  return <p>Some content - inside the page...</p>;
};

// export default withApollo({ ssr: true })(IndexPage);
export default withApollo({ ssr: true })(MyComponent);
