open Providers.UsdPriceProvider;
open Belt.Option;

module TotalRaisedEtherCountup = {
  [@bs.module "./TotalRaisedEtherCountup.js"] [@react.component]
  external make: (~totalRaised: string) => React.element = "default";
};

type patronageRaised =
  | Loaded(string, option(string))
  | Loading;

let uesTotalPatronage = () => {
  let optTotalPatronageWei = QlHooks.useAmountRaised(); //->Web3Utils.fromWeiBNToEth;
  let optCurrentUsdEthPrice = useUsdPrice(); //->mapWithDefault(0., a => a);
  // let optCurrentUsdEthPrice = Some(0.5); //->mapWithDefault(0., a => a);

  switch (optTotalPatronageWei) {
  | Some(totalPatronageWei) =>
    let totalPatronageEth =
      totalPatronageWei->BN.toStringGet(.)->Web3Utils.fromWeiToEth;

    let optTotaPatronageUsd =
      optCurrentUsdEthPrice->Belt.Option.flatMap(currentUsdEthPrice =>
        Some(
          Js.Float.toFixedWithPrecision(
            Belt.Float.fromString(totalPatronageEth)
            ->mapWithDefault(0., a => a)
            *. currentUsdEthPrice,
            ~digits=2,
          ),
        )
      );

    Loaded(totalPatronageEth, optTotaPatronageUsd);
  | _ => Loading
  };
};

external cast: Js.Json.t => QlHooks.SubWildcardQuery.t = "%identity";
type person = {
  .
  "id": string,
  "timeAcquired": string,
};

[@react.component]
let make = () => {
  let totalPatronageRaised = uesTotalPatronage();

  switch (totalPatronageRaised) {
  | Loaded(totalRaised, optTotaPatronageUsd) =>
    <p>
      <small>
        <span className={Styles.totalRaisedText(1.5)}>
          {React.string("Wildcards has currently raised ")}
        </span>
        <br />
        <span className={Styles.totalRaisedText(4.)}>
          <TotalRaisedEtherCountup totalRaised />
          <strong> {React.string(" ETH ")} </strong>
        </span>
        <br />
        {switch (optTotaPatronageUsd) {
         | Some(totalPatronageUsd) =>
           <React.Fragment>
             <span className={Styles.totalRaisedText(2.5)}>
               {React.string("(")}
               {React.string(totalPatronageUsd)}
               <strong> {React.string(" USD")} </strong>
               {React.string(")")}
             </span>
             <br />
             <span className={Styles.totalRaisedText(1.5)}>
               {React.string(" for conservation.")}
             </span>
           </React.Fragment>
         | None => React.null
         }}
      </small>
    </p>
  | Loading => <Rimble.Loader />
  };
};
// <ApolloConsumer>
//   ...{client => {
//     // Js.log(client);

//     open ReasonApolloHooks.ApolloHooks;

//     let filterByNameQuery =
//       QlHooks.SubWildcardQuery.make(~tokenId="0", ());
//     let readQueryOptions = toReadQueryOptions(filterByNameQuery);

//     module PersonsNameFilterReadQuery =
//       ApolloClient.ReadQuery(QlHooks.SubWildcardQuery);
//     module PersonsNameFilterWriteQuery =
//       ApolloClient.WriteQuery(QlHooks.SubWildcardQuery);

//     // let updateFiltered =
//     //     (person: person, name, filteredPersons: array(person)) =>
//     //   person##name === name
//     //     ? filteredPersons->Belt.Array.concat([|person|])
//     //     : filteredPersons->Belt.Array.keep(p => p##id !== person##id);

//     // By default, apollo adds field __typename to the query and will use it
//     // to normalize data. Parsing the result with Config.parse will remove the field,
//     // which won't allow to save the data back to cache. This means we can't use ReadQuery.make,
//     // which parses cache result, and have to use the readQuery which returns Json.t.
//     switch (
//       PersonsNameFilterReadQuery.readQuery(client, readQueryOptions)
//     ) {
//     | exception _ => ()
//     | cachedResponse =>
//       switch (cachedResponse |> Js.Nullable.toOption) {
//       | None => ()
//       | Some(cachedPersons) =>
//         // Js.log("Cached Result For Simon");
//         // Js.log(cachedPersons);
//         // // readQuery returns unparsed data as Json.t, but since PersonsNameFilterQuery
//         // // is not using any graphql_ppx directive, the data will have the same format,
//         // // (with the addition of __typename field) and can be cast to PersonsNameFilterConfig.t.
//         let persons = cast(cachedPersons);
//         // Js.log(persons);
//         let processPerson:
//           WildCards.QlHooks.SubWildcardQuery.t =>
//           WildCards.QlHooks.SubWildcardQuery.t = [%raw
//           "(person) => {
//               // console.log(person.wildcard.timeAcquired)
//               // console.log(person.wildcard.timeAcquired + 50)
//               let timeAcquired = parseInt(person.wildcard.timeAcquired)
//               // console.log(timeAcquired)
//               return {
//                 ...person,
//                 wildcard: {
//                  ...person.wildcard,
//                 timeAcquired: (timeAcquired - 86400).toString()
//                 }
//               }
//              }"
//         ];
//         let updatedPersons = processPerson(persons);
//         // Js.log(updatedPersons);
//         PersonsNameFilterWriteQuery.make(
//           ~client,
//           ~variables=filterByNameQuery##variables,
//           ~data=updatedPersons,
//           (),
//         );
//       }
//     };

//     <p> "testing"->React.string </p>;
//   }}
// </ApolloConsumer>
