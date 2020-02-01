external cast: Js.Json.t => QlHooks.SubWildcardQuery.t = "%identity";

// NOTE: keeping this code here since it might be easier in the future to cast the json from the query into an object to work with (eg removing the Options etc)
/*
 type owner = {
   address: string,
   id: string,
 };
 type wildcard = {
   id: string,
   animal: string,
   owner,
   // patronageNumeratorPriceScaled:
   // price: { id: "0xdd7cdb3c31f4d98b98dd0f8c6f478cb960a6960a20516c8d03bd0cd6c8bf8662", price: "8000000000000000" }
   // timeAcquired: "1577699799"
   // timeCollected: "1577699799"
   // totalCollected: "493551596974884"
 };
 type buyEvent = {token: wildcard};
 type eventCounter = {buyEvents: array(buyEvent)};
 type globalQlState = {eventCounter};
 external unsafeCastToWildcardObject: Js.Json.t => wildcard = "%identity";
 */

type gqlState = {isLoaded: bool};

module RootContext = {
  let context = React.createContext(false);
  // Create a provider component
  let make = React.Context.provider(context);
  // Tell bucklescript how to translate props into JS
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
};

let useIsInitialized: unit => bool =
  () => React.useContext(RootContext.context);

[@react.component]
let make = (~children) => {
  let result = QlHooks.useBuySubscriptionData();
  // let lastEvent = React.useState(() => );

  let initialDataLoad = QlHooks.useInitialDataLoad();
  let hasLoadedInitialData =
    switch (initialDataLoad) {
    | Some(_) => true
    | None => false
    };

  <RootContext value=hasLoadedInitialData>
    {hasLoadedInitialData
       ? result->Belt.Option.mapWithDefault(React.null, result =>
           <ApolloConsumer>
             ...{client => {
               open ReasonApolloHooks.ApolloHooks;
               let latestBuyEvent =
                 result##eventCounter
                 ->Belt.Option.flatMap(eventCounter => eventCounter##buyEvents)
                 ->Belt.Option.flatMap(buyEvents =>
                     buyEvents->Belt.Array.get(0)
                   );
               //  let filterByNameQuery = QlHooks.InitialLoad.make();
               switch (latestBuyEvent) {
               | None => React.null
               | Some(buyEvent) =>
                 let filterByNameQuery =
                   QlHooks.SubWildcardQuery.make(
                     ~tokenId=buyEvent##token##id,
                     (),
                   );
                 let readQueryOptions = toReadQueryOptions(filterByNameQuery);
                 module WildcardsNameFilterReadQuery =
                   ApolloClient.ReadQuery(QlHooks.SubWildcardQuery);
                 module WildcardsNameFilterWriteQuery =
                   ApolloClient.WriteQuery(QlHooks.SubWildcardQuery);
                 switch (
                   WildcardsNameFilterReadQuery.readQuery(
                     client,
                     readQueryOptions,
                   )
                 ) {
                 | exception _ => ()
                 | cachedResponse =>
                   switch (cachedResponse |> Js.Nullable.toOption) {
                   | None => ()
                   | Some(cachedWildcards) =>
                     let wildcards = cast(cachedWildcards);

                     WildcardsNameFilterWriteQuery.make(
                       ~client,
                       ~variables=filterByNameQuery##variables,
                       ~data=wildcards,
                       (),
                     );
                   }
                 };
                 React.null;
               };
             }}
           </ApolloConsumer>
         )
       : React.null}
    children
  </RootContext>;
};
