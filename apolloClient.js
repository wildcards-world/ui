// import { ApolloClient } from 'apollo-client'
// import { InMemoryCache } from "apollo-cache-inmemory";
// import { HttpLink } from 'apollo-link-http'
import fetch from "isomorphic-unfetch";
// import { getClientInstance } from "./src/SsrEasyAccess.bs";

let ws = require("isomorphic-ws");

/*
type dataObject = {
  .
  "__typename": string,
  "id": string,
};
let fetch = [%raw "require('isomorphic-unfetch')"];
// createInMemoryCache(~dataIdFromObject=(obj: dataObject) => obj##id, ());

let inMemoryCache = () =>
  ApolloInMemoryCache.createInMemoryCache(
    // ~dataIdFromObject=
    //   (obj: dataObject) => {
    //     obj##id ++ obj##__typename;
    //   },
    ~cacheRedirects=[%raw
      "{
    Query: {
      global: (_, args, { getCacheKey }) => {
        let result = getCacheKey({ __typename: 'Global', id: args.id })
        // console.log(\"the result within\", result)
        return result
      },
      wildcard: (_, args, { getCacheKey }) => {
        return getCacheKey({ __typename: 'Wildcard', id: args.id })
      },
    },
    // Subscription: {
    //   global: (_, args, { getCacheKey }) => {
    //     let result = getCacheKey({ __typename: 'Global', id: args.id })
    //     console.log(\"the result within\", result)
    //     return result
    //   },
    //   wildcard: (_, args, { getCacheKey }) => {
    //     return getCacheKey({ __typename: 'Wildcard', id: args.id })
    //   },
    // },
  }"
    ],
    (),
  );

let httpLink = networkId =>
  ApolloLinks.createHttpLink(
    ~fetch,
    ~uri=
      switch (networkId) {
      | 5 => "https://api.thegraph.com/subgraphs/name/wild-cards/wildcards-goerli"
      | _ => "https://api.thegraph.com/subgraphs/name/wild-cards/wildcards"
      },
    (),
  );
let wsLink = networkId =>
  ApolloLinks.webSocketLink({
    uri:
      switch (networkId) {
      | 5 => "wss://api.thegraph.com/subgraphs/name/wild-cards/wildcards-goerli"
      | _ => "wss://api.thegraph.com/subgraphs/name/wild-cards/wildcards"
      },
    options: {
      reconnect: true,
      connectionParams: None,
    },
    webSocketImpl: ws,
  });

let webSocketHttpLink = networkId =>
  ApolloLinks.split(
    operation => {
      let operationDefition =
        ApolloUtilities.getMainDefinition(operation.query);
      operationDefition.kind == "OperationDefinition"
      && operationDefition.operation == "subscription";
    },
    wsLink(networkId),
    httpLink(networkId),
  );

let instance = (~link, ~ssrMode, ~cache) =>
  ReasonApollo.createApolloClient(~link, ~cache, ~ssrMode, ());
*/

// const fetch = require("node-fetch");
const ApolloClient = require("apollo-client").default;
// Setup the network "links"
const { WebSocketLink } = require("apollo-link-ws");
const { HttpLink } = require("apollo-link-http");
// const ws = require("ws");
const { split } = require("apollo-link");
const { getMainDefinition } = require("apollo-utilities");
const { InMemoryCache } = require("apollo-cache-inmemory");
// const dotenv = require("dotenv");

// dotenv.config();

const connection_string =
  "api.thegraph.com/subgraphs/name/wild-cards/wildcards-goerli";

const httpLink = new HttpLink({
  uri: "https://" + connection_string, // use https for secure endpoint
  fetch: fetch,
});

// Create a WebSocket link:
const wsLink = new WebSocketLink({
  uri: "wss://" + connection_string, // use wss for a secure endpoint
  options: {
    reconnect: true,
  },
  webSocketImpl: ws,
});

// using the ability to split links, you can send data to each link
// depending on what kind of operation is being sent
const link = split(
  // split based on operation type
  ({ query }) => {
    const { kind, operation } = getMainDefinition(query);
    return kind === "OperationDefinition" && operation === "subscription";
  },
  wsLink,
  httpLink
);

// // Instantiate client
// const client = new ApolloClient({
//   link,
//   cache: new InMemoryCache(),
// });

export default function createApolloClient(initialState, ctx) {
  // The `ctx` (NextPageContext) will only be present on the server.
  // use it to extract auth headers (ctx.req) or similar.
  return new ApolloClient({
    ssrMode: Boolean(ctx),
    link,
    cache: new InMemoryCache().restore(initialState),
  });
  // return getClientInstance(
  //   new InMemoryCache().restore(initialState),
  //   Boolean(ctx)
  // );
}
