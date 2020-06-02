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
let httpLink = (~uri) => ApolloLinks.createHttpLink(~uri, ());
/* Create an WS Link */
let wsLink = (~uri) =>
  ApolloLinks.webSocketLink({
    uri,
    options: {
      reconnect: true,
      connectionParams: None,
    },
  });

/* based on test, execute left or right */
let webSocketHttpLink = (~uribase) =>
  ApolloLinks.split(
    operation => {
      let operationDefition =
        ApolloUtilities.getMainDefinition(operation.query);
      operationDefition.kind == "OperationDefinition"
      && operationDefition.operation == "subscription";
    },
    wsLink(~uri="wss://" ++ uribase),
    httpLink(~uri="https://" ++ uribase),
  );

let instance = (~getGraphEndpoint) => {
  let graphEndpoint = getGraphEndpoint();

  ReasonApollo.createApolloClient(
    ~link=webSocketHttpLink(~uribase=graphEndpoint),
    ~cache=inMemoryCache(),
    (),
  );
};
