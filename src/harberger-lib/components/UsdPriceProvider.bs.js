// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Async from "../Async.bs.js";
import * as Curry from "bs-platform/lib/es6/curry.js";
import * as Decco from "decco/src/Decco.bs.js";
import * as React from "react";
import * as Globals from "../Globals.bs.js";
import * as Js_dict from "bs-platform/lib/es6/js_dict.js";
import * as Js_json from "bs-platform/lib/es6/js_json.js";
import * as Belt_Array from "bs-platform/lib/es6/belt_Array.js";
import * as Belt_Float from "bs-platform/lib/es6/belt_Float.js";
import * as Belt_Option from "bs-platform/lib/es6/belt_Option.js";

var context = React.createContext(undefined);

var make = context.Provider;

function makeProps(value, children, param) {
  return {
          value: value,
          children: children
        };
}

var PriceProvider = {
  context: context,
  make: make,
  makeProps: makeProps
};

function c(param) {
  return param.c;
}

function price_encode(v) {
  return Js_dict.fromArray([[
                "c",
                Decco.arrayToJson(Decco.stringToJson, v.c)
              ]]);
}

function price_decode(v) {
  var dict = Js_json.classify(v);
  if (typeof dict === "number") {
    return Decco.error(undefined, "Not an object", v);
  }
  if (dict.TAG !== /* JSONObject */2) {
    return Decco.error(undefined, "Not an object", v);
  }
  var c = Decco.arrayFromJson(Decco.stringFromJson, Belt_Option.getWithDefault(Js_dict.get(dict._0, "c"), null));
  if (c.TAG === /* Ok */0) {
    return {
            TAG: /* Ok */0,
            _0: {
              c: c._0
            }
          };
  }
  var e = c._0;
  return {
          TAG: /* Error */1,
          _0: {
            path: ".c" + e.path,
            message: e.message,
            value: e.value
          }
        };
}

function ethUsd(param) {
  return param.ethUsd;
}

function ethUsdPrice_encode(v) {
  return Js_dict.fromArray([[
                "XETHZUSD",
                Decco.optionToJson(price_encode, v.ethUsd)
              ]]);
}

function ethUsdPrice_decode(v) {
  var dict = Js_json.classify(v);
  if (typeof dict === "number") {
    return Decco.error(undefined, "Not an object", v);
  }
  if (dict.TAG !== /* JSONObject */2) {
    return Decco.error(undefined, "Not an object", v);
  }
  var ethUsd = Decco.optionFromJson(price_decode, Belt_Option.getWithDefault(Js_dict.get(dict._0, "XETHZUSD"), null));
  if (ethUsd.TAG === /* Ok */0) {
    return {
            TAG: /* Ok */0,
            _0: {
              ethUsd: ethUsd._0
            }
          };
  }
  var e = ethUsd._0;
  return {
          TAG: /* Error */1,
          _0: {
            path: ".XETHZUSD" + e.path,
            message: e.message,
            value: e.value
          }
        };
}

function result(param) {
  return param.result;
}

function krakenPriceResponse_encode(v) {
  return Js_dict.fromArray([[
                "result",
                Decco.optionToJson(ethUsdPrice_encode, v.result)
              ]]);
}

function krakenPriceResponse_decode(v) {
  var dict = Js_json.classify(v);
  if (typeof dict === "number") {
    return Decco.error(undefined, "Not an object", v);
  }
  if (dict.TAG !== /* JSONObject */2) {
    return Decco.error(undefined, "Not an object", v);
  }
  var result = Decco.optionFromJson(ethUsdPrice_decode, Belt_Option.getWithDefault(Js_dict.get(dict._0, "result"), null));
  if (result.TAG === /* Ok */0) {
    return {
            TAG: /* Ok */0,
            _0: {
              result: result._0
            }
          };
  }
  var e = result._0;
  return {
          TAG: /* Error */1,
          _0: {
            path: ".result" + e.path,
            message: e.message,
            value: e.value
          }
        };
}

function getPrice(param) {
  return Async.let_(fetch("https://api.kraken.com/0/public/Ticker?pair=ETHUSD"), (function (result) {
                return Async.let_(result.json(), (function (response) {
                              var krakenPriceObj = krakenPriceResponse_decode(response);
                              var tmp;
                              if (krakenPriceObj.TAG === /* Ok */0) {
                                var getPriceFromArray = function (a) {
                                  return Belt_Array.get(a, 0);
                                };
                                tmp = Belt_Option.flatMap(Belt_Option.map(Belt_Option.flatMap(krakenPriceObj._0.result, ethUsd), c), getPriceFromArray);
                              } else {
                                console.log(krakenPriceObj._0);
                                tmp = undefined;
                              }
                              return Globals.async(tmp);
                            }));
              }));
}

function UsdPriceProvider(Props) {
  var children = Props.children;
  var match = React.useState(function () {
        
      });
  var setEtherUsdPrice = match[1];
  var etherUsdPrice = match[0];
  React.useEffect((function () {
          if (etherUsdPrice === undefined) {
            Globals.mapAsync(getPrice(undefined), (function (newPrice) {
                    return Curry._1(setEtherUsdPrice, (function (param) {
                                  return Belt_Option.flatMap(newPrice, Belt_Float.fromString);
                                }));
                  }));
          }
          
        }), [
        setEtherUsdPrice,
        etherUsdPrice
      ]);
  return React.createElement(make, makeProps(etherUsdPrice, children, undefined));
}

function useUsdPrice(param) {
  return React.useContext(context);
}

var initialState = {
  usdPrice: undefined
};

var make$1 = UsdPriceProvider;

export {
  initialState ,
  PriceProvider ,
  c ,
  price_encode ,
  price_decode ,
  ethUsd ,
  ethUsdPrice_encode ,
  ethUsdPrice_decode ,
  result ,
  krakenPriceResponse_encode ,
  krakenPriceResponse_decode ,
  getPrice ,
  make$1 as make,
  useUsdPrice ,
  
}
/* context Not a pure module */
