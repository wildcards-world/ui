// A collection of functions to make code cleaner.

// From here: https://dev.to/johnridesabike/optional-chaining-in-reason-1im6
let (<$>) = Belt.Option.map;
let (>>=) = Belt.Option.flatMap;

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
