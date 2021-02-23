@ocaml.doc(`Adds a comma between groups of 3 decimals`)
let format = Js.String2.replaceByRe(_, %re("/\d(?=(\d{3})+\.)/g"), "$&,")

@ocaml.doc(`Formats a float to 2 digits precision with groups of 3 decimals separated by a comma`)
let formatFloat = (~digits=2, number) => number->Js.Float.toFixedWithPrecision(~digits)->format

@ocaml.doc(`Formats a string float to 2 digits precision with groups of 3 decimals separated by a comma`)
let toCentsFixedNoRounding = floatString => floatString->Js.Float.fromString->formatFloat

@ocaml.doc(`Formats a BigNumber (10^18, wei) to 2 digits precision (ether) with groups of 3 decimals separated by a comma`)
let formatEther = rawNumber => rawNumber->Ethers.Utils.formatEther->toCentsFixedNoRounding

// TODO: Move these to use Ethers!
// Float
let toFixedWithPrecisionNoTrailingZeros = (number: float, ~digits) =>
  number->Js.Float.toFixedWithPrecision(~digits)->float_of_string->Float.toString

let toFixedWithPrecisionNoTrailingZerosEth = (~digits=9, eth) =>
  eth
  ->Eth.fromWeiEth
  ->Float.fromString
  ->Option.getWithDefault(0.)
  ->toFixedWithPrecisionNoTrailingZeros(~digits)

// Placed here because of a dependency cycle in `Eth.res`
let getEthUnit = (value, unit) => {
  open Eth
  switch unit {
  | Eth(unit) => fromWei(value, unit->ethUnitToJs)
  | Usd(conversion, digits) =>
    (fromWei(value, #ether->ethUnitToJs)->Js.Float.fromString *. conversion)
      ->toFixedWithPrecisionNoTrailingZeros(~digits)
  }
}
