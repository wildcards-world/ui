// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Eth from "../harberger-lib/Eth.bs.js";
import BnJs from "bn.js";
import * as Js_json from "rescript/lib/es6/js_json.js";
import * as TokenId from "../harberger-lib/TokenId.bs.js";
import * as MomentRe from "bs-moment/src/MomentRe.bs.js";
import * as Belt_Option from "rescript/lib/es6/belt_Option.js";
import * as Caml_format from "rescript/lib/es6/caml_format.js";

function parse(json) {
  var str = Js_json.decodeString(json);
  if (str !== undefined) {
    return new BnJs(str);
  } else {
    console.log("CRITICAL - should never happen!");
    return new BnJs(0);
  }
}

function serialize(bn) {
  return bn.toString();
}

var $$BigInt = {
  parse: parse,
  serialize: serialize
};

function parse$1(stringBn) {
  return new BnJs(stringBn);
}

function serialize$1(bn) {
  return bn.toString();
}

var BigIntStr = {
  parse: parse$1,
  serialize: serialize$1
};

function parse$2(tokenIdJson) {
  return Belt_Option.getWithDefault(TokenId.make(Belt_Option.mapWithDefault(Js_json.decodeString(tokenIdJson), "0", (function (a) {
                        return a;
                      }))), TokenId.makeFromInt(0));
}

var serialize$2 = TokenId.toString;

var GqlTokenId = {
  parse: parse$2,
  serialize: serialize$2
};

function parse$3(tokenIdJson) {
  return Belt_Option.getWithDefault(TokenId.make(tokenIdJson), TokenId.makeFromInt(0));
}

var serialize$3 = TokenId.toString;

var GqlTokenIdStr = {
  parse: parse$3,
  serialize: serialize$3
};

function parse$4(price) {
  return Eth.makeWithDefault(Belt_Option.mapWithDefault(Js_json.decodeString(price), "0", (function (a) {
                    return a;
                  })), 0);
}

function serialize$4(price) {
  return price.toString();
}

var Price = {
  parse: parse$4,
  serialize: serialize$4
};

function parse$5(moment) {
  return MomentRe.momentWithUnix(Belt_Option.mapWithDefault(Js_json.decodeString(moment), 0, Caml_format.int_of_string));
}

function serialize$5(moment) {
  return String(moment.unix());
}

var GqlMoment = {
  parse: parse$5,
  serialize: serialize$5
};

function parse$6(address) {
  return Belt_Option.mapWithDefault(Js_json.decodeString(address), "0x0", (function (a) {
                return a;
              }));
}

function serialize$6(address) {
  return address;
}

var GqlAddress = {
  parse: parse$6,
  serialize: serialize$6
};

export {
  $$BigInt ,
  BigIntStr ,
  GqlTokenId ,
  GqlTokenIdStr ,
  Price ,
  GqlMoment ,
  GqlAddress ,
}
/* Eth Not a pure module */
