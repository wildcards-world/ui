// TODO: remove the "NoGorilla" option, it makes the code messy for no reason. Rather use an `option` if there might not be a gorilla.
type gorilla =
  | Andy
  | Vitalik
  | Simon
  | NoGorilla;

let getId: gorilla => option(string) =
  aGorilla =>
    switch (aGorilla) {
    | Simon => Some("0")
    | Andy => Some("1")
    // | Vitalik => "42",
    | _ => None
    };

let getNameFromId: option(string) => string =
  gorillaId =>
    switch (gorillaId) {
    | None => "Vitalik"
    | Some(tokenIdSet) =>
      switch (tokenIdSet) {
      | "0" => "Simon"
      | "1" => "Andy"
      | _ => "Unknown"
      }
    };

let getName: gorilla => string =
  gorillaId =>
    switch (gorillaId) {
    | Vitalik => "Vitalik"
    | Simon => "Simon"
    | Andy => "Andy"
    | _ => "Unknown"
    };

let getGorilla: string => gorilla =
  gorillaName => {
    let gorillaNameLower = Js.String.toLowerCase(gorillaName);
    switch (gorillaNameLower) {
    | "vitalik" => Vitalik
    | "simon" => Simon
    | "andy" => Andy
    | _ => NoGorilla
    };
  };

let getNextPrevStr = gorilla =>
  switch (gorilla) {
  | Vitalik => ("andy", "simon")
  | Simon => ("vitalik", "andy")
  | Andy => ("simon", "vitalik")
  | _ => ("vitalik", "simon")
  };

let getImage = gorilla =>
  switch (gorilla) {
  | Vitalik =>
    %bs.raw
    {|require('../../img/gorillas/gorilla1.png')|}
  | Simon =>
    %bs.raw
    {|require('../../img/gorillas/gorilla2.png')|}
  | Andy =>
    %bs.raw
    {|require('../../img/gorillas/gorilla3.png')|}
  | _ =>
    %bs.raw
    {|require('../../img/gorillas/gorilla3.png')|}
  };

let useBuy = gorilla => {
  open Hooks;

  let gorillaId = getId(gorilla);
  switch (gorillaId) {
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

let useCurrentPrice = gorilla => {
  open Hooks;
  open Accounting;

  let gorillaId = getId(gorilla);
  switch (gorillaId) {
  | None => useCurrentPriceWei()->defaultZeroS
  | Some(tokenIdSet) => useCurrentPriceWeiNew(tokenIdSet)->defaultZeroS
  };
};

// TODO: you could get this from the contract, but seems like a waste
let pledgeRate = gorilla => {
  open Hooks;
  open Accounting;

  let gorillaId = getId(gorilla);

  switch (gorillaId) {
  | None => ("3", "10", 0.025, 40.)
  | Some(tokenIdSet) => ("24", "10", 0.2, 5.)
  };
};

let useCurrentPriceEth = gorilla => {
  open Belt.Option;
  let gorillaId = getId(gorilla);
  (
    switch (gorillaId) {
    | None => Hooks.useCurrentPriceEth()
    | Some(tokenIdSet) => Hooks.useCurrentPriceEthNew(tokenIdSet)
    }
  )
  ->mapWithDefault("loading", a => a);
};
let useCurrentPriceUsd = gorilla => {
  open Belt.Option;
  let gorillaId = getId(gorilla);
  (
    switch (gorillaId) {
    | None => Hooks.useCurrentPriceUsd()
    | Some(tokenIdSet) => Hooks.useCurrentPriceUsdNew(tokenIdSet)
    }
  )
  ->mapWithDefault("loading", a => a);
};
