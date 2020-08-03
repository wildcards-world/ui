open Globals;
open ReasonApolloHooks;

external cast: Js.Json.t => QlHooks.SubWildcardQuery.t = "%identity";

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
  let result = QlHooks.useStateChangeSubscriptionData();

  let initialDataLoad = QlHooks.useInitialDataLoad();
  let hasLoadedInitialData =
    switch (initialDataLoad) {
    | Some(_) => true
    | None => false
    };

  <RootContext value=hasLoadedInitialData>
    {result->Belt.Option.mapWithDefault(React.null, result =>
       <ApolloConsumer>
         ...{client => {
           let stateChanges = result##stateChanges->Array.getUnsafe(0);
           let changedWildcards = stateChanges##wildcardChanges;
           let changedPatrons = stateChanges##patronChanges;

           let _ =
             changedWildcards->Array.map(wildcard => {
               let query =
                 QlHooks.SubWildcardQuery.make(~tokenId=wildcard##id, ());
               let readQueryOptions =
                 ApolloHooks.toQueryObj(query)->Obj.magic;
               module SubWildcardQueryReadQuery =
                 ApolloClient.ReadQuery(QlHooks.SubWildcardQuery);
               module SubWildcardQueryWriteQuery =
                 ApolloClient.WriteQuery(QlHooks.SubWildcardQuery);
               switch (
                 SubWildcardQueryReadQuery.readQuery(client, readQueryOptions)
               ) {
               | exception _ => ()
               | cachedResponse =>
                 switch (cachedResponse |> Js.Nullable.toOption) {
                 | None => ()
                 | Some(cachedWildcard) =>
                   let setupWildcardForCache:
                     (
                       . {
                           .
                           "id": string,
                           "owner": {
                             .
                             "address": Js.Json.t,
                             "id": string,
                           },
                           "patronageNumeratorPriceScaled": Js.Json.t,
                           "price": {
                             .
                             "id": string,
                             "price": Js.Json.t,
                           },
                           "timeAcquired": Js.Json.t,
                           "timeCollected": Js.Json.t,
                           "tokenId": Js.Json.t,
                           "totalCollected": Js.Json.t,
                         },
                       Js.Json.t
                     ) =>
                     QlHooks.SubWildcardQuery.t = [%raw
                     {|
                           (wildcardData, cachedWildcard) =>{
                             return {
                               wildcard: {
                                 typename: "Wildcard",
                                 ...wildcardData,
                                 ...cachedWildcard.wildcard
                               }
                             }
                           }
                           |}
                   ];
                   SubWildcardQueryWriteQuery.make(
                     ~client,
                     ~variables=query##variables,
                     ~data=setupWildcardForCache(. wildcard, cachedWildcard),
                     (),
                   );
                 }
               };
             });

           changedPatrons
           ->Array.map(patron => {
               let patronQuery =
                 QlHooks.LoadPatron.make(~patronId=patron##id, ());
               let readQueryOptions =
                 ApolloHooks.toQueryObj(patronQuery)->Obj.magic;
               module LoadPatronReadQuery =
                 ApolloClient.ReadQuery(QlHooks.LoadPatron);
               module LoadPatronWriteQuery =
                 ApolloClient.WriteQuery(QlHooks.LoadPatron);
               switch (
                 LoadPatronReadQuery.readQuery(client, readQueryOptions)
               ) {
               | exception _ => ()
               | cachedResponse =>
                 switch (cachedResponse |> Js.Nullable.toOption) {
                 | None => ()
                 | Some(cachedPatron) =>
                   let setupPatronForCache:
                     (
                       . {
                           .
                           "address": Js.Json.t,
                           "availableDeposit": Js.Json.t,
                           "foreclosureTime": Js.Json.t,
                           "id": string,
                           "lastUpdated": Js.Json.t,
                           "patronTokenCostScaledNumerator": Js.Json.t,
                           "previouslyOwnedTokens":
                             Js.Array.t({. "id": string}),
                           "tokens": Js.Array.t({. "id": string}),
                         },
                       Js.Json.t
                     ) =>
                     QlHooks.LoadPatron.t = [%raw
                     {|
                           (patronData, cachedPatron) =>{
                             return {
                               patron: {
                                 typename: "Patron",
                                 ...patronData,
                                 ...cachedPatron.patron
                               }
                             }
                           }
                           |}
                   ];
                   LoadPatronWriteQuery.make(
                     ~client,
                     ~variables=patronQuery##variables,
                     ~data=setupPatronForCache(. patron, cachedPatron),
                     (),
                   );
                 }
               };

               React.null;
             })
           ->React.array;
         }}
       </ApolloConsumer>
     )}
    children
  </RootContext>;
};
