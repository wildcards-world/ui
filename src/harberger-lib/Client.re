type dataObject = {
  .
  "__typename": string,
  "id": string,
};
// createInMemoryCache(~dataIdFromObject=(obj: dataObject) => obj##id, ());

/* Create an InMemoryCache */
let inMemoryCache = () => ApolloInMemoryCache.createInMemoryCache();

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

type context =
  | Neither
  | MaticQuery
  | MainnetQuery;
let chainContextToStr = chain =>
  switch (chain) {
  | Neither => "neither"
  | MaticQuery => "matic"
  | MainnetQuery => "mainnet"
  };
type queryContext = {context};

[@bs.send]
external getContext: ReasonApolloTypes.splitTest => option(queryContext) =
  "getContext";

/* based on test, execute left or right */
let webSocketHttpLink = (~uri, ~matic, ~subscriptions) =>
  ApolloLinks.split(
    operation => {
      let operationDefition =
        ApolloUtilities.getMainDefinition(operation.query);
      operationDefition.kind == "OperationDefinition"
      && operationDefition.operation == "subscription";
    },
    wsLink(~uri=subscriptions),
    ApolloLinks.split(
      operation => {
        let context = operation->getContext;

        let usingMatic =
          switch (context) {
          | Some({context}) =>
            switch (context) {
            | MaticQuery => true
            | Neither => false
            | MainnetQuery => false
            }
          | None => false
          };
        usingMatic;
      },
      httpLink(~uri=matic),
      httpLink(~uri),
    ),
  );

type qlEndpoints = {
  mainnet: string,
  matic: string,
  ws: string,
};

let instance = (~getGraphEndpoints: unit => qlEndpoints) => {
  let {mainnet, matic, ws} = getGraphEndpoints();

  ReasonApollo.createApolloClient(
    ~link=webSocketHttpLink(~uri=mainnet, ~matic, ~subscriptions=ws),
    ~cache=inMemoryCache(),
    (),
  );
};
