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
module SubBuyEvents = [%graphql
  {|
    subscription {
      eventCounter(id: 1) {
        id
        buyEventCount
          buyEvents(first: 5, orderBy: timestamp, orderDirection: desc) {
          id
          token {
            id
            # NOTE: no need to decode these values, this is only for updating the cache.
            animal: tokenId #@bsDecoder(fn: "tokenIdToAnimal")
            timeAcquired #@bsDecoder(fn: "decodeMoment")
            totalCollected #@bsDecoder(fn: "decodePrice")
            patronageNumeratorPriceScaled #@bsDecoder(fn: "decodeBN")
            timeCollected #@bsDecoder(fn: "decodeBN")
            # timeCollected @bsDecoder(fn: "decodeMoment")
            price {
              id
              price #@bsDecoder(fn: "decodePrice")
            }
            owner {
              address #@bsDecoder(fn: "decodeAddress")
              id
            }
          }
        }
      }
    }
  |}
];

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

module LoadPatronNew = [%graphql
  {|
    query ($patronId: String!) {
      patronNew(id: $patronId) {
        id
        address @bsDecoder(fn: "decodeAddress")
        lastUpdated @bsDecoder(fn: "decodeBN")
        totalLoyaltyTokens @bsDecoder(fn: "decodeBN")
        totalLoyaltyTokensIncludingUnRedeemed @bsDecoder(fn: "decodeBN")
        tokens {
          id
        }
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

[@bs.deriving abstract]
type data = {tokenId: string};

let useWildcardQuery = animal =>
  ApolloHooks.useQuery(
    ~variables=
      SubWildcardQuery.make(~tokenId=Animal.getId(animal), ())##variables,
    SubWildcardQuery.definition,
  );
let useBuySubscription = () =>
  ApolloHooks.useSubscription(
    ~variables=SubBuyEvents.make()##variables,
    SubBuyEvents.definition,
  );
let useBuySubscriptionData = () => {
  let (simple, _) = useBuySubscription();
  switch (simple) {
  | Data(response) => Some(response)
  | _ => None
  };
};

let useLoadTopContributors = numberOfLeaders =>
  ApolloHooks.useSubscription(
    ~variables=LoadTopContributors.make(~numberOfLeaders, ())##variables,
    LoadTopContributors.definition,
  );
let useLoadTopContributorsData = numberOfLeaders => {
  let (simple, _) = useLoadTopContributors(numberOfLeaders);
  switch (simple) {
  | Data(largestContributors) =>
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
    Some(monthlyContributions);
  | _ => None
  };
};

let usePatron: Animal.t => option(string) =
  animal => {
    let (simple, _) = useWildcardQuery(animal);
    switch (simple) {
    | Data(response) =>
      response##wildcard
      ->Belt.Option.flatMap(wildcard => Some(wildcard##owner##address))
    | _ => None
    };
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

let useTimeAcquired: Animal.t => graphqlDataLoad(MomentRe.Moment.t) =
  animal => {
    let (simple, _) = useWildcardQuery(animal);
    switch (simple) {
    | Loading => Loading
    | Error(error) => Error(error)
    | NoData => Loading
    | Data(response) =>
      Data(
        response##wildcard
        ->Belt.Option.mapWithDefault(MomentRe.momentNow(), wildcard
            // wildcard
            => wildcard##timeAcquired),
      )
    };
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
    switch (simple) {
    | Data(response) =>
      response##patron
      ->Belt.Option.flatMap(patron => Some(patron##foreclosureTime))
    | _ => None
    };
  };
let usePatronQuery = patron => {
  let (simple, _) = useQueryPatron(patron);
  switch (simple) {
  | Data(response) => Some(response)
  | _ => None
  };
};
let useTimeAcquiredWithDefault = (animal, default: MomentRe.Moment.t) => {
  switch (useTimeAcquired(animal)) {
  | Data(moment) => moment
  | _ => default
  };
};

let useDaysHeld = animal =>
  switch (useTimeAcquired(animal)) {
  | Data(moment) =>
    Some((MomentRe.diff(MomentRe.momentNow(), moment, `days), moment))
  | _ => None
  };
let useTotalCollectedOrDue: unit => graphqlDataLoad((BN.bn, BN.bn, BN.bn)) =
  () => {
    let (simple, _) =
      ApolloHooks.useQuery(SubTotalRaisedOrDueQuery.definition);

    switch (simple) {
    | Loading => Loading
    | Error(error) => Error(error)
    | NoData => Loading
    | Data(response) =>
      response##global
      ->Belt.Option.mapWithDefault(Loading, global =>
          Data((
            global##totalCollectedOrDueAccurate,
            global##timeLastCollected,
            global##totalTokenCostScaledNumeratorAccurate,
          ))
        )
    };
  };

let getCurrentTimestamp = () =>
  string_of_int(Js.Math.floor(Js.Date.now() /. 1000.));

let useCurrentTime = () => {
  let (currentTime, setTimeLeft) =
    React.useState(() => getCurrentTimestamp());

  React.useEffect0(() => {
    let interval =
      Js.Global.setInterval(
        () => {setTimeLeft(_ => {getCurrentTimestamp()})},
        2000,
      );
    Some(() => Js.Global.clearInterval(interval));
  });
  currentTime;
};

let useAmountRaised = () => {
  let currentTimestamp = useCurrentTime();

  switch (useTotalCollectedOrDue()) {
  | Data((
      amountCollectedOrDue,
      timeCalculated,
      totalTokenCostScaledNumeratorAccurate,
    )) =>
    let timeElapsed = BN.new_(currentTimestamp)->BN.subGet(. timeCalculated);

    let amountRaisedSinceLastCollection =
      totalTokenCostScaledNumeratorAccurate
      ->BN.mulGet(. timeElapsed)
      ->BN.divGet(.
          // BN.new_("1000000000000")->BN.mulGet(. BN.new_("31536000")),
          BN.new_("31536000000000000000"),
        );
    Some(amountCollectedOrDue->BN.addGet(. amountRaisedSinceLastCollection));
  | _ => None
  };
};

let useTotalCollectedToken: Animal.t => option((Eth.t, BN.bn, BN.bn)) =
  animal => {
    let (simple, _) = useWildcardQuery(animal);

    switch (simple) {
    | Data(response) =>
      response##wildcard
      ->Belt.Option.flatMap(wc =>
          Some((
            wc##totalCollected,
            wc##timeCollected,
            wc##patronageNumeratorPriceScaled,
          ))
        )
    // | Loading
    // | Error(_error)
    // | NoData => None
    | _ => None
    };
  };
type patronLoyaltyTokenDetails = {
  currentLoyaltyTokens: Eth.t,
  lastCollected: BN.bn,
  numberOfAnimalsOwned: BN.bn,
};
let usePatronLoyaltyTokenDetails:
  Web3.ethAddress => option(patronLoyaltyTokenDetails) =
  address => {
    let (response, _) = useQueryPatronNew(address);

    switch (response) {
    | Data(responseData) =>
      responseData##patronNew
      ->Belt.Option.flatMap(patron =>
          Some({
            currentLoyaltyTokens: BN.new_("123"),
            lastCollected: patron##lastUpdated,
            numberOfAnimalsOwned:
              BN.new_(patron##tokens->Array.length->string_of_int),
          })
        )

    // | Loading
    // | Error(_error)
    // | NoData => None
    | _ => None
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

let useTotalLoyaltyToken: Web3.ethAddress => option(Eth.t) =
  patron => {
    let currentTimestamp = useCurrentTime();

    switch (usePatronLoyaltyTokenDetails(patron)) {
    | Some({currentLoyaltyTokens, lastCollected, numberOfAnimalsOwned}) =>
      let timeElapsed = BN.new_(currentTimestamp) |-| lastCollected;
      // Reference: https://github.com/wild-cards/contracts-private/blob/v2testing/migrations/7_receipt_tokens.js#L6
      let totalLoyaltyTokensPerSecondPerAnimal = BN.new_("11574074074074");
      let totalLoyaltyTokensFor1Animal =
        totalLoyaltyTokensPerSecondPerAnimal |*| timeElapsed;
      let totalLoyaltyTokensForAllAnimal =
        numberOfAnimalsOwned |*| totalLoyaltyTokensFor1Animal;
      let totalLoyaltyTokensForUser =
        currentLoyaltyTokens |+| totalLoyaltyTokensForAllAnimal;
      Some(totalLoyaltyTokensForUser);
    | None => None
    };
  };

let useRemainingDeposit: string => option((Eth.t, BN.bn, BN.bn)) =
  patron => {
    let (simple, _) = useQueryPatron(patron);

    switch (simple) {
    | Data(response) =>
      response##patron
      ->Belt.Option.flatMap(wc =>
          Some((
            wc##availableDeposit,
            wc##lastUpdated,
            wc##patronTokenCostScaledNumerator,
          ))
        )
    | _ => None
    };
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