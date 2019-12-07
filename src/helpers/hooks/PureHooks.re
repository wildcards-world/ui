[@bs.deriving {abstract: light}]
type drizzle = {web3: Web3.t};

[@bs.deriving {abstract: light}]
type drizzleContext = {
  drizzle,
  useCacheCall: (. string, string, TokenId.t) => Js.Nullable.t(string),
};

[@bs.deriving {abstract: light}]
type drizzleState = {accounts: array(string)};

[@bs.module "@drizzle/react-plugin"] [@bs.scope "drizzleReactHooks"]
external useDrizzleState: ([@bs.uncurry] (drizzleState => 'a)) => 'a =
  "useDrizzleState";

[@bs.module "@drizzle/react-plugin"] [@bs.scope "drizzleReactHooks"]
external useDrizzle: unit => drizzleContext = "useDrizzle";

let useCurrentUser: unit => option(string) =
  () => {
    let state = useDrizzleState(state => state->accounts);
    state->Belt.Array.get(0);
  };

let useWeb3: unit => Web3.t =
  () => {
    useDrizzle()->drizzle->web3;
  };
let useCacheCall = (method, id) =>
  useDrizzle()->useCacheCall(. "WildcardSteward_v0", method, id);

// let useCacheCallVitalikStr = method =>
//   Js.Nullable.fromOption(Some("result"));
// useDrizzle()->useCacheCallVitalikStr(. "VitalikSteward", method);

// let useTotalPatronageWei = id => {
//   let totalCollectedOpt =
//     "totalCollected"->useCacheCall(id)->Js.Nullable.toOption;
//   let patronageOwedOpt =
//     "patronageOwed"->useCacheCall(id)->Js.Nullable.toOption;

//   switch (totalCollectedOpt, patronageOwedOpt) {
//   | (Some(totalCollected), Some(patronageOwed)) =>
//     let totalCollectedBN: BN.bn = BN.new_(totalCollected);
//     let patronageOwedBN: BN.bn = BN.new_(patronageOwed);
//     Some(totalCollectedBN->BN.addGet(. patronageOwedBN)->BN.toStringGet(.));
//   | _ => None
//   };
// };

let useTimeAcquired = id => {
  let timeAquiredRaw = "timeAcquired"->useCacheCall(id)->Js.Nullable.toOption;

  timeAquiredRaw->Belt.Option.flatMap(timeStamp =>
    Some(MomentRe.momentWithUnix(int_of_string(timeStamp)))
  );
};

let useTimeAcquiredAnimal: Animal.t => option(MomentRe.Moment.t) =
  gorilla => {
    switch (Animal.getTokenId(gorilla)) {
    | Some(gorillaId) => gorillaId->useTimeAcquired
    | None => "1"->useTimeAcquired
    };
  };
