open Globals;

type owner = {. "address": Js.Json.t};

type price = {. "price": Eth.t};

type wildcard = {
  id: string,
  tokenId: TokenId.t,
  patronageNumerator: Js.Json.t,
  owner,
  price,
};

let decodeBN: Js.Json.t => BN.t =
  number =>
    number
    ->Js.Json.decodeString
    ->Belt.Option.mapWithDefault("0", a => a) /*trusting that gql will be reliable here*/
    ->BN.new_;

open GqlConverters;

module InitialLoad = [%graphql
  {|
       query($amount: Int!, $globalId: String!) {
         wildcards(first: $amount) {
           id
           animal: tokenId @ppxCustom(module: "GqlTokenId")
           owner {
             address
             id
           }
           price {
             price @ppxCustom(module: "Price")
             id
           }
           totalCollected @ppxCustom(module: "Price")
           timeCollected @ppxCustom(module: "BigInt")
           patronageNumeratorPriceScaled @ppxCustom(module: "BigInt")
           # timeCollected @ppxCustom(module: "GqlMoment")
           timeAcquired @ppxCustom(module: "GqlMoment")
           auctionStartPrice @ppxCustom(module: "BigInt")
           launchTime @ppxCustom(module: "BigInt")
         }
         global(id: $globalId) {
           id
           totalCollectedOrDueAccurate @ppxCustom(module: "BigInt")
           timeLastCollected @ppxCustom(module: "BigInt")
           totalTokenCostScaledNumeratorAccurate @ppxCustom(module: "BigInt")
           defaultAuctionLength @ppxCustom(module: "BigInt")
           defaultAuctionEndPrice @ppxCustom(module: "BigInt")
           defaultAuctionStartPrice @ppxCustom(module: "BigInt")
         }
       }
     |}
];

let createContext: Client.queryContext => 'a = Obj.magic;

let useInitialDataLoad = (~chain) => {
  Js.log(chain);
  None;
  /*let (simple, _full) =
      ApolloHooks.useQuery(
        ~notifyOnNetworkStatusChange=true,
        // ~fetchPolicy=ApolloHooksTypes.NoCache,
        ~fetchPolicy=ApolloHooksTypes.CacheFirst,
        // This is a wierd hack for the sake of caching
        ~variables=
          InitialLoad.make(
            ~amount=
              switch (chain) {
              | Client.MaticQuery => 31
              | Client.Neither
              | Client.MainnetQuery => 30
              },
            ~globalId=
              switch (chain) {
              | Client.MaticQuery => "Matic-Global"
              | Client.Neither
              | Client.MainnetQuery => "1"
              },
            (),
          )##variables,
        ~context={context: chain}->createContext,
        InitialLoad.definition,
      );

    switch (simple) {
    | Data(data) => Some(data)
    | Loading
    | NoData
    | Error(_) => None
    };*/
};

let useAnimalList = (~chain) => {
  let allData = useInitialDataLoad(~chain);
  React.useMemo2(
    () => {
      allData->oMap(data => data##wildcards->Array.map(wc => wc##animal))
      |||| [||]
    },
    (allData, chain),
  );
};

// module SubWildcardQuery = [%graphql
//   {|
//        query ($tokenId: String!) {
//          wildcard(id: $tokenId) {
//            id
//            animal: tokenId @ppxCustom(module: "GqlTokenId")
//            timeAcquired @ppxCustom(module: "GqlMoment")
//            totalCollected @ppxCustom(module: "Price")
//            patronageNumerator @ppxCustom(module: "BigInt")
//            patronageNumeratorPriceScaled @ppxCustom(module: "BigInt")
//            timeCollected @ppxCustom(module: "BigInt")
//            # timeCollected @ppxCustom(module: "GqlMoment")
//            price {
//              id
//              price @ppxCustom(module: "Price")
//            }
//            owner {
//              address @ppxCustom(module: "GqlAddress")
//              id
//            }
//            auctionStartPrice @ppxCustom(module: "BigInt")
//            launchTime @ppxCustom(module: "BigInt")
//          }
//        }
//      |}
// ];

// module WildcardDataQuery = [%graphql
//   {|
//     query ($tokenId: String!) {
//       launchedWildcards_by_pk(id: $tokenId) {
//         wildcard {
//           id
//           name
//           description
//           organization {
//             name
//             id
//           }
//           image
//           real_wc_photos {
//             image
//             photographer
//           }
//           artistOfWildcard {
//             name
//             id
//           }
//         }
//       }
//     }
//   |}
// ];

// module MaticStateQuery = [%graphql
//   {|
//     query ($address: String!, $network: String!) {
//       maticState(address: $address, network: $network) {
//         balance
//         daiNonce
//         error
//         stewardNonce
//       }
//     }
//   |}
// ];
// module HomeAnimalsQuery = [%graphql
//   {|
//     query {
//       homeAnimals {
//         id
//         next
//         prev
//         wildcardData {
//           description
//           id
//           name
//           organisationId
//         }
//       }
//     }
//   |}
// ];

module ArtistQuery = [%graphql
  {|
    query ($artistIdentifier: String!) {
      artist_by_pk(id: $artistIdentifier) {
        eth_address
        id
        name
        website
        launchedWildcards: wildcardData (where: {id: { _is_null: false}}) {
          key
          id
          name
          image
          organization {
            id
            name
            logo
          }
        }
        unlaunchedWildcards: wildcardData (where: {id: { _is_null: true}}) {
          key
          name
          image
          organization {
            id
            name
            logo
          }
        }
      }
    }
  |}
];
// // NOTE: If multiple transactions happen in the same block they may get missed, maybe one day that will be a problem for us ;)
// module SubStateChangeEvents = [%graphql
//   {|
//        subscription {
//          stateChanges(first: 1, orderBy: timestamp, orderDirection: desc) {
//            id
//            timestamp
//            wildcardChanges {
//              id
//              tokenId
//              timeAcquired
//              totalCollected
//              patronageNumeratorPriceScaled
//              timeCollected
//              price {
//                id
//                price
//              }
//              owner {
//                address
//                id
//              }
//            }
//            patronChanges {
//              id
//              address
//              lastUpdated
//              # lastUpdated @ppxCustom(module: "GqlMoment")
//              previouslyOwnedTokens {
//                id
//              }
//              tokens {
//                id
//              }
//              availableDeposit
//              patronTokenCostScaledNumerator
//              foreclosureTime
//            }
//          }
//        }
//      |}
// ];

// module LoadPatron = [%graphql
//   {|
//        query ($patronId: String!) {
//          patron(id: $patronId) {
//            id
//            previouslyOwnedTokens {
//              id
//            }
//            tokens {
//              id
//            }
//            availableDeposit  @ppxCustom(module: "Price")
//            patronTokenCostScaledNumerator  @ppxCustom(module: "BigInt")
//            foreclosureTime  @ppxCustom(module: "BigInt")
//            address @ppxCustom(module: "GqlAddress")
//            lastUpdated @ppxCustom(module: "BigInt")
//            totalLoyaltyTokens @ppxCustom(module: "BigInt")
//            totalLoyaltyTokensIncludingUnRedeemed @ppxCustom(module: "BigInt")
//          }
//        }
//      |}
// ];

// module LoadTokenDataArray = [%graphql
//   {|
//         query ($wildcardIdArray: [String!]!) {
//           wildcards (where: {id_in: $wildcardIdArray}) {
//             # totalCollected
//             # patronageNumeratorPriceScaled
//             # timeCollected
//             id
//             totalCollected @ppxCustom(module: "Price")
//             patronageNumeratorPriceScaled @ppxCustom(module: "BigInt")
//             timeCollected @ppxCustom(module: "BigInt")
//           }
//         }
//      |}
// ];
// module LoadOrganisationData = [%graphql
//   {|
//       query ($orgId: String!) {
//         organisations_by_pk(id: $orgId) {
//           description
//           name
//           website
//           wildcard (where: {id: {_is_null: false}}) {
//             id @ppxCustom(module: "GqlTokenId")
//           }
//           unlaunched: wildcard(where: {id: {_is_null: true}, real_wc_photos: {image: {_is_null: false}}}) {
//             key
//             real_wc_photos {
//               image
//               photographer
//             }
//             name
//             commonName
//             description
//           }
//           logo
//           logo_badge
//           youtube_vid
//         }
//       }
//      |}
// ];

// module LoadTopContributors = [%graphql
//   {|
//       query ($numberOfLeaders: Int!) {
//         patrons(first: $numberOfLeaders, orderBy: patronTokenCostScaledNumerator, orderDirection: desc, where: {id_not: "NO_OWNER"}) {
//           id
//           patronTokenCostScaledNumerator  @ppxCustom(module: "BigInt")
//         }
//       }
//   |}
// ];

// module SubTotalRaisedOrDueQuery = [%graphql
//   {|
//        query {
//          global(id: "1") {
//            id
//            totalCollectedOrDueAccurate @ppxCustom(module: "BigInt")
//            timeLastCollected @ppxCustom(module: "BigInt")
//            totalTokenCostScaledNumeratorAccurate @ppxCustom(module: "BigInt")
//          }
//        }
//      |}
// ];
type errorPlaceholder;
type graphqlDataLoad('a) =
  | Loading
  | Error(errorPlaceholder)
  // | Error(ReasonApolloHooks.ApolloHooksTypes.apolloError)
  | NoData
  | Data('a);

let getQueryPrefix = (chain: Client.context) =>
  switch (chain) {
  | MainnetQuery
  | Neither => ""
  | MaticQuery => "matic"
  };

let subscriptionResultOptionMap = (result, mapping) => {
  Js.log2(result, mapping);
  /*switch (result) {
    | ApolloHooks.Subscription.Data(response) => response->mapping->Some
    | ApolloHooks.Subscription.Error(_)
    | ApolloHooks.Subscription.Loading
    | ApolloHooks.Subscription.NoData => None
    };*/
  None;
};
let subscriptionResultToOption = result =>
  subscriptionResultOptionMap(result, a => a);

let queryResultOptionMap = (result, mapping) => {
  Js.log2(result, mapping);
  /*switch (result) {
    | ApolloHooks.Query.Data(response) => response->mapping->Some
    | ApolloHooks.Query.Error(_)
    | ApolloHooks.Query.Loading
    | ApolloHooks.Query.NoData => None
    };*/
  None;
};
let queryResultOptionFlatMap = (result, mapping) => {
  Js.log2(result, mapping);
  /*switch (result) {
    | ApolloHooks.Query.Data(response) => response->mapping
    | ApolloHooks.Query.Error(_)
    | ApolloHooks.Query.Loading
    | ApolloHooks.Query.NoData => None
    };*/
  None;
};
let queryResultToOption = result => queryResultOptionMap(result, a => a);

type data = {tokenId: string};

let useWildcardQuery = (~chain, tokenId) => Obj.magic((chain, tokenId));
// ApolloHooks.useQuery(
//   ~context={context: chain}->createContext,
//   ~variables=
//     SubWildcardQuery.make(
//       ~tokenId=chain->getQueryPrefix ++ tokenId->TokenId.toString,
//       (),
//     )##variables,
//   SubWildcardQuery.definition,
// );

let useLoadTokenDataArrayQuery = (~chain, tokenIdArray) =>
  Obj.magic((chain, tokenIdArray));
// ApolloHooks.useQuery(
//   ~variables=
//     LoadTokenDataArray.make(
//       ~wildcardIdArray=tokenIdArray->Array.map(id => id->TokenId.toString),
//       (),
//     )##variables,
//   ~context={context: chain}->createContext,
//   LoadTokenDataArray.definition,
// );
let useWildcardDataQuery = tokenId => {
  Obj.magic(
    tokenId,
    // ApolloHooks.useQuery(
    //   ~variables=
    //     WildcardDataQuery.make(~tokenId=tokenId->TokenId.toString, ())##variables,
    //   WildcardDataQuery.definition,
    // );
  );
};

let useHomeAnimalsQuery = () => Obj.magic();
// ApolloHooks.useQuery(HomeAnimalsQuery.definition);
// let useBuySubscription = () =>
//   ApolloHooks.useSubscription(
//     ~variables=SubBuyEvents.make()##variables,
//     SubBuyEvents.definition,
//   );
let useStateChangeSubscription = () => Obj.magic();
// ApolloHooks.useSubscription(
//   ~variables=SubStateChangeEvents.make()##variables,
//   SubStateChangeEvents.definition,
// );

let useLoadOrganisationQuery = orgId => Obj.magic(orgId);
// ApolloHooks.useQuery(
//   ~variables=LoadOrganisationData.make(~orgId, ())##variables,
//   LoadOrganisationData.definition,
// );
// let useBuySubscriptionData = () => {
//   let (simple, _) = useBuySubscription();
//   switch (simple) {
//   | Data(response) => Some(response)
//   | _ => None
//   };
// };
let useStateChangeSubscriptionData = () => {
  let (simple, _) = useStateChangeSubscription();
  subscriptionResultToOption(simple);
};
let useLoadOrganisationData = orgId => {
  Js.log(
    orgId,
    /*let (simple, _) = useLoadOrganisationQuery(orgId);
      queryResultToOption(simple);*/
  );
  None;
};
let useLoadOrganisationLogo = orgId => {
  Js.log(orgId);
  None;
  /* let result = useLoadOrganisationData(orgId);
     result
     ->Option.flatMap(org => org##organisations_by_pk)
     ->Option.map(org => org##logo); */
};
let useLoadOrganisationLogoBadge = orgId => {
  Js.log(orgId);
  None;
  /*  let result = useLoadOrganisationData(orgId);
      result
      ->Option.flatMap(org => org##organisations_by_pk)
      ->Option.map(org => org##logo_badge |||| org##logo); */
};
let useHomePageAnimalsData = () => {
  let (simple, _) = useHomeAnimalsQuery();
  queryResultToOption(simple);
};
// type wildcardData = {
//   description: string,
//   id: string,
//   name: string,
//   organisationId: string,
// };
type homePageAnimal = {
  id: TokenId.t,
  prev: TokenId.t,
  next: TokenId.t,
  // wildcardData,
};
let useHomePageAnimalArrayOpt = () => {
  useHomePageAnimalsData()
  ->oMap(homeAnimals =>
      homeAnimals##homeAnimals
      ->Array.map(animal =>
          {
            id: TokenId.fromStringUnsafe(animal##id),
            prev: TokenId.fromStringUnsafe(animal##prev),
            next: TokenId.fromStringUnsafe(animal##next),
          }
        )
    );
};
let useHomePageAnimalArray = () => {
  useHomePageAnimalArrayOpt() |||| [||];
};
let useDetailsPageNextPrevious = (currentToken: TokenId.t) => {
  let homepageAnimalData = useHomePageAnimalArray();
  let defaultValue = {
    id: TokenId.fromStringUnsafe("2"),
    next: TokenId.fromStringUnsafe("1"),
    prev: TokenId.fromStringUnsafe("0"),
  };
  let forwardNextLookup =
    React.useMemo1(
      () =>
        homepageAnimalData->Array.reduce(
          Js.Dict.empty(),
          (dict, item) => {
            dict->Js.Dict.set(item.id->TokenId.toString, item);
            dict;
          },
        ),
      [|homepageAnimalData|],
    );

  forwardNextLookup->Js.Dict.get(currentToken->TokenId.toString)
  |||| defaultValue;
};

[@decco.decode]
type animalDescription = array(string);
let useWildcardDescription = tokenId => {
  let (simple, _) = useWildcardDataQuery(tokenId);
  queryResultOptionMap(simple, a =>
    a##launchedWildcards_by_pk
    ->oMap(b =>
        b##wildcard##description
        ->animalDescription_decode
        ->Belt.Result.getWithDefault([||])
      )
    |||| [||]
  );
};

let useWildcardName = tokenId => {
  let (simple, _) = useWildcardDataQuery(tokenId);
  queryResultOptionFlatMap(simple, a =>
    a##launchedWildcards_by_pk->Option.flatMap(b => b##wildcard##name)
  );
};
let useWildcardAvatar = tokenId => {
  let (simple, _) = useWildcardDataQuery(tokenId);
  queryResultOptionFlatMap(simple, a =>
    a##launchedWildcards_by_pk->Option.flatMap(b => b##wildcard##image)
  );
};
let useWildcardArtist = tokenId => {
  let (simple, _) = useWildcardDataQuery(tokenId);
  queryResultOptionFlatMap(simple, a =>
    a##launchedWildcards_by_pk
    ->Option.flatMap(b => b##wildcard##artistOfWildcard)
  );
};
let useRealImages = tokenId => {
  let (simple, _) = useWildcardDataQuery(tokenId);
  queryResultOptionFlatMap(simple, a =>
    a##launchedWildcards_by_pk->Option.map(b => b##wildcard##real_wc_photos)
  );
};
let useWildcardOrgId = (~tokenId) => {
  let (simple, _) = useWildcardDataQuery(tokenId);
  queryResultOptionFlatMap(simple, a =>
    a##launchedWildcards_by_pk
    ->Option.flatMap(b => b##wildcard##organization)
    ->Option.map(org => org##id)
  );
};
let useWildcardOrgName = (~tokenId) => {
  let (simple, _) = useWildcardDataQuery(tokenId);
  queryResultOptionFlatMap(simple, a =>
    a##launchedWildcards_by_pk
    ->Option.flatMap(b => b##wildcard##organization)
    ->Option.map(org => org##name)
  );
};

let useLoadTopContributors = numberOfLeaders => {
  /*  ApolloHooks.useSubscription(
        ~variables=LoadTopContributors.make(~numberOfLeaders, ())##variables,
        LoadTopContributors.definition,
      ); */
  Js.log(numberOfLeaders);
  None;
};
let useLoadTopContributorsData = numberOfLeaders => {
  Js.log(numberOfLeaders);
  None;
  /*   let (simple, _) = useLoadTopContributors(numberOfLeaders);

       let getLargestContributors = largestContributors => {
         let monthlyContributions =
           largestContributors##patrons
           |> Js.Array.map(patron => {
                let monthlyContribution =
                  patron##patronTokenCostScaledNumerator
                  ->BN.mul(BN.new_("2592000")) // A month with 30 days has 2592000 seconds
                  ->BN.div(
                      // BN.new_("1000000000000")->BN.mul( BN.new_("31536000")),
                      BN.new_("31536000000000000000"),
                    )
                  ->Web3Utils.fromWeiBNToEthPrecision(~digits=4);
                (patron##id, monthlyContribution);
              });
         monthlyContributions;
       };

       simple->subscriptionResultOptionMap(getLargestContributors); */
};
let usePatron: (~chain: Client.context, TokenId.t) => option(string) =
  (~chain, animal) => {
    let (simple, _) = useWildcardQuery(~chain, animal);
    let getAddress = response =>
      response##wildcard
      ->Belt.Option.flatMap(wildcard => Some(wildcard##owner##address));

    simple->queryResultOptionFlatMap(getAddress);
  };

let useIsAnimalOwened = (~chain, ownedAnimal) => {
  let currentAccount =
    RootProvider.useCurrentUser()
    ->Belt.Option.mapWithDefault("loading", a => a);

  let currentPatron =
    usePatron(~chain, ownedAnimal)
    ->Belt.Option.mapWithDefault("no-patron-defined", a => a);

  currentAccount->Js.String.toLowerCase
  == currentPatron->Js.String.toLocaleLowerCase;
};

let useTimeAcquired:
  (~chain: Client.context, TokenId.t) => option(MomentRe.Moment.t) =
  (~chain, animal) => {
    let (simple, _) = useWildcardQuery(~chain, animal);
    let getTimeAquired = response =>
      response##wildcard
      ->Belt.Option.mapWithDefault(MomentRe.momentNow(), wildcard
          // wildcard
          => wildcard##timeAcquired);

    simple->queryResultOptionMap(getTimeAquired);
  };

let useQueryPatron = (~chain, patron) => {
  Js.log2(chain, patron);
  /*  // ApolloHooks.useQuery(
      //   ~context={context: chain}->createContext,
      //   ~variables=
      //     LoadPatron.make(~patronId=chain->getQueryPrefix ++ patron, ())##variables,
      //   LoadPatron.definition,
      // ); */
  None;
};

let useQueryPatronNew = patron => {
  Js.log(patron);
  /* ApolloHooks.useQuery(
       ~variables=LoadPatron.make(~patronId=patron, ())##variables,
       LoadPatron.definition,
     ); */
  None;
};

let useForeclosureTimeBn = (~chain, patron) => {
  Js.log2(chain, patron);
  None;
  /*   let (simple, _) = useQueryPatron(~chain, patron);
       let getForclosureTime = response =>
         response##patron->Belt.Option.map(patron => patron##foreclosureTime);

       simple->queryResultOptionFlatMap(getForclosureTime); */
};

let useForeclosureTime = (~chain, patron) => {
  useForeclosureTimeBn(~chain, patron)->Option.map(Helper.bnToMoment);
};

let usePatronQuery = (~chain, patron) => {
  Js.log2(chain, patron);
  None;
  /*   let (simple, _) = useQueryPatron(~chain, patron);

       simple->queryResultToOption; */
};
let useTimeAcquiredWithDefault = (~chain, animal, default: MomentRe.Moment.t) =>
  useTimeAcquired(~chain, animal) |||| default;
let useDaysHeld = (~chain, tokenId) =>
  useTimeAcquired(~chain, tokenId)
  ->oMap(moment =>
      (MomentRe.diff(MomentRe.momentNow(), moment, `days), moment)
    );
let useTotalCollectedOrDue: unit => option((BN.t, BN.t, BN.t)) =
  () => {
    None;
        /* let (simple, _) =
             ApolloHooks.useQuery(SubTotalRaisedOrDueQuery.definition);
           let getTotalCollected = response =>
             response##global
             ->oMap(global =>
                 (
                   global##totalCollectedOrDueAccurate,
                   global##timeLastCollected,
                   global##totalTokenCostScaledNumeratorAccurate,
                 )
               );

           simple->queryResultOptionFlatMap(getTotalCollected); */
  };

let getCurrentTimestamp = () =>
  string_of_int(Js.Math.floor(Js.Date.now() /. 1000.));

let useCurrentTime = () => {
  let (currentTime, setTimeLeft) =
    React.useState(() => getCurrentTimestamp());

  React.useEffect1(
    () => {
      let interval =
        Js.Global.setInterval(
          () => {setTimeLeft(_ => {getCurrentTimestamp()})},
          2000,
        );
      Some(() => Js.Global.clearInterval(interval));
    },
    [|setTimeLeft|],
  );
  currentTime;
};
let useCurrentTimestampBn = () => {
  useCurrentTime()->BN.new_;
};
let useAmountRaised = () => {
  let currentTimestamp = useCurrentTime();

  useTotalCollectedOrDue()
  ->oMap(
      (
        (
          amountCollectedOrDue,
          timeCalculated,
          totalTokenCostScaledNumeratorAccurate,
        ),
      ) => {
      let timeElapsed = BN.new_(currentTimestamp)->BN.sub(timeCalculated);

      let amountRaisedSinceLastCollection =
        totalTokenCostScaledNumeratorAccurate
        ->BN.mul(timeElapsed)
        ->BN.div(
            // BN.new_("1000000000000")->BN.mul( BN.new_("31536000")),
            BN.new_("31536000000000000000"),
          );
      amountCollectedOrDue->BN.add(amountRaisedSinceLastCollection);
    });
};

let useTotalCollectedToken:
  (~chain: Client.context, TokenId.t) => option((Eth.t, BN.t, BN.t)) =
  (~chain, animal) => {
    let (simple, _) = useWildcardQuery(~chain, animal);
    let getTotalCollectedData = response =>
      response##wildcard
      ->oMap(wc =>
          (
            wc##totalCollected,
            wc##timeCollected,
            wc##patronageNumeratorPriceScaled,
          )
        );

    simple->queryResultOptionFlatMap(getTotalCollectedData);
  };

let useTotalCollectedTokenArray = (~chain, animalArray) => {
  let (simple, _) = useLoadTokenDataArrayQuery(~chain, animalArray);
  simple->queryResultToOption;
};

let usePatronageNumerator = (~chain, tokenId: TokenId.t) => {
  let (simple, _) = useWildcardQuery(~chain, tokenId);
  let patronageNumerator = response =>
    response##wildcard
    ->Belt.Option.map(wildcard => wildcard##patronageNumerator);

  simple->queryResultOptionFlatMap(patronageNumerator);
};

// TODO: fix this, this is a hardcoded pledgerate. It should be fetched from the graph!
let usePledgeRate = (~chain, tokenId) => {
  let optPatronageNumerator = usePatronageNumerator(~chain, tokenId);
  React.useMemo1(
    () => {
      switch (optPatronageNumerator) {
      | Some(patronageNumerator) =>
        let result = patronageNumerator |/| BN.new_("12000000000");
        result->BN.toNumberFloat /. 1000.;
      | None => 0.
      }
    },
    [|optPatronageNumerator|],
  );
};

let usePledgeRateDetailed = (~chain, tokenId) => {
  let pledgeRate = usePledgeRate(~chain, tokenId);
  let inversePledgeRate = 1. /. pledgeRate;
  let numeratorOverYear = (pledgeRate *. 1200.)->Float.toInt->string_of_int;
  (numeratorOverYear, "100", pledgeRate, inversePledgeRate);
};

type patronLoyaltyTokenDetails = {
  currentLoyaltyTokens: Eth.t,
  currentLoyaltyTokensIncludingUnredeemed: Eth.t,
  lastCollected: BN.t,
  numberOfAnimalsOwned: BN.t,
};
let usePatronLoyaltyTokenDetails = (~chain, address) => {
  Js.log2(chain, address);
  None;
  /* // NOTE: we are using both 'new patron' and 'patron' because the work on the graph is incomplete.
     let (response, _) = useQueryPatron(~chain, address);

     [@ocaml.warning "-4"]
     (
       switch (response) {
       | Data(dataPatron) =>
         switch (dataPatron##patron) {
         | Some(patron) =>
           Some({
             currentLoyaltyTokens: patron##totalLoyaltyTokens,
             currentLoyaltyTokensIncludingUnredeemed:
               patron##totalLoyaltyTokensIncludingUnRedeemed,
             lastCollected: patron##lastUpdated,
             numberOfAnimalsOwned:
               BN.new_(patron##tokens->Obj.magic->Array.length->string_of_int),
           })
         | _ => None
         }
       // | Loading
       // | Error(_error)
       // | NoData => None
       | _ => None
       }
     ); */
};

// TODO:: Take min of total deposit and amount raised
let useAmountRaisedToken: (~chain: Client.context, TokenId.t) => option(Eth.t) =
  (~chain, animal) => {
    let currentTimestamp = useCurrentTime();

    switch (useTotalCollectedToken(~chain, animal)) {
    | Some((
        amountCollectedOrDue,
        timeCalculated,
        patronageNumeratorPriceScaled,
      )) =>
      let timeElapsed = BN.new_(currentTimestamp)->BN.sub(timeCalculated);

      let amountRaisedSinceLastCollection =
        patronageNumeratorPriceScaled
        ->BN.mul(timeElapsed)
        ->BN.div(
            // BN.new_("1000000000000")->BN.mul( BN.new_("31536000")),
            BN.new_("31536000000000000000"),
          );

      Some(amountCollectedOrDue->BN.add(amountRaisedSinceLastCollection));
    | None => None
    };
  };
let calculateTotalRaised =
    (
      currentTimestamp,
      (amountCollectedOrDue, timeCalculated, patronageNumeratorPriceScaled),
    ) => {
  let timeElapsed = BN.new_(currentTimestamp)->BN.sub(timeCalculated);

  let amountRaisedSinceLastCollection =
    patronageNumeratorPriceScaled
    ->BN.mul(timeElapsed)
    ->BN.div(
        // BN.new_("1000000000000")->BN.mul( BN.new_("31536000")),
        BN.new_("31536000000000000000"),
      );

  amountCollectedOrDue->BN.add(amountRaisedSinceLastCollection);
};
let useTotalRaisedAnimalGroup:
  array(TokenId.t) => (option(Eth.t), option(Eth.t)) =
  animals => {
    let currentTimestamp = useCurrentTime();

    let detailsMainnet =
      useTotalCollectedTokenArray(~chain=Client.MainnetQuery, animals);
    let detailsMatic =
      useTotalCollectedTokenArray(
        ~chain=Client.MaticQuery,
        animals->Array.map(id => ("matic" ++ id->Obj.magic)->Obj.magic),
      );

    (
      switch (detailsMainnet) {
      | Some(detailsArray) =>
        Some(
          detailsArray##wildcards
          ->Array.reduce(BN.new_("0"), (acc, animalDetails) =>
              calculateTotalRaised(
                currentTimestamp,
                (
                  animalDetails##totalCollected,
                  animalDetails##timeCollected,
                  animalDetails##patronageNumeratorPriceScaled,
                ),
              )
              |+| acc
            ),
        )
      | None => None
      },
      switch (detailsMatic) {
      | Some(detailsArray) =>
        Some(
          detailsArray##wildcards
          ->Array.reduce(BN.new_("0"), (acc, animalDetails) =>
              calculateTotalRaised(
                currentTimestamp,
                (
                  animalDetails##totalCollected,
                  animalDetails##timeCollected,
                  animalDetails##patronageNumeratorPriceScaled,
                ),
              )
              |+| acc
            ),
        )
      | None => None
      },
    );
  };

let useTimeSinceTokenWasLastSettled:
  (~chain: Client.context, TokenId.t) => option(BN.t) =
  (~chain, animal) => {
    let currentTimestamp = useCurrentTime();

    switch (useTotalCollectedToken(~chain, animal)) {
    | Some((_, timeCalculated, _)) =>
      let timeElapsed = BN.new_(currentTimestamp)->BN.sub(timeCalculated);

      Some(timeElapsed);
    | None => None
    };
  };

let useUnredeemedLoyaltyTokenDueForUser:
  (~chain: Client.context, TokenId.t, int) => option(Eth.t) =
  (~chain, animal, numberOfTokens) => {
    switch (useTimeSinceTokenWasLastSettled(~chain, animal)) {
    | Some(timeSinceTokenWasLastSettled) =>
      let totalLoyaltyTokensPerSecondPerAnimal = BN.new_("11574074074074");
      let totalLoyaltyTokensForAllAnimals =
        timeSinceTokenWasLastSettled
        |*| totalLoyaltyTokensPerSecondPerAnimal
        |*| BN.newInt_(numberOfTokens);
      Some(totalLoyaltyTokensForAllAnimals);
    | None => None
    };
  };
let useTotalLoyaltyToken:
  (~chain: Client.context, Web3.ethAddress) => option((Eth.t, Eth.t)) =
  (~chain, patron) => {
    let currentTimestamp = useCurrentTime();

    switch (usePatronLoyaltyTokenDetails(~chain, patron)) {
    | Some({
        currentLoyaltyTokens,
        currentLoyaltyTokensIncludingUnredeemed,
        lastCollected,
        numberOfAnimalsOwned,
      }) =>
      let timeElapsed = BN.new_(currentTimestamp) |-| lastCollected;
      // Reference: https://github.com/wild-cards/contracts-private/blob/v2testing/migrations/7_receipt_tokens.js#L6
      let totalLoyaltyTokensPerSecondPerAnimal = BN.new_("11574074074074");
      let totalLoyaltyTokensFor1Animal =
        totalLoyaltyTokensPerSecondPerAnimal |*| timeElapsed;
      let totalLoyaltyTokensForAllAnimals =
        numberOfAnimalsOwned |*| totalLoyaltyTokensFor1Animal;
      let totalLoyaltyTokensForUser =
        currentLoyaltyTokensIncludingUnredeemed
        |+| totalLoyaltyTokensForAllAnimals;
      Some((totalLoyaltyTokensForUser, currentLoyaltyTokens));
    | None => None
    };
  };

let useRemainingDeposit:
  (~chain: Client.context, string) => option((Eth.t, BN.t, BN.t)) =
  (~chain, patron) => {
    Js.log2(chain, patron);
    None;
    /* let (simple, _) = useQueryPatron(~chain, patron);

       let getRemainingDepositData = response =>
         response##patron
         ->oMap(wc =>
             (
               wc##availableDeposit,
               wc##lastUpdated,
               wc##patronTokenCostScaledNumerator,
             )
           );

       simple->queryResultOptionFlatMap(getRemainingDepositData); */
  };

// TODO:: Take min of total deposit and amount raised
let useRemainingDepositEth: (~chain: Client.context, string) => option(Eth.t) =
  (~chain, patron) => {
    let currentTimestamp = useCurrentTime();

    switch (useRemainingDeposit(~chain, patron)) {
    | Some((availableDeposit, lastUpdated, patronTokenCostScaledNumerator)) =>
      let timeElapsed = BN.new_(currentTimestamp)->BN.sub(lastUpdated);

      let amountRaisedSinceLastCollection =
        patronTokenCostScaledNumerator
        ->BN.mul(timeElapsed)
        ->BN.div(
            // BN.new_("1000000000000")->BN.mul( BN.new_("31536000")),
            BN.new_("31536000000000000000"),
          );
      Some(availableDeposit->BN.sub(amountRaisedSinceLastCollection));
    | None => None
    };
  };

type animalPrice =
  | Foreclosed(BN.t)
  | Price(Eth.t)
  | Loading;

let usePrice: (~chain: Client.context, TokenId.t) => animalPrice =
  (~chain, animal) => {
    /*let (simple, _) = useWildcardQuery(~chain, animal);
      let optCurrentPatron = usePatron(~chain, animal);
      let currentPatron =
        optCurrentPatron->Belt.Option.mapWithDefault("no-patron-defined", a =>
          a
        );
      let foreclosureTime = useForeclosureTimeBn(~chain, currentPatron);

      let currentTime = useCurrentTime();

      switch (simple) {
      | Data(response) =>
        let priceValue =
          response##wildcard
          ->Belt.Option.mapWithDefault(Eth.makeFromInt(0), wildcard =>
              wildcard##price##price
            );

        switch (optCurrentPatron, foreclosureTime) {
        | (Some(_currentPatron), Some(foreclosureTime)) =>
          if (foreclosureTime->BN.lt(currentTime->BN.new_)) {
            Foreclosed(foreclosureTime);
          } else {
            Price(priceValue);
          }
        | (Some(_), None) => Price(priceValue)
        | _ => Loading
        };
      | Error(_)
      | Loading
      | NoData => Loading
      }; */

    Js.log2(chain, animal);
    Loading;
  };

let useIsForeclosed = (~chain, currentPatron) => {
  let optAvailableDeposit = useRemainingDepositEth(~chain, currentPatron);

  optAvailableDeposit->Option.mapWithDefault(true, availableDeposit => {
    !availableDeposit->BN.gt(BN.new_("0"))
  });
};

let useAuctionStartPrice = (~chain, _tokenId: TokenId.t) => {
  let optData = useInitialDataLoad(~chain);

  optData
  ->Option.flatMap(data => data##global)
  ->Option.map(global => global##defaultAuctionStartPrice);
};
let useAuctionEndPrice = (~chain, _tokenId: TokenId.t) => {
  let optData = useInitialDataLoad(~chain);

  optData
  ->Option.flatMap(data => data##global)
  ->Option.map(global => global##defaultAuctionEndPrice);
};
let useAuctioLength = (~chain, _tokenId: TokenId.t) => {
  let optData = useInitialDataLoad(~chain);

  optData
  ->Option.flatMap(data => data##global)
  ->Option.map(global => global##defaultAuctionLength);
};
let useLaunchTimeBN = (~chain, tokenId: TokenId.t) => {
  Js.log2(chain, tokenId);
  None;
  /* let (simple, _) = useWildcardQuery(~chain, tokenId);

     switch (simple) {
     | Data(response) =>
       response##wildcard->Belt.Option.map(wildcard => wildcard##launchTime)
     | Error(_)
     | Loading
     | NoData => None
     }; */
};

let useMaticStateQuery = (~forceRefetch, address, network) =>
  Js.log3(forceRefetch, address, network);
/* ApolloHooks.useQuery(
     ~variables=MaticStateQuery.make(~address, ~network, ())##variables,
     ~fetchPolicy=
       forceRefetch
         ? ReasonApolloHooks.ApolloHooksTypes.CacheAndNetwork
         : ReasonApolloHooks.ApolloHooksTypes.CacheFirst,
     ~context=Client.MainnetQuery->Obj.magic,
     MaticStateQuery.definition,
   ); */
None;

// let useMaticStateQueryPartial = forceRefetch =>
//   ApolloHooks.useQuery(
//     ~fetchPolicy=
//       forceRefetch
//         ? ReasonApolloHooks.ApolloHooksTypes.CacheAndNetwork
//         : ReasonApolloHooks.ApolloHooksTypes.CacheFirst,
//     ~context=Client.MainnetQuery->Obj.magic,
//   );
let useMaticState = (~forceRefetch, address, network) => {
  Js.log3(forceRefetch, address, network);
  None;
  /*  let (simple, _) = useMaticStateQuery(~forceRefetch, address, network);
      switch (simple) {
      | Data(response) => Some(response##maticState)
      | Error(_)
      | Loading
      | NoData => None
      }; */
};
// let useForceUpdateMaticStateFunc = network => {
//   let partialMaticStateReq = useMaticStateQueryPartial(true);
//   address => {
//     let (simple, _) =
//       partialMaticStateReq(
//         ~variables=MaticStateQuery.make(~address, ~network, ())##variables,
//         MaticStateQuery.definition,
//       );
//     switch (simple) {
//     | Data(response) => Some(response##maticState)
//     | Error(_)
//     | Loading
//     | NoData => None
//     };
//   };
// };

let useArtistData = (~artistIdentifier) => {
  let artistQuery =
    ArtistQuery.use(ArtistQuery.makeVariables(~artistIdentifier, ()));

  switch (artistQuery) {
  | {loading: true, _} => None
  | {error: Some(_error), _} => None
  | {data: Some({artist_by_pk}), _} => artist_by_pk
  | _ => None
  };
};

let useArtistEthAddress = (~artistIdentifier) => {
  let artistData = useArtistData(~artistIdentifier);
  artistData->Option.flatMap(data => {data.eth_address});
};
let useArtistName = (~artistIdentifier) => {
  let artistData = useArtistData(~artistIdentifier);
  artistData->Option.map(data => data.name);
};
let useArtistWebsite = (~artistIdentifier) => {
  let artistData = useArtistData(~artistIdentifier);
  artistData->Option.flatMap(data => data.website);
};
let useArtistLaunchedWildcards = (~artistIdentifier) => {
  let artistData = useArtistData(~artistIdentifier);
  artistData->Option.map(data => data.launchedWildcards);
};
let useArtistUnlaunchedWildcards = (~artistIdentifier) => {
  let artistData = useArtistData(~artistIdentifier);
  artistData->Option.map(data => data.unlaunchedWildcards);
};
type wildcardKey = int;
type artistOrg = {
  id: string,
  name: string,
  logo: string,
  wildcards: array(wildcardKey),
};
let useArtistOrgs = (~artistIdentifier) => {
  let artistData = useArtistData(~artistIdentifier);
  artistData->Option.map(data => {
    let dict = Js.Dict.empty();
    data.launchedWildcards
    ->Array.map(wildcard => {
        switch (wildcard.organization) {
        | Some(org) =>
          let orgId = org.id;
          switch (dict->Js.Dict.get(orgId)) {
          | Some(orgObj) =>
            let newOrgObj = {
              ...orgObj,
              wildcards: orgObj.wildcards->Array.concat([|wildcard.key|]),
            };
            dict->Js.Dict.set(orgId, newOrgObj);
          | None =>
            dict->Js.Dict.set(
              orgId,
              {
                id: orgId,
                name: org.name,
                logo: org.logo,
                wildcards: [|wildcard.key|],
              },
            )
          };

        | None => ()
        }
      })
    ->ignore;
    dict->Js.Dict.values;
  });
};
