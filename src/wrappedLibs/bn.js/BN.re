// [@bs.deriving {abstract: light}]
[@bs.deriving abstract]
type bn = {
  add: (. bn) => bn,
  sub: (. bn) => bn,
  mul: (. bn) => bn,
  div: (. bn) => bn,
  gt: (. bn) => bool,
  toString: (. unit) => string,
};

[@bs.new] [@bs.module "bn.js"] external new_: string => bn = "default";
