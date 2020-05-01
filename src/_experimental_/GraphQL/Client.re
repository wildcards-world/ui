type dataObject = {
  .
  "__typename": string,
  "id": string,
};
// createInMemoryCache(~dataIdFromObject=(obj: dataObject) => obj##id, ());

/* Create an InMemoryCache */
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

/* Create an HTTP Link */
let httpLink = networkId =>
  ApolloLinks.createHttpLink(
    ~uri=
      switch (networkId) {
      | 5 => "https://api.thegraph.com/subgraphs/name/wild-cards/wildcards-goerli"
      | _ => "https://api.thegraph.com/subgraphs/name/wild-cards/wildcards"
      },
    (),
  );
/* Create an WS Link */
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
  });

/* based on test, execute left or right */
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

let instance = networkId =>
  ReasonApollo.createApolloClient(
    ~link=webSocketHttpLink(networkId),
    ~cache=inMemoryCache(),
    (),
  );
