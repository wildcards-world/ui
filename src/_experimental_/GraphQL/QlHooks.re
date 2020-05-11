open ReasonApolloHooks;
open Globals;

type owner = {. "address": Js.Json.t};

type price = {. "price": Eth.t};

type wildcard = {
  id: string,
  tokenId: Animal.t,
  patronageNumerator: Js.Json.t,
  owner,
  price,
};

let tokenIdToAnimal: Js.Json.t => Animal.t =
  tokenIdJson =>
    tokenIdJson
    ->Js.Json.decodeString
    ->Belt.Option.mapWithDefault("0", a => a)
    ->Animal.getAnimalFromId
    ->Belt.Option.mapWithDefault(Animal.Vitalik, a => a);

let decodePrice: Js.Json.t => Eth.t =
  price =>
    price
    ->Js.Json.decodeString
    ->Belt.Option.mapWithDefault("0", a => a)
    ->Eth.makeWithDefault(0);

let decodeMoment: Js.Json.t => MomentRe.Moment.t =
  price =>
    price
    ->Js.Json.decodeString
    ->Belt.Option.mapWithDefault(0, a => a->int_of_string) /*trusting that gql will be reliable here*/
    ->MomentRe.momentWithUnix;
let decodeBN: Js.Json.t => BN.bn =
  number =>
    number
    ->Js.Json.decodeString
    ->Belt.Option.mapWithDefault("0", a => a) /*trusting that gql will be reliable here*/
    ->BN.new_;
// TODO: make a real address string
let decodeAddress: Js.Json.t => string =
  address =>
    address->Js.Json.decodeString->Belt.Option.mapWithDefault("0x0", a => a);

module InitialLoad = [%graphql
  {|
    query {
      wildcards(first: 14) {
        id
        animal: tokenId @bsDecoder(fn: "tokenIdToAnimal")
        owner {
          address
          id
        }
        price {
          price @bsDecoder(fn: "decodePrice")
          id
        }
        totalCollected @bsDecoder(fn: "decodePrice")
        timeCollected @bsDecoder(fn: "decodeBN")
        patronageNumeratorPriceScaled @bsDecoder(fn: "decodeBN")
        # timeCollected @bsDecoder(fn: "decodeMoment")
        timeAcquired @bsDecoder(fn: "decodeMoment")
      }
      global(id: 1) {
        id
        totalCollectedOrDueAccurate @bsDecoder(fn: "decodeBN")
        timeLastCollected @bsDecoder(fn: "decodeBN")
        totalTokenCostScaledNumeratorAccurate @bsDecoder(fn: "decodeBN")
      }
    }
  |}
];

// TODO: remove this function, it was an interesting but failed experiment.
let useInitialDataLoad = () => {
  let (simple, _full) =
    ApolloHooks.useQuery(
      ~notifyOnNetworkStatusChange=true,
      InitialLoad.definition,
    );

  switch (simple) {
  | Data(data) => Some(data)
  | Loading
  | NoData
  | Error(_) => None
  };
};

module SubWildcardQuery = [%graphql
  {|
    query ($tokenId: String!) {
      wildcard(id: $tokenId) {
        id
        animal: tokenId @bsDecoder(fn: "tokenIdToAnimal")
        timeAcquired @bsDecoder(fn: "decodeMoment")
        totalCollected @bsDecoder(fn: "decodePrice")
        patronageNumeratorPriceScaled @bsDecoder(fn: "decodeBN")
        timeCollected @bsDecoder(fn: "decodeBN")
        # timeCollected @bsDecoder(fn: "decodeMoment")
        price {
          id
          price @bsDecoder(fn: "decodePrice")
        }
        owner {
          address @bsDecoder(fn: "decodeAddress")
          id
        }
      }
    }
  |}
];
// NOTE: If multiple transactions happen in the same block they may get missed, maybe one day that will be a problem for us ;)
module SubStateChangeEvents = [%graphql
  {|
    subscription {
      stateChanges(first: 1, orderBy: timestamp, orderDirection: desc) {
        id
        timestamp
        txEventList
        wildcardChanges {
          id
          tokenId
          timeAcquired
          totalCollected
          patronageNumeratorPriceScaled
          timeCollected
          price {
            id
            price
          }
          owner {
            address
            id
          }
        }
        patronChanges {
          id
          address
          lastUpdated
          # lastUpdated @bsDecoder(fn: "decodeMoment")
          previouslyOwnedTokens {
            id
          }
          tokens {
            id
          }
          availableDeposit
          patronTokenCostScaledNumerator
          foreclosureTime
        }
      }
    }
  |}
];
// module SubBuyEvents = [%graphql
//   {|
//     subscription {
//       eventCounter(id: 1) {
//         id
//         buyEventCount
//           buyEvents(first: 5, orderBy: timestamp, orderDirection: desc) {
//           id
//           token {
//             id
//             # NOTE: no need to decode these values, this is only for updating the cache.
//             animal: tokenId #@bsDecoder(fn: "tokenIdToAnimal")
//             timeAcquired #@bsDecoder(fn: "decodeMoment")
//             totalCollected #@bsDecoder(fn: "decodePrice")
//             patronageNumeratorPriceScaled #@bsDecoder(fn: "decodeBN")
//             timeCollected #@bsDecoder(fn: "decodeBN")
//             # timeCollected @bsDecoder(fn: "decodeMoment")
//             price {
//               id
//               price #@bsDecoder(fn: "decodePrice")
//             }
//             owner {
//               address #@bsDecoder(fn: "decodeAddress")
//               id
//             }
//           }
//         }
//       }
//     }
//   |}
// ];

module LoadPatron = [%graphql
  {|
    query ($patronId: String!) {
      patron(id: $patronId) {
        id
        address @bsDecoder(fn: "decodeAddress")
        lastUpdated @bsDecoder(fn: "decodeBN")
        # lastUpdated @bsDecoder(fn: "decodeMoment")
        previouslyOwnedTokens {
          id
        }
        tokens {
          id
        }
        availableDeposit  @bsDecoder(fn: "decodePrice")
        patronTokenCostScaledNumerator  @bsDecoder(fn: "decodeBN")
        foreclosureTime  @bsDecoder(fn: "decodeMoment")
      }
    }
  |}
];

// NOTE: we currently have two patron objects while the graph is in a half updated state. When the graph is refactored these 'patron' queries will be merged into one.
module LoadPatronNew = [%graphql
  {|
    query ($patronId: String!) {
      patronNew(id: $patronId) {
        id
        address @bsDecoder(fn: "decodeAddress")
        lastUpdated @bsDecoder(fn: "decodeBN")
        totalLoyaltyTokens @bsDecoder(fn: "decodeBN")
        totalLoyaltyTokensIncludingUnRedeemed @bsDecoder(fn: "decodeBN")
      }
    }
  |}
];
module LoadPatronNewNoDecode = [%graphql
  {|
    query ($patronId: String!) {
      patronNew(id: $patronId) {
        id
        address
        lastUpdated
        totalLoyaltyTokens
        totalLoyaltyTokensIncludingUnRedeemed
      }
    }
  |}
];

module LoadTopContributors = [%graphql
  {|
    query ($numberOfLeaders: Int!) {
      patrons(first: $numberOfLeaders, orderBy: patronTokenCostScaledNumerator, orderDirection: desc) {
        id
        patronTokenCostScaledNumerator  @bsDecoder(fn: "decodeBN")
      }
    }
  |}
];

module SubTotalRaisedOrDueQuery = [%graphql
  {|
    query {
      global(id: 1) {
        id
        totalCollectedOrDueAccurate @bsDecoder(fn: "decodeBN")
        timeLastCollected @bsDecoder(fn: "decodeBN")
        totalTokenCostScaledNumeratorAccurate @bsDecoder(fn: "decodeBN")
      }
    }
  |}
];

type graphqlDataLoad('a) =
  | Loading
  | Error(ReasonApolloHooks.ApolloHooksTypes.apolloError)
  | NoData
  | Data('a);

let subscriptionResultOptionMap = (result, mapping) =>
  switch (result) {
  | ApolloHooks.Subscription.Data(response) => response->mapping->Some
  | ApolloHooks.Subscription.Error(_)
  | ApolloHooks.Subscription.Loading
  | ApolloHooks.Subscription.NoData => None
  };
let subscriptionResultToOption = result =>
  subscriptionResultOptionMap(result, a => a);

let queryResultOptionMap = (result, mapping) =>
  switch (result) {
  | ApolloHooks.Query.Data(response) => response->mapping->Some
  | ApolloHooks.Query.Error(_)
  | ApolloHooks.Query.Loading
  | ApolloHooks.Query.NoData => None
  };
let queryResultOptionFlatMap = (result, mapping) =>
  switch (result) {
  | ApolloHooks.Query.Data(response) => response->mapping
  | ApolloHooks.Query.Error(_)
  | ApolloHooks.Query.Loading
  | ApolloHooks.Query.NoData => None
  };
let queryResultToOption = result => queryResultOptionMap(result, a => a);

type data = {tokenId: string};

let useWildcardQuery = animal =>
  ApolloHooks.useQuery(
    ~variables=
      SubWildcardQuery.make(~tokenId=Animal.getId(animal), ())##variables,
    SubWildcardQuery.definition,
  );
// let useBuySubscription = () =>
//   ApolloHooks.useSubscription(
//     ~variables=SubBuyEvents.make()##variables,
//     SubBuyEvents.definition,
//   );
let useStateChangeSubscription = () =>
  ApolloHooks.useSubscription(
    ~variables=SubStateChangeEvents.make()##variables,
    SubStateChangeEvents.definition,
  );
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

let useLoadTopContributors = numberOfLeaders =>
  ApolloHooks.useSubscription(
    ~variables=LoadTopContributors.make(~numberOfLeaders, ())##variables,
    LoadTopContributors.definition,
  );
let useLoadTopContributorsData = numberOfLeaders => {
  let (simple, _) = useLoadTopContributors(numberOfLeaders);

  let getLargestContributors = largestContributors => {
    let monthlyContributions =
      largestContributors##patrons
      |> Js.Array.map(patron => {
           let monthlyContribution =
             patron##patronTokenCostScaledNumerator
             ->BN.mulGet(. BN.new_("2592000")) // A month with 30 days has 2592000 seconds
             ->BN.divGet(.
                 // BN.new_("1000000000000")->BN.mulGet(. BN.new_("31536000")),
                 BN.new_("31536000000000000000"),
               )
             ->Web3Utils.fromWeiBNToEthPrecision(~digits=4);
           (patron##id, monthlyContribution);
         });
    monthlyContributions;
  };

  simple->subscriptionResultOptionMap(getLargestContributors);
};

let usePatron: Animal.t => option(string) =
  animal => {
    let (simple, _) = useWildcardQuery(animal);
    let getAddress = response =>
      response##wildcard
      ->Belt.Option.flatMap(wildcard => Some(wildcard##owner##address));

    simple->queryResultOptionFlatMap(getAddress);
  };

let useIsAnimalOwened = ownedAnimal => {
  let currentAccount =
    RootProvider.useCurrentUser()
    ->Belt.Option.mapWithDefault("loading", a => a);

  let currentPatron =
    usePatron(ownedAnimal)
    ->Belt.Option.mapWithDefault("no-patron-defined", a => a);

  currentAccount->Js.String.toLowerCase
  == currentPatron->Js.String.toLocaleLowerCase;
};

let useTimeAcquired: Animal.t => option(MomentRe.Moment.t) =
  animal => {
    let (simple, _) = useWildcardQuery(animal);
    let getTimeAquired = response =>
      response##wildcard
      ->Belt.Option.mapWithDefault(MomentRe.momentNow(), wildcard
          // wildcard
          => wildcard##timeAcquired);

    simple->queryResultOptionMap(getTimeAquired);
  };

let useQueryPatron = patron =>
  ApolloHooks.useQuery(
    ~variables=LoadPatron.make(~patronId=patron, ())##variables,
    LoadPatron.definition,
  );
let useQueryPatronNew = patron =>
  ApolloHooks.useQuery(
    ~variables=LoadPatronNew.make(~patronId=patron, ())##variables,
    LoadPatronNew.definition,
  );

let useForeclosureTime: string => option(MomentRe.Moment.t) =
  patron => {
    let (simple, _) = useQueryPatron(patron);
    let getForclosureTime = response =>
      response##patron->Belt.Option.map(patron => patron##foreclosureTime);

    simple->queryResultOptionFlatMap(getForclosureTime);
  };
let usePatronQuery = patron => {
  let (simple, _) = useQueryPatron(patron);

  simple->queryResultToOption;
};
let useTimeAcquiredWithDefault = (animal, default: MomentRe.Moment.t) =>
  useTimeAcquired(animal) |||| default;

let useDaysHeld = animal =>
  useTimeAcquired(animal)
  ->oMap(moment =>
      (MomentRe.diff(MomentRe.momentNow(), moment, `days), moment)
    );

let useTotalCollectedOrDue: unit => option((BN.bn, BN.bn, BN.bn)) =
  () => {
    let (simple, _) =
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

    simple->queryResultOptionFlatMap(getTotalCollected);
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
      let timeElapsed =
        BN.new_(currentTimestamp)->BN.subGet(. timeCalculated);

      let amountRaisedSinceLastCollection =
        totalTokenCostScaledNumeratorAccurate
        ->BN.mulGet(. timeElapsed)
        ->BN.divGet(.
            // BN.new_("1000000000000")->BN.mulGet(. BN.new_("31536000")),
            BN.new_("31536000000000000000"),
          );
      amountCollectedOrDue->BN.addGet(. amountRaisedSinceLastCollection);
    });
};

let useTotalCollectedToken: Animal.t => option((Eth.t, BN.bn, BN.bn)) =
  animal => {
    let (simple, _) = useWildcardQuery(animal);
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
type patronLoyaltyTokenDetails = {
  currentLoyaltyTokens: Eth.t,
  currentLoyaltyTokensIncludingUnredeemed: Eth.t,
  lastCollected: BN.bn,
  numberOfAnimalsOwned: BN.bn,
};
let usePatronLoyaltyTokenDetails:
  Web3.ethAddress => option(patronLoyaltyTokenDetails) =
  address => {
    // NOTE: we are using both 'new patron' and 'patron' because the work on the graph is incomplete.
    let (responseNewPatron, _) = useQueryPatronNew(address);
    let (response, _) = useQueryPatron(address);

    switch (responseNewPatron, response) {
    | (Data(dataNewPatron), Data(dataPatron)) =>
      switch (dataNewPatron##patronNew, dataPatron##patron) {
      | (Some(newPatron), Some(patron)) =>
        Some({
          currentLoyaltyTokens: newPatron##totalLoyaltyTokens,
          currentLoyaltyTokensIncludingUnredeemed:
            newPatron##totalLoyaltyTokensIncludingUnRedeemed,
          lastCollected: newPatron##lastUpdated,
          numberOfAnimalsOwned:
            BN.new_(patron##tokens->Obj.magic->Array.length->string_of_int),
        })
      | _ => None
      }
    // | Loading
    // | Error(_error)
    // | NoData => None
    | (_, _) => None
    };
  };

// TODO:: Take min of total deposit and amount raised
let useAmountRaisedToken: Animal.t => option(Eth.t) =
  animal => {
    let currentTimestamp = useCurrentTime();

    switch (useTotalCollectedToken(animal)) {
    | Some((
        amountCollectedOrDue,
        timeCalculated,
        patronageNumeratorPriceScaled,
      )) =>
      let timeElapsed =
        BN.new_(currentTimestamp)->BN.subGet(. timeCalculated);

      let amountRaisedSinceLastCollection =
        patronageNumeratorPriceScaled
        ->BN.mulGet(. timeElapsed)
        ->BN.divGet(.
            // BN.new_("1000000000000")->BN.mulGet(. BN.new_("31536000")),
            BN.new_("31536000000000000000"),
          );
      Some(
        amountCollectedOrDue->BN.addGet(. amountRaisedSinceLastCollection),
      );
    | None => None
    };
  };

let useTimeSinceTokenWasLastSettled: Animal.t => option(BN.bn) =
  animal => {
    let currentTimestamp = useCurrentTime();

    switch (useTotalCollectedToken(animal)) {
    | Some((_, timeCalculated, _)) =>
      let timeElapsed =
        BN.new_(currentTimestamp)->BN.subGet(. timeCalculated);

      Some(timeElapsed);
    | None => None
    };
  };

let useUnredeemedLoyaltyTokenDueFromWildcard: Animal.t => option(Eth.t) =
  animal => {
    switch (useTimeSinceTokenWasLastSettled(animal)) {
    | Some(timeSinceTokenWasLastSettled) =>
      let totalLoyaltyTokensPerSecondPerAnimal = BN.new_("11574074074074");
      let totalLoyaltyTokensForAllAnimals =
        timeSinceTokenWasLastSettled |*| totalLoyaltyTokensPerSecondPerAnimal;
      Some(totalLoyaltyTokensForAllAnimals);
    | None => None
    };
  };

let useTotalLoyaltyToken: Web3.ethAddress => option((Eth.t, Eth.t)) =
  patron => {
    let currentTimestamp = useCurrentTime();

    switch (usePatronLoyaltyTokenDetails(patron)) {
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

let useRemainingDeposit: string => option((Eth.t, BN.bn, BN.bn)) =
  patron => {
    let (simple, _) = useQueryPatron(patron);

    let getRemainingDepositData = response =>
      response##patron
      ->oMap(wc =>
          (
            wc##availableDeposit,
            wc##lastUpdated,
            wc##patronTokenCostScaledNumerator,
          )
        );

    simple->queryResultOptionFlatMap(getRemainingDepositData);
  };

// TODO:: Take min of total deposit and amount raised
let useRemainingDepositEth: string => option(Eth.t) =
  patron => {
    let currentTimestamp = useCurrentTime();

    switch (useRemainingDeposit(patron)) {
    | Some((availableDeposit, lastUpdated, patronTokenCostScaledNumerator)) =>
      let timeElapsed = BN.new_(currentTimestamp)->BN.subGet(. lastUpdated);

      let amountRaisedSinceLastCollection =
        patronTokenCostScaledNumerator
        ->BN.mulGet(. timeElapsed)
        ->BN.divGet(.
            // BN.new_("1000000000000")->BN.mulGet(. BN.new_("31536000")),
            BN.new_("31536000000000000000"),
          );
      Some(availableDeposit->BN.subGet(. amountRaisedSinceLastCollection));
    | None => None
    };
  };

type animalPrice =
  | Foreclosed
  | Price(Eth.t)
  | Loading;
let usePrice: Animal.t => animalPrice =
  animal => {
    let (simple, _) = useWildcardQuery(animal);
    let currentPatron =
      usePatron(animal)
      ->Belt.Option.mapWithDefault("no-patron-defined", a => a);
    let availableDeposit = useRemainingDepositEth(currentPatron);

    switch (simple) {
    | Data(response) =>
      let priceValue =
        response##wildcard
        ->Belt.Option.mapWithDefault(Eth.makeFromInt(0), wildcard =>
            wildcard##price##price
          );

      switch (availableDeposit) {
      | Some(deposit) =>
        if (deposit->BN.gtGet(. BN.new_("0"))) {
          Price(priceValue);
        } else {
          Foreclosed;
        }
      | None => Foreclosed // I'm not sure if this is the correct thing to put here... If the availableDeposit is undefined, it could mean the token belongs to the steward and is foreclosed, or it could mean it
      };
    | _ => Loading
    };
  };

let useIsForeclosed: Web3.ethAddress => bool =
  currentPatron => {
    let optAvailableDeposit = useRemainingDepositEth(currentPatron);

    optAvailableDeposit->Option.mapWithDefault(true, availableDeposit =>
      availableDeposit->BN.ltGet(. BN.new_("0"))
    );
  };
