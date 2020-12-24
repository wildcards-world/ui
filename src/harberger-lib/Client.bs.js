// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Curry from "bs-platform/lib/es6/curry.js";
import * as ApolloLinks from "@wildcards/reason-apollo/src/ApolloLinks.bs.js";
import * as ApolloLink from "apollo-link";
import * as ReasonApollo from "@wildcards/reason-apollo/src/ReasonApollo.bs.js";
import * as ApolloLinkWs from "apollo-link-ws";
import * as ApolloUtilities from "apollo-utilities";
import * as ApolloInMemoryCache from "@wildcards/reason-apollo/src/ApolloInMemoryCache.bs.js";

function inMemoryCache(param) {
  return ApolloInMemoryCache.createInMemoryCache(undefined, undefined, undefined, undefined, undefined);
}

function httpLink(uri) {
  return ApolloLinks.createHttpLink(uri, undefined, undefined, undefined, undefined, undefined, undefined);
}

function wsLink(uri) {
  return new ApolloLinkWs.WebSocketLink({
              uri: uri,
              options: {
                reconnect: true,
                connectionParams: undefined
              }
            });
}

function chainContextToStr(chain) {
  switch (chain) {
    case /* Neither */0 :
        return "neither";
    case /* MaticQuery */1 :
        return "matic";
    case /* MainnetQuery */2 :
        return "mainnet";
    
  }
}

function webSocketHttpLink(uri, matic, subscriptions) {
  return ApolloLink.split((function (operation) {
                var operationDefition = ApolloUtilities.getMainDefinition(operation.query);
                if (operationDefition.kind === "OperationDefinition") {
                  return operationDefition.operation === "subscription";
                } else {
                  return false;
                }
              }), wsLink(subscriptions), ApolloLink.split((function (operation) {
                    var context = operation.getContext();
                    if (context !== undefined) {
                      return context.context === 1;
                    } else {
                      return false;
                    }
                  }), httpLink(matic), httpLink(uri)));
}

function instance(getGraphEndpoints) {
  var match = Curry._1(getGraphEndpoints, undefined);
  return ReasonApollo.createApolloClient(webSocketHttpLink(match.mainnet, match.matic, match.ws), inMemoryCache(undefined), undefined, undefined, undefined, undefined, undefined);
}

export {
  inMemoryCache ,
  httpLink ,
  wsLink ,
  chainContextToStr ,
  webSocketHttpLink ,
  instance ,
  
}
/* ApolloLinks Not a pure module */