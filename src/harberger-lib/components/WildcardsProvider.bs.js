// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Curry from "bs-platform/lib/es6/curry.js";
import * as React from "react";
import * as Belt_Option from "bs-platform/lib/es6/belt_Option.js";
import * as ReactApollo from "react-apollo";
import * as Client$WildCards from "../Client.bs.js";
import * as ReactHooks from "@apollo/react-hooks";
import * as RootProvider$WildCards from "../RootProvider.bs.js";

function WildcardsProvider$GraphQl(Props) {
  var getGraphEndpoints = Props.getGraphEndpoints;
  var children = Props.children;
  var networkId = Belt_Option.mapWithDefault(RootProvider$WildCards.useNetworkId(undefined), 1, (function (a) {
          return a;
        }));
  var client = React.useMemo((function () {
          return Client$WildCards.instance(Curry._1(getGraphEndpoints, networkId));
        }), [
        getGraphEndpoints,
        networkId
      ]);
  return React.createElement(ReactApollo.ApolloProvider, {
              client: client,
              children: React.createElement(ReactHooks.ApolloProvider, {
                    client: client,
                    children: children
                  })
            });
}

var GraphQl = {
  make: WildcardsProvider$GraphQl
};

function WildcardsProvider(Props) {
  var getGraphEndpoints = Props.getGraphEndpoints;
  var children = Props.children;
  var stewardContractAddress = Props.stewardContractAddress;
  var stewardAbi = Props.stewardAbi;
  return React.createElement(RootProvider$WildCards.make, {
              children: React.createElement(WildcardsProvider$GraphQl, {
                    getGraphEndpoints: getGraphEndpoints,
                    children: children
                  }),
              stewardContractAddress: stewardContractAddress,
              stewardAbi: stewardAbi
            });
}

var make = WildcardsProvider;

export {
  GraphQl ,
  make ,
  
}
/* react Not a pure module */