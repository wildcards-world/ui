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

module SubUserName = [%graphql
  {|
    subscription {
      wildcards(first: 5) {
        id
        animal: tokenId @bsDecoder(fn: "tokenIdToAnimal")
        patronageNumerator
        owner {
          address
        }
        price {
          price @bsDecoder(fn: "decodePrice")
        }
      }
    }
  |}
];

module SubPriceQuery = [%graphql
  {|
    subscription ($tokenId: String!) {
      wildcard(id: $tokenId) {
        price {
          price @bsDecoder(fn: "decodePrice")
        }
      }
    }
  |}
];

module SubPrice = ReasonApolloHooks.Subscription.Make(SubPriceQuery);

module UserQuery = ReasonApolloHooks.Subscription.Make(SubUserName);

type graphqlDataLoad('a) =
  | Loading
  | Error(ReasonApolloHooks.Subscription.error)
  | NoData
  | Data('a);

[@bs.deriving abstract]
type data = {tokenId: string};

let usePrice: Animal.t => graphqlDataLoad(Eth.t) =
  animal => {
    let (simple, _) =
      SubPrice.use(
        ~variables=
          SubPriceQuery.make(
            ~tokenId=
              Animal.getId(animal)->Belt.Option.mapWithDefault("42", a => a),
            (),
          )##variables,
        (),
      );

    switch (simple) {
    | Loading => Loading
    | Error(error) => Error(error)
    | NoData => Loading
    | Data(response) =>
      Data(
        response##wildcard
        ->Belt.Option.mapWithDefault(Eth.makeFromInt(0), wildcard =>
            wildcard##price##price
          ),
      )
    };
  };

let usePriceStringWithDefault = (animal, default: string, units) => {
  switch (usePrice(animal)) {
  | Data(price) =>
    Js.log(price);
    "default"; //price->Eth.get(units);
  | _ => default
  };
};
let useTimeAquired: Animal.t => graphqlDataLoad(Eth.t) =
  animal => {
    let (simple, _) =
      SubPrice.use(
        ~variables=
          SubPriceQuery.make(
            ~tokenId=
              Animal.getId(animal)->Belt.Option.mapWithDefault("42", a => a),
            (),
          )##variables,
        (),
      );

    switch (simple) {
    | Loading => Loading
    | Error(error) => Error(error)
    | NoData => Loading
    | Data(response) =>
      Data(
        response##wildcard
        ->Belt.Option.mapWithDefault(Eth.makeFromInt(0), wildcard =>
            wildcard##price##price
          ),
      )
    };
  };

let usePriceStringWithDefault = (animal, default: string, units) => {
  switch (usePrice(animal)) {
  | Data(price) =>
    Js.log(price);
    "default"; //price->Eth.get(units);
  | _ => default
  };
};

[@react.component]
let make = () => {
  <div>
    <p> "Price"->React.string </p>
    {ReasonReact.array(
       Array.mapi(
         (_index, animal) => {
           let animalId =
             Animal.getId(animal)->Belt.Option.mapWithDefault("42", a => a);
           let price0 =
             usePriceStringWithDefault(animal, "loading", Eth.Eth(`gwei));
           <p>
             {("price token #" ++ animalId ++ ": " ++ price0)->React.string}
           </p>;
         },
         Animal.orderedArray,
       ),
     )}
    <p> "Price USD"->React.string </p>
    {ReasonReact.array(
       Array.mapi(
         (_index, animal) => {
           let animalId =
             Animal.getId(animal)->Belt.Option.mapWithDefault("42", a => a);
           let price0 = usePriceStringWithDefault(animal, "loading", `gwei);
           <p>
             {("price token #" ++ animalId ++ ": " ++ price0)->React.string}
           </p>;
         },
         Animal.orderedArray,
       ),
     )}
  </div>;
};
