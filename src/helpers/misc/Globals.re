// A collection of functions to make code cleaner.

// From here: https://dev.to/johnridesabike/optional-chaining-in-reason-1im6
let (<$>) = Belt.Option.map;
let (>>=) = Belt.Option.flatMap;

let restr = React.string;

// For use with: https://github.com/reasonml-labs/bs-let
module Opt = {
  let let_ = Belt.Option.flatMap;
};
