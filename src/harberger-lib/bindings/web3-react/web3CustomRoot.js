import { createWeb3ReactRoot } from "@web3-react/core";
import * as React from "react";

export default function ({ id, getLibrary, children }) {
  const Web3ReactProviderWithId = createWeb3ReactRoot(id);
  return (
    <div>
      <Web3ReactProviderWithId getLibrary={getLibrary}>
        {children}
      </Web3ReactProviderWithId>
    </div>
  );
}
