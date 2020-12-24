// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Belt_Float from "bs-platform/lib/es6/belt_Float.js";
import * as Web3Utils from "web3-utils";
import * as Belt_Option from "bs-platform/lib/es6/belt_Option.js";
import * as Globals$WildCards from "./Globals.bs.js";

function fromWeiToEth(value) {
  return Web3Utils.fromWei(value, "ether");
}

function fromWeiBNToEth(value) {
  return Web3Utils.fromWei(value, "ether");
}

function fromWeiBNToEthPrecision(value, digits) {
  return Globals$WildCards.toFixedWithPrecisionNoTrailingZeros(Belt_Option.mapWithDefault(Belt_Float.fromString(Web3Utils.fromWei(value, "ether")), 0, (function (a) {
                    return a;
                  })), digits);
}

function toWeiFromEth(value) {
  return Web3Utils.toWei(value, "ether");
}

export {
  fromWeiToEth ,
  fromWeiBNToEth ,
  fromWeiBNToEthPrecision ,
  toWeiFromEth ,
  
}
/* web3-utils Not a pure module */