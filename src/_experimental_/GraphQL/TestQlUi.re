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

module GetUserNameSubscription = ReasonApollo.CreateSubscription(SubUserName);

let ste = ReasonReact.string;

module UserQuery = ReasonApolloHooks.Subscription.Make(SubUserName);

[@react.component]
let make = (~children) => {
  let (simple, _) = UserQuery.use();

  let state =
    switch (simple) {
    | Loading => GlobalContext.Loading
    | Error(error) => GlobalContext.Error(error)
    | NoData => GlobalContext.Loading
    | Data(response) => GlobalContext.Loaded("loadedd - from inside ;)")
    //  <div>
    //    {switch (response##wildcards) {
    //     | posts =>
    //       posts
    //       |> Array.map(post =>
    //            <div
    //              className="bg-white rounded-t-lg overflow-hidden p-4 p-10 flex justify-center">
    //              <div>
    //                <p> "testing boom"->React.string </p>
    //                {
    //                  let animalName = post##animal->Animal.getName;
    //                  let animalWeiPrice = post##price##price->Eth.get(`wei);
    //                  let animalEthPrice =
    //                    post##price##price->Eth.get(`ether);
    //                  ReasonReact.string(
    //                    {j|$animalName Price(wei): $animalWeiPrice Price(eth): $animalEthPrice|j},
    //                  );
    //                }
    //              </div>
    //            </div>
    //          )
    //       |> ReasonReact.array
    //     }}
    //  </div>
    };

  <GlobalContext value=state> children </GlobalContext>;
  // };
  // </GetUserNameSubscription>;
};
