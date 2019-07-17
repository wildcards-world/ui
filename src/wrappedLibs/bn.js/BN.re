[@bs.deriving abstract]
type bn = {
  add: (. bn) => bn,
  toString: (. unit) => string,
};

let new_: string => bn = [%bs.raw
  {|(number) => {
    const BN = require('bn.js');
    const newBn = new BN(number)
    return newBn}|}
];
