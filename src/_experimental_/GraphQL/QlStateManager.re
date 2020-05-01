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
                   let setupWildcardForCache = [%raw
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
                   Js.log3("Cached Patron", cachedPatron, patron);
                   let setupPatronForCache = [%raw
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

               let queryNew =
                 QlHooks.LoadPatronNewNoDecode.make(~patronId=patron##id, ());
               module LoadPatronNewQuery =
                 ReasonApollo.CreateQuery(QlHooks.LoadPatronNewNoDecode);
               module LoadPatronNewWriteQuery =
                 ApolloClient.WriteQuery(QlHooks.LoadPatronNew);

               <LoadPatronNewQuery
                 variables=queryNew##variables fetchPolicy="network-only">
                 ...{({result, _}) => {
                   switch (result) {
                   | Loading
                   | Error(_) => React.null
                   | Data(response) =>
                     let filterByNameQuery =
                       QlHooks.LoadPatronNew.make(~patronId=patron##id, ());
                     let readQueryOptions =
                       ApolloHooks.toQueryObj(filterByNameQuery)->Obj.magic;
                     module LoadPatronNewReadQuery =
                       ApolloClient.ReadQuery(QlHooks.LoadPatronNew);
                     module LoadPatronNewWriteQuery =
                       ApolloClient.WriteQuery(QlHooks.LoadPatronNew);
                     switch (
                       LoadPatronNewReadQuery.readQuery(
                         client,
                         readQueryOptions,
                       )
                     ) {
                     | exception _ => ()
                     | cachedResponse =>
                       switch (cachedResponse |> Js.Nullable.toOption) {
                       | None => ()
                       | Some(cachedPatron) =>
                         let setupNewPatronForCache = [%raw
                           {|
                         (newPatronData, previousPatron) =>{
                           let newPatron = newPatronData.newPatron;
                           let prevPatron = previousPatron.newPatron;
                           return {
                             newPatron: {
                               typename: "NewPatron",
                               ...prevPatron,
                               ...newPatron,
                             }
                           }
                         }
                         |}
                         ];
                         LoadPatronNewWriteQuery.make(
                           ~client,
                           ~variables=filterByNameQuery##variables,
                           ~data=
                             setupNewPatronForCache(. response, cachedPatron),
                           (),
                         );
                       }
                     };
                     React.null;
                   }
                 }}
               </LoadPatronNewQuery>;
             })
           ->React.array;
         }}
       </ApolloConsumer>
     )}
    children
  </RootContext>;
};
