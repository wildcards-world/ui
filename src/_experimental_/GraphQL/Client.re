/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();
/* Create an HTTP Link */
// let httpLink =
//   ApolloLinks.createHttpLink(
//     ~uri="https://api.thegraph.com/subgraphs/name/jasoons/wildcards-goerli",
//     (),
//   );
/* Create an WS Link */
let wsLink =
  ApolloLinks.webSocketLink(
    ~uri="wss://api.thegraph.com/subgraphs/name/jasoons/wildcards-goerli",
    (),
  );
let instance =
  ReasonApollo.createApolloClient(~link=wsLink, ~cache=inMemoryCache, ());
