open ReasonApolloHooks;
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
      wildcards(first: 13) {
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

// module InitialLoadQuery = ReasonApolloHooks.ApolloHooksQuery(InitialLoad);
let useIsInitialized = () => {
  // let (simple, _full) = InitialLoadQuery.use();
  let (simple, _full) =
    ApolloHooks.useQuery(
      ~notifyOnNetworkStatusChange=true,
      InitialLoad.definition,
    );
  // InitialLoadQuery.use();

  switch (simple) {
  | Data(_data) =>
    // Js.log(data);
    true
  | Loading
  | NoData
  | Error(_) => false
  };
};

module SubWildcardQuery = [%graphql
  {|
    query ($tokenId: String!) {
      wildcard(id: $tokenId) {
        id
        timeAcquired @bsDecoder(fn: "decodeMoment")
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

module LoadPatron = [%graphql
  {|
    query ($patronId: String!) {
      patron(id: $patronId) {
        id
        address @bsDecoder(fn: "decodeAddress")
        lastUpdated @bsDecoder(fn: "decodeMoment")
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
  | Error(ReasonApolloHooks.ApolloHooksQuery.queryError)
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
let usePrice: Animal.t => option(Eth.t) =
  animal => {
    let (simple, _) = useWildcardQuery(animal);

    switch (simple) {
    | Data(response) =>
      Some(
        response##wildcard
        ->Belt.Option.mapWithDefault(Eth.makeFromInt(0), wildcard =>
            wildcard##price##price
          ),
      )
    | _ => None
    };
  };

let usePatron: Animal.t => option(string) =
  animal => {
    let (simple, _) = useWildcardQuery(animal);
    switch (simple) {
    | Data(response) =>
      Some(
        response##wildcard
        ->Belt.Option.mapWithDefault("Loading", wildcard =>
            wildcard##owner##address
          ),
      )
    | _ => None
    };
  };

let useIsAnimalOwened = ownedAnimal => {
  let currentAccount =
    Hooks.useCurrentUser()->Belt.Option.mapWithDefault("loading", a => a);

  let currentPatron =
    usePatron(ownedAnimal)
    ->Belt.Option.mapWithDefault("no-patron-defined", a => a);

  currentAccount == currentPatron;
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

let useRemainingDeposit: string => option(Eth.t) =
  patron => {
    let (simple, _) =
      ApolloHooks.useQuery(
        ~variables=LoadPatron.make(~patronId=patron, ())##variables,
        LoadPatron.definition,
      );
    switch (simple) {
    | Loading => None
    | Error(_error) => None
    | NoData => None
    | Data(response) =>
      response##patron
      ->Belt.Option.flatMap(patron => Some(patron##availableDeposit))
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
        () => {setTimeLeft(_ => getCurrentTimestamp())},
        2000,
      );
    Some(() => Js.Global.clearInterval(interval));
  });
  currentTime;
};

// (a1_price*a1numerator)+(a2_price*a2numerator)+ ....
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
          BN.new_("1000000000000")->BN.mulGet(. BN.new_("31536000")),
        );
    Some(amountCollectedOrDue->BN.addGet(. amountRaisedSinceLastCollection));
  | _ => None
  };
};
