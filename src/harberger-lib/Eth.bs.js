// Generated by ReScript, PLEASE EDIT WITH CARE

import BnJs from "bn.js";
import * as Helper from "./Helper.bs.js";
import * as Belt_Float from "rescript/lib/es6/belt_Float.js";
import * as Web3Utils from "web3-utils";
import * as Belt_Option from "rescript/lib/es6/belt_Option.js";
import * as Caml_option from "rescript/lib/es6/caml_option.js";

var _map = {"wei":"wei","kwei":"kwei","mwei":"mwei","gwei":"gwei","microether":"microether","milliether":"milliether","ether":"ether","kether":"kether","mether":"mether","geher":"geher","tether":"tether"};

function ethUnitToJs(param) {
  return param;
}

function ethUnitFromJs(param) {
  return _map[param];
}

function fromWeiEth(value) {
  return Web3Utils.fromWei(value, "ether");
}

function getFloat(value, unit) {
  if (unit.TAG === /* Eth */0) {
    return Number(Web3Utils.fromWei(value, unit._0));
  } else {
    return Number(Web3Utils.fromWei(value, "ether")) * unit._0;
  }
}

function make(wei) {
  if (Helper.isPositiveStringInteger(wei)) {
    return Caml_option.some(new BnJs(wei));
  }
  
}

function makeWithDefault(tokenId, $$default) {
  var wei = make(tokenId);
  if (wei !== undefined) {
    return Caml_option.valFromOption(wei);
  } else {
    return new BnJs(String($$default));
  }
}

function makeFromInt(tokenId) {
  return new BnJs(String(tokenId));
}

function makeFromEthStr(eth) {
  return Belt_Option.flatMap(Belt_Float.fromString(eth), (function (ethFloat) {
                return Caml_option.some(new BnJs(Web3Utils.toWei(String(ethFloat), "ether")));
              }));
}

export {
  ethUnitToJs ,
  ethUnitFromJs ,
  fromWeiEth ,
  getFloat ,
  make ,
  makeWithDefault ,
  makeFromInt ,
  makeFromEthStr ,
}
/* bn.js Not a pure module */
