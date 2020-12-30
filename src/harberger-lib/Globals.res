// A collection of functions to make code cleaner.
include Async

// Float
let toFixedWithPrecisionNoTrailingZeros = (number: float, ~digits) =>
  number->Js.Float.toFixedWithPrecision(~digits)->float_of_string->Float.toString

// React components
let restr = React.string
let reactMapWithDefault: (option<'a>, React.element, 'a => React.element) => React.element = (
  opt,
  default,
  f,
) =>
  switch opt {
  | None => default
  | Some(item) => f(item)
  }
let reactMap = (opt, f) => reactMapWithDefault(opt, React.null, f)

// For use with: https://github.com/reasonml-labs/bs-let
module Opt = {
  let let_ = Belt.Option.flatMap
}
