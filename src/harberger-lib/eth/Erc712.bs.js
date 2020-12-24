// Generated by ReScript, PLEASE EDIT WITH CARE


var permit = [
  {
    name: "holder",
    type: "address"
  },
  {
    name: "spender",
    type: "address"
  },
  {
    name: "nonce",
    type: "uint256"
  },
  {
    name: "expiry",
    type: "uint256"
  },
  {
    name: "allowed",
    type: "bool"
  }
];

var eip712Domain = [
  {
    name: "name",
    type: "string"
  },
  {
    name: "version",
    type: "string"
  },
  {
    name: "verifyingContract",
    type: "address"
  },
  {
    name: "salt",
    type: "bytes32"
  }
];

export {
  permit ,
  eip712Domain ,
  
}
/* No side effect */