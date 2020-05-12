// A collection of functions to make code cleaner.
include Belt;
include Async;

// From here: https://dev.to/johnridesabike/optional-chaining-in-reason-1im6
let oMap = Belt.Option.map;
let (<$>) = oMap;
let oFlatMap = Belt.Option.flatMap;
let (>>=) = oFlatMap;
let mapd = Option.mapWithDefault;
let (||||) = Option.getWithDefault;

let (|+|) = (a, b) => a->BN.addGet(. b);
let (|*|) = (a, b) => a->BN.mulGet(. b);
let (|-|) = (a, b) => a->BN.subGet(. b);
let (|/|) = (a, b) => a->BN.divGet(. b);
let (|==|) = (a, b) => a->BN.eqGet(. b);
let (|<|) = (a, b) => a->BN.ltGet(. b);
let (|>|) = (a, b) => a->BN.gtGet(. b);

// Float
let toFixedWithPrecisionNoTrailingZeros = (number: float, ~digits) => {
  number
  ->Js.Float.toFixedWithPrecision(~digits)
  ->float_of_string
  ->Float.toString;
};

// React components
let restr = React.string;
let reactMapWithDefault:
  (option('a), React.element, 'a => React.element) => React.element =
  (opt, default, f) =>
    switch (opt) {
    | None => default
    | Some(item) => f(item)
    };
let reactMap = (opt, f) => reactMapWithDefault(opt, React.null, f);

// For use with: https://github.com/reasonml-labs/bs-let
module Opt = {
  let let_ = Belt.Option.flatMap;
};