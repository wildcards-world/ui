open Globals;
open ReasonApolloHooks;

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

// type gqlState = {isLoaded: bool};

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
  // let lastEvent = React.useState(() => );

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
           Js.log("clint");
           Js.log(client);
           Js.log("clint");
           let stateChanges = result##stateChanges->Array.getUnsafe(0);
           let changedWildcards = stateChanges##wildcardChanges;
           let changedPatrons = stateChanges##patronChanges;
           Js.log4(
             "STATE CHANGE!!",
             changedWildcards,
             changedPatrons,
             stateChanges,
           );

           let _ =
             changedWildcards->Array.map(wildcard => {
               Js.log2("changed wildcard", wildcard);
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

           //  let _ =
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
                 //  variables=queryNew##variables fetchPolicy="no-cache">

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
                           //  let setupPatronNewForCache = [%raw
                           //    {|
                           //  (newPatronData, previousPatronData) =>{
                           //    console.log("inside javascript world", newPatronData);
                           //    const updatedData = {
                           //      patronNew: {
                           //        ...previousPatronData.patronNew,
                           //        ...newPatronData.patronNew,
                           //      }
                           //    };
                           //    console.log("in js, new updated data", updatedData);
                           //    return updatedData;
                           //   //  return newPatronData;
                           //  }
                           //  |}
                           //  ];
                           let newPatronData = response;
                           Js.log(newPatronData);
                           //    let addPatronNewTypename = [%raw
                           //      {|
                           //  (newPatronData) =>{
                           //    console.log("inside javascript world ACTUAL", newPatronData);
                           //    const updatedData = {
                           //      ...newPatronData,
                           //      patronNew: {
                           //        ...newPatronData.patronNew,
                           //        typename: "patronNew",
                           //        totalLoyaltyTokens: newPatronData.patronNew.totalLoyaltyTokens + "0",
                           //        totalLoyaltyTokensIncludingUnRedeemed: newPatronData.patronNew.totalLoyaltyTokensIncludingUnRedeemed + "0",
                           //      }
                           //    };
                           //    console.log("in js, new updated data ACTUAL", updatedData);
                           //    return updatedData;
                           //  }
                           //  |}
                           //    ];
                           //  let newData =
                           //    setupPatronNewForCache(. response, cachedPatron);
                           //  let oldData =
                           //  LoadPatronNewWriteQuery.make(
                           //    ~client,
                           //    ~variables=filterByNameQuery##variables,
                           //    ~data=newData,
                           //    (),
                           //  );
                           //  addPatronNewTypename(. cachedPatron)->ignore;
                           Js.log3("Cached Patron", cachedPatron, patron);
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
                             //  ~data=
                             //    setupPatronNewForCache(.
                             //      response,
                             //      cachedPatron,
                             //    ),
                             ~data=
                               setupNewPatronForCache(.
                                 response,
                                 cachedPatron,
                               ),
                             (),
                           );
                         }
                       };
                       React.null;
                     }
                   }}
                 </LoadPatronNewQuery>;
               //  React.null;
             })
           ->React.array;
         }}
       </ApolloConsumer>
     )
     //  React.null;
}
    children
  </RootContext>;
};
