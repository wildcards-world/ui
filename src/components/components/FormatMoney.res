@ocaml.doc(`Adds a comma between groups of 3 decimals`)
let format = Js.String2.replaceByRe(_, %re("/\d(?=(\d{3})+\.)/g"), "$&,")

@ocaml.doc(`Formats a float to 2 digits precision with groups of 3 decimals separated by a comma`)
let formatFloat = (~digits=2, number) => number->Js.Float.toFixedWithPrecision(~digits)->format

@ocaml.doc(`Formats a string float to 2 digits precision with groups of 3 decimals separated by a comma`)
let toCentsFixedNoRounding = floatString => floatString->Js.Float.fromString->formatFloat

@ocaml.doc(`Formats a BigNumber (10^18, wei) to 2 digits precision (ether) with groups of 3 decimals separated by a comma`)
let formatEther = rawNumber => rawNumber->Ethers.Utils.formatEther->toCentsFixedNoRounding
