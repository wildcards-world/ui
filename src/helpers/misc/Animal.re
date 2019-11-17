// TODO: remove the "NoAnimal" option, it makes the code messy for no reason. Rather use an `option` if there might not be a animal.
type t =
  | Andy
  | Vitalik
  | Simon
  | NoAnimal;

let getId: t => option(string) =
  anAnimal =>
    switch (anAnimal) {
    | Simon => Some("0")
    | Andy => Some("1")
    // | Vitalik => "42",
    | _ => None
    };

let getTokenId: t => option(TokenId.t) =
  anAnimal =>
    switch (anAnimal) {
    | Simon => Some(TokenId.makeFromInt(0))
    | Andy => Some(TokenId.makeFromInt(1))
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
    | _ => "Unknown"
    };

let getAnimal: string => t =
  animalName => {
    let animalNameLower = Js.String.toLowerCase(animalName);
    switch (animalNameLower) {
    | "vitalik" => Vitalik
    | "simon" => Simon
    | "andy" => Andy
    | _ => NoAnimal
    };
  };

let getNextPrevStr = animal =>
  switch (animal) {
  | Vitalik => ("andy", "simon")
  | Simon => ("vitalik", "andy")
  | Andy => ("simon", "vitalik")
  | _ => ("vitalik", "simon")
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
