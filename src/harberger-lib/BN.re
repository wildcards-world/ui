// [@bs.deriving {abstract: light}]
[@bs.deriving abstract]
type t = {
  add: (. t) => t,
  sub: (. t) => t,
  mul: (. t) => t,
  div: (. t) => t,
  gt: (. t) => bool,
  lt: (. t) => bool,
  eq: (. t) => bool,
  cmp: (. t) => int,
  sqr: (. unit) => t,
  toString: (. unit) => string,
};
type bn = t;

[@bs.send] external toNumber: t => int = "toNumber";
[@bs.send] external toNumberFloat: t => float = "toNumber";

[@bs.new] [@bs.module "bn.js"] external new_: string => t = "default";

// [@bs.module "@polkadot/util"] external tSqrt: (. t) => t = "tSqrt";

// let test = tSqrt(. new_("50"));
