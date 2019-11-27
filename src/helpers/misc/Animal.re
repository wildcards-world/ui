// TODO: remove the "NoAnimal" option, it makes the code messy for no reason. Rather use an `option` if there might not be a animal.
type t =
  | Andy
  | Vitalik
  | Verano
  | Tarkus
  | Pancho
  | Mijungla
  | Llajuita
  | Espumita
  | Cubai
  | CatStevens
  | Aruma
  | Apthapi
  | Ajayu
  | Simon;

let getId: t => option(string) =
  anAnimal =>
    switch (anAnimal) {
    | Simon => Some("0")
    | Andy => Some("1")
    | Verano => Some("2")
    | Tarkus => Some("3")
    | Pancho => Some("4")
    | Mijungla => Some("5")
    | Llajuita => Some("6")
    | Espumita => Some("7")
    | Cubai => Some("8")
    | CatStevens => Some("9")
    | Aruma => Some("10")
    | Apthapi => Some("11")
    | Ajayu => Some("12")
    // | Vitalik => Some("42") // We don't show an ID for vitalik since using legacy contract still.
    | _ => None
    };

let getTokenId: t => option(TokenId.t) =
  anAnimal =>
    switch (anAnimal) {
    | Simon => Some(TokenId.makeFromInt(0))
    | Andy => Some(TokenId.makeFromInt(1))
    | Verano => Some(TokenId.makeFromInt(2))
    | Tarkus => Some(TokenId.makeFromInt(3))
    | Pancho => Some(TokenId.makeFromInt(4))
    | Mijungla => Some(TokenId.makeFromInt(5))
    | Llajuita => Some(TokenId.makeFromInt(6))
    | Espumita => Some(TokenId.makeFromInt(7))
    | Cubai => Some(TokenId.makeFromInt(8))
    | CatStevens => Some(TokenId.makeFromInt(9))
    | Aruma => Some(TokenId.makeFromInt(10))
    | Apthapi => Some(TokenId.makeFromInt(11))
    | Ajayu => Some(TokenId.makeFromInt(12))
    // | Vitalik => "42",
    | _ => None
    };

let getNameFromId: option(string) => string =
  animalId =>
    switch (animalId) {
    | None => "Vitalik"
    | Some(tokenIdSet) =>
      switch (tokenIdSet) {
      | "0" => "Simon"
      | "1" => "Andy"
      | _ => "Unknown"
      }
    };

let getName: t => string =
  animalId =>
    switch (animalId) {
    | Vitalik => "Vitalik"
    | Simon => "Simon"
    | Andy => "Andy"
    | Verano => "Verano"
    | Tarkus => "Tarkus"
    | _ => "add name"
    };

let getAnimal: string => option(t) =
  animalName => {
    let animalNameLower = Js.String.toLowerCase(animalName);
    switch (animalNameLower) {
    | "vitalik" => Some(Vitalik)
    | "simon" => Some(Simon)
    | "andy" => Some(Andy)
    | _ => None
    };
  };

let getNextPrev = animal =>
  switch (animal) {
  | Vitalik => (Andy, Simon)
  | Simon => (Vitalik, Andy)
  | Andy => (Simon, Vitalik)
  | _ => (Simon, Vitalik)
  };

let getImage = animal =>
  switch (animal) {
  | Vitalik =>
    %bs.raw
    {|require('../../img/animals/gorilla1.png')|}
  | Simon =>
    %bs.raw
    {|require('../../img/animals/gorilla2.png')|}
  | Andy =>
    %bs.raw
    {|require('../../img/animals/gorilla3.png')|}
  | _ =>
    %bs.raw
    {|require('../../img/animals/gorilla3.png')|}
  };

let useBuy = animal => {
  open Hooks;

  let animalId = getId(animal);
  switch (animalId) {
  | None =>
    let buyObj = useBuyTransaction();
    (
      (
        (newPrice, txObject) =>
          buyObj##send(. newPrice->Web3Utils.toWeiFromEth, txObject)
      ),
      buyObj##_TXObjects,
    );
  | Some(tokenIdSet) =>
    let buyObj = useBuyTransactionNew();
    (
      (
        (newPrice, txObject) =>
          buyObj##send(.
            tokenIdSet,
            newPrice->Web3Utils.toWeiFromEth,
            txObject,
          )
      ),
      buyObj##_TXObjects,
    );
  };
};

let useCurrentPrice = animal => {
  open Hooks;
  open Accounting;

  let animalId = getId(animal);
  switch (animalId) {
  | None => useCurrentPriceWei()->defaultZeroS
  | Some(tokenIdSet) => useCurrentPriceWeiNew(tokenIdSet)->defaultZeroS
  };
};

// TODO: you could get this from the contract, but seems like a waste
let pledgeRate = animal => {
  let animalId = getId(animal);

  switch (animalId) {
  | None => ("3", "10", 0.025, 40.)
  | Some(_tokenIdSet) => ("24", "10", 0.2, 5.)
  };
};

let useCurrentPriceEth = animal => {
  open Belt.Option;
  let animalId = getId(animal);
  (
    switch (animalId) {
    | None => Hooks.useCurrentPriceEth()
    | Some(tokenIdSet) => Hooks.useCurrentPriceEthNew(tokenIdSet)
    }
  )
  ->mapWithDefault("loading", a => a);
};
let useCurrentPriceUsd = animal => {
  open Belt.Option;
  let animalId = getId(animal);
  (
    switch (animalId) {
    | None => Hooks.useCurrentPriceUsd()
    | Some(tokenIdSet) => Hooks.useCurrentPriceUsdNew(tokenIdSet)
    }
  )
  ->mapWithDefault("loading", a => a);
};

let isLaunched: t => bool =
  anAnimal =>
    switch (anAnimal) {
    | Simon => true
    | Andy => true
    | Vitalik => true
    | _ => false
    };

let useIsAnimalOwened = animal => {
  let animalId = animal->getId;
  let currentAccount =
    Hooks.useCurrentUser()->Belt.Option.mapWithDefault("loading", a => a);
  let currentPatron =
    switch (animalId) {
    | Some(id) =>
      Hooks.useCurrentPatronNew(id->int_of_string)
      ->Belt.Option.mapWithDefault("no-patron-defined", a => a)

    | None =>
      Hooks.useCurrentPatron()
      ->Belt.Option.mapWithDefault("no-patron-defined", a => a)
    };
  currentAccount == currentPatron;
};
