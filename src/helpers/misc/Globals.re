// A collection of functions to make code cleaner.
include Belt;

// From here: https://dev.to/johnridesabike/optional-chaining-in-reason-1im6
let (<$>) = Belt.Option.map;
let (>>=) = Belt.Option.flatMap;

let (|+|) = (a, b) => a->BN.addGet(. b);
let (|*|) = (a, b) => a->BN.mulGet(. b);
let (|-|) = (a, b) => a->BN.subGet(. b);
let (|/|) = (a, b) => a->BN.divGet(. b);
let (|==|) = (a, b) => a->BN.eqGet(. b);
let (|>|) = (a, b) => a->BN.ltGet(. b);
let (|<|) = (a, b) => a->BN.gtGet(. b);

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