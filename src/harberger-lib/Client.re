type dataObject = {
  .
  "__typename": string,
  "id": string,
};
open Globals;
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
        // Js.log2("let", operation->getContext);
        let context = operation->getContext;
        Js.log2(
          "Context",
          context->Option.mapWithDefault("NOT DEFINED", a => a->Obj.magic),
        );
        // Js.log(context.context);
        let usingMatic =
          switch (operation->getContext) {
          | Some({context}) =>
            switch (context) {
            | MaticQuery =>
              Js.log("it was MATIC!!");
              true;
            | Neither =>
              Js.log("it was neither");
              false;
            | MainnetQuery =>
              Js.log("it was MAINNET");
              false;
            }
          | None => false
          };
        Js.log2("USING MATIC", usingMatic);
        usingMatic;
      },
      // true;
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
