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
        HomeAnimal:
        {
          wildcardData: (_, args, { getCacheKey }) => {
        let result = getCacheKey({ __typename: 'WildcardData', id: args.id })
        console.log('getting wildcard data', result)
        return result
      }
        },
        HomeAnimals:
        {
          wildcardData: (_, args, { getCacheKey }) => {
        let result = getCacheKey({ __typename: 'WildcardData', id: args.id })
        console.log('getting wildcard data', result)
        return result
      }
        },
        homeAnimal:
        {
          wildcardData: (_, args, { getCacheKey }) => {
        let result = getCacheKey({ __typename: 'WildcardData', id: args.id })
        console.log('2getting wildcard data', result)
        return result
      }
        },
        homeAnimals:
        {
          wildcardData: (_, args, { getCacheKey }) => {
        let result = getCacheKey({ __typename: 'WildcardData', id: args.id })
        console.log('2getting wildcard data', result)
        return result
      }
        },
        AnimalId:
        {
          wildcardData: (_, args, { getCacheKey }) => {
        let result = getCacheKey({ __typename: 'WildcardData', id: args.id })
        console.log('2getting wildcard data', result)
        return result
      }},
        AnimalIds:
        {
          wildcardData: (_, args, { getCacheKey }) => {
        let result = getCacheKey({ __typename: 'WildcardData', id: args.id })
        console.log('2getting wildcard data', result)
        return result
      }
        },
    Query: {
          wildcardDatas: (_, args, { getCacheKey }) => {
            console.log('datatttaaa');
        let result = getCacheKey({ __typename: 'WildcardData', id: args.id })
        console.log('2getting wildcard data', result)
        return result
      },
          wildcardData: (_, args, { getCacheKey }) => {
            console.log('datatttaaa');
        let result = getCacheKey({ __typename: 'WildcardData', id: args.id })
        console.log('2getting wildcard data', result)
        return result
      },
      global: (_, args, { getCacheKey }) => {
        let result = getCacheKey({ __typename: 'Global', id: args.id })
        return result
      },
      wildcard: (_, args, { getCacheKey }) => {
        const result = getCacheKey({ __typename: 'Wildcard', id: args.id})
        return result
      },
      // global: (_, args, { getCacheKey }) => {
      //   let result = getCacheKey({ __typename: 'Global', id: args.id + 'wc' })
      //   // console.log(\"the result within\", result)
      //   return result
      // },
      // wildcard: (_, args, { getCacheKey }) => {
      //   return getCacheKey({ __typename: 'Wildcard', id: args.id + 'wc' })
      // },
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
    httpLink(~uri="http://" ++ uribase),
  );

let instance = (~getGraphEndpoint) => {
  let graphEndpoint = getGraphEndpoint();

  ReasonApollo.createApolloClient(
    ~link=webSocketHttpLink(~uribase=graphEndpoint),
    ~cache=inMemoryCache(),
    (),
  );
};
