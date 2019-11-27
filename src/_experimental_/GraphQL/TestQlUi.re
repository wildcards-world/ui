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

[@react.component]
let make = () => {
  <GetUserNameSubscription>
    ...{({result}) =>
      switch (result) {
      | Loading => <div> {ReasonReact.string("Loading")} </div>
      | Error(error) => <div> {error##message->ste} </div>
      | Data(response) =>
        <div>
          {switch (response##wildcards) {
           | posts =>
             posts
             |> Array.map(post =>
                  <div
                    className="bg-white rounded-t-lg overflow-hidden p-4 p-10 flex justify-center">
                    <div>
                      <p> "testing"->React.string </p>
                      {
                        let animalName = post##animal->Animal.getName;
                        let animalWeiPrice =
                          post##price##price->Eth.get(`wei);
                        let animalEthPrice =
                          post##price##price->Eth.get(`ether);
                        ReasonReact.string(
                          {j|$animalName Price(wei): $animalWeiPrice Price(eth): $animalEthPrice|j},
                        );
                      }
                    </div>
                  </div>
                )
             |> ReasonReact.array
           }}
        </div>
      }
    }
  </GetUserNameSubscription>;
};
