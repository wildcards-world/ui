type dataObject = {
  .
  "__typename": string,
  "id": string,
};
// createInMemoryCache(~dataIdFromObject=(obj: dataObject) => obj##id, ());

/* Create an InMemoryCache */
let inMemoryCache =
  ApolloInMemoryCache.createInMemoryCache(
    // ~dataIdFromObject=
    //   (obj: dataObject) => {
    //     Js.log("obj");
    //     Js.log(obj);
    //     Js.log(obj##id ++ obj##__typename);
    //     obj##id ++ obj##__typename;
    //   },
    ~cacheRedirects=[%raw
      "{
    Query: {
      global: (_, args, { getCacheKey }) => {
        let result = getCacheKey({ __typename: 'Global', id: args.id })
        console.log(\"the result within\", result)
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
    // Query: {
    //   global: (_, args, { getCacheKey }) => {
    //     let result = getCacheKey({ __typename: 'Global', id: args.id })
    //     console.log(\"the result within\", result)
    //     return result
    //   },
    //   //     books: (_, args, { getCacheKey }) =>
    //   // args.ids.map(id =>
    //   //   getCacheKey({ __typename: 'Book', id: id }))
    //   wildcards: (_, args, { getCacheKey }) => {
    //     let result = args.ids.map(id=>getCacheKey({ __typename: 'Global', id: args.id }))
    //     console.log(\"the result within\", result)
    //     return result
    //   }
    //   // global: (_, args, { getCacheKey }) => {
    //   //   getCacheKey({ __typename: 'Global', id: args.id })
    //   // }
    // },
  }"
    ],
    (),
  );

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(
    ~uri="https://api.thegraph.com/subgraphs/name/jasoons/wildcards",
    (),
  );
/* Create an WS Link */
let wsLink =
  ApolloLinks.webSocketLink(
    ~uri="wss://api.thegraph.com/subgraphs/name/jasoons/wildcards",
    (),
  );

/* based on test, execute left or right */
let webSocketHttpLink =
  ApolloLinks.split(
    operation => {
      let operationDefition =
        ApolloUtilities.getMainDefinition(operation##query);
      operationDefition##kind == "OperationDefinition"
      &&
      operationDefition##operation == "subscription";
    },
    wsLink,
    httpLink,
  );

let instance =
  ReasonApollo.createApolloClient(
    ~link=webSocketHttpLink,
    ~cache=inMemoryCache,
    (),
  );
