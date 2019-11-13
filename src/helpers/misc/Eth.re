type t = BN.bn;

[@bs.module "web3-utils"]
external get:
  (
    t,
    [@bs.string] [
      | `wei
      | `kwei
      | `mwei
      | `gwei
      | `microether
      | `milliether
      | `ether
      | `kether
      | `mether
      | `gether
      | `tether
    ]
  ) =>
  string =
  "fromWei";
[@bs.module "web3-utils"] external toWei: (string, string) => string = "toWei";

let getEth: t => string = value => get(value, `ether);

let make: string => option(t) =
  wei => Helper.isStringInteger(wei) ? Some(BN.new_(wei)) : None;
let makeWithDefault: (string, int) => t =
  (tokenId, default) =>
    switch (make(tokenId)) {
    | Some(wei) => wei
    | None => default->Belt.Int.toString->BN.new_
    };
let makeFromInt: int => t = tokenId => tokenId->Belt.Int.toString->BN.new_;

let makeFromEthStr: string => option(t) =
  eth =>
    Belt.Float.fromString(eth)
    ->Belt.Option.flatMap(ethFloat =>
        Some(BN.new_(toWei(Belt.Float.toString(ethFloat), "ether")))
      );
