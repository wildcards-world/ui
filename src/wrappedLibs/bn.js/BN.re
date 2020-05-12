// [@bs.deriving {abstract: light}]
[@bs.deriving abstract]
type bn = {
  add: (. bn) => bn,
  sub: (. bn) => bn,
  mul: (. bn) => bn,
  div: (. bn) => bn,
  gt: (. bn) => bool,
  lt: (. bn) => bool,
  eq: (. bn) => bool,
  cmp: (. bn) => int,
  sqr: (. unit) => bn,
  toString: (. unit) => string,
};

[@bs.new] [@bs.module "./bn-default.js"]
external new_: string => bn = "default";

[@bs.module "@polkadot/util"] external bnSqrt: (. bn) => bn = "bnSqrt";

// let test = bnSqrt(. new_("50"));
