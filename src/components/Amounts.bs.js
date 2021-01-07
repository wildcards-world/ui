// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Css from "bs-css-emotion/src/Css.bs.js";
import * as Eth from "../harberger-lib/Eth.bs.js";
import * as CssJs from "bs-css-emotion/src/CssJs.bs.js";
import * as Curry from "bs-platform/lib/es6/curry.js";
import * as React from "react";
import * as Countup from "./components/Countup.bs.js";
import * as Web3Utils from "../harberger-lib/Web3Utils.bs.js";
import * as Belt_Array from "bs-platform/lib/es6/belt_Array.js";
import * as Belt_Option from "bs-platform/lib/es6/belt_Option.js";
import * as Caml_option from "bs-platform/lib/es6/caml_option.js";
import * as UsdPriceProvider from "../harberger-lib/components/UsdPriceProvider.bs.js";

function stringToArray(str) {
  return Array.from(str);
}

function toDollarCentsFixedNoRounding(aFloat) {
  return Belt_Option.getWithDefault(Caml_option.null_to_opt(String(aFloat).match(/^\d+[.]\d\d/g)), [""])[0];
}

function Amounts$AmountRaised(Props) {
  var populateElementOpt = Props.populateElement;
  var mainnetEth = Props.mainnetEth;
  var maticDai = Props.maticDai;
  var populateElement = populateElementOpt !== undefined ? populateElementOpt : (function (bigTextComponent, smallTextComponent, optCommentTextComponent) {
        return React.createElement("p", undefined, bigTextComponent, React.createElement("span", {
                        className: CssJs.style([CssJs.fontSize(CssJs.em(0.5))])
                      }, smallTextComponent), " USD", optCommentTextComponent !== undefined ? React.createElement(React.Fragment, undefined, React.createElement("br", undefined), React.createElement("small", undefined, Caml_option.valFromOption(optCommentTextComponent))) : null);
      });
  var currentUsdEthPrice = UsdPriceProvider.useUsdPrice(undefined);
  var match = React.useState(function () {
        return [];
      });
  var setPrevUsdRaisedStrArray = match[1];
  var prevUsdRaisedStrArray = match[0];
  var match$1 = React.useState(function () {
        return [
                "",
                "",
                "",
                ""
              ];
      });
  var setRaisedDisplay = match$1[1];
  var match$2 = match$1[0];
  var raisedSubChanged = match$2[3];
  var usdRaisedFloat = Belt_Option.mapWithDefault(currentUsdEthPrice, 0, (function (usdEthRate) {
          return Eth.getFloat(mainnetEth, {
                      TAG: /* Usd */1,
                      _0: usdEthRate,
                      _1: 2
                    });
        })) + Eth.getFloat(maticDai, {
        TAG: /* Eth */0,
        _0: "ether"
      });
  var usdRaisedStr = usdRaisedFloat.toFixed(6);
  var usdRaised2Precision = toDollarCentsFixedNoRounding(usdRaisedFloat);
  React.useEffect((function () {
          var usdRaisedDigitArray = Array.from(usdRaisedStr);
          var indexOfTheFirstChangedDigit = usdRaisedDigitArray.findIndex(function (newDigit, index) {
                var digit = Belt_Array.get(prevUsdRaisedStrArray, index);
                if (digit !== undefined) {
                  return digit !== newDigit;
                } else {
                  return false;
                }
              });
          var match = usdRaised2Precision.length;
          if (indexOfTheFirstChangedDigit < 0) {
            Curry._1(setRaisedDisplay, (function (param) {
                    return [
                            usdRaised2Precision,
                            "",
                            usdRaisedStr.slice(match),
                            ""
                          ];
                  }));
          } else if (indexOfTheFirstChangedDigit > match) {
            Curry._1(setRaisedDisplay, (function (param) {
                    return [
                            usdRaised2Precision,
                            "",
                            usdRaisedStr.slice(match, indexOfTheFirstChangedDigit),
                            usdRaisedStr.slice(indexOfTheFirstChangedDigit)
                          ];
                  }));
          } else {
            Curry._1(setRaisedDisplay, (function (param) {
                    return [
                            usdRaised2Precision.slice(0, indexOfTheFirstChangedDigit),
                            usdRaised2Precision.slice(indexOfTheFirstChangedDigit),
                            "",
                            usdRaisedStr.slice(match)
                          ];
                  }));
          }
          Curry._1(setPrevUsdRaisedStrArray, (function (param) {
                  return usdRaisedDigitArray;
                }));
          
        }), [usdRaisedStr]);
  var match$3 = Web3Utils.fromWeiBNToEthPrecision(mainnetEth, 4);
  var match$4 = Web3Utils.fromWeiBNToEthPrecision(maticDai, 2);
  var optExplainerString;
  var exit = 0;
  if (match$3 === "0" && match$4 === "0") {
    optExplainerString = undefined;
  } else {
    exit = 1;
  }
  if (exit === 1) {
    optExplainerString = match$4 === "0" ? "(" + match$3 + " ETH)" : (
        match$3 === "0" ? undefined : "(" + match$3 + " ETH + " + match$4 + "DAI)"
      );
  }
  var styleOnCountUp = Curry._1(Css.style, {
        hd: Css.color(Css.green),
        tl: {
          hd: Css.fontWeight("bold"),
          tl: /* [] */0
        }
      });
  var tmp = raisedSubChanged === "" ? null : React.createElement(Countup.StringFloat.make, {
          stringFloat: raisedSubChanged,
          styleOnCountUp: styleOnCountUp
        });
  return React.createElement(React.Fragment, undefined, Curry._3(populateElement, React.createElement(React.Fragment, undefined, React.createElement("span", undefined, match$2[0]), React.createElement(Countup.StringFloat.make, {
                          stringFloat: match$2[1],
                          styleOnCountUp: styleOnCountUp
                        })), React.createElement(React.Fragment, undefined, React.createElement("span", undefined, match$2[2]), tmp), Belt_Option.map(optExplainerString, (function (prim) {
                        return prim;
                      }))));
}

var AmountRaised = {
  make: Amounts$AmountRaised
};

function Amounts$Basic(Props) {
  var populateElementOpt = Props.populateElement;
  var mainnetEth = Props.mainnetEth;
  var maticDai = Props.maticDai;
  var populateElement = populateElementOpt !== undefined ? populateElementOpt : (function (amountText, optCommentTextComponent) {
        return React.createElement("p", undefined, amountText, " USD", optCommentTextComponent !== undefined ? React.createElement(React.Fragment, undefined, React.createElement("br", undefined), React.createElement("small", undefined, Caml_option.valFromOption(optCommentTextComponent))) : null);
      });
  var currentUsdEthPrice = UsdPriceProvider.useUsdPrice(undefined);
  var usdRaisedFloat = Belt_Option.mapWithDefault(currentUsdEthPrice, 0, (function (usdEthRate) {
          return Eth.getFloat(mainnetEth, {
                      TAG: /* Usd */1,
                      _0: usdEthRate,
                      _1: 2
                    });
        })) + Eth.getFloat(maticDai, {
        TAG: /* Eth */0,
        _0: "ether"
      });
  var usdRaisedStr = usdRaisedFloat.toFixed(2);
  var match = Web3Utils.fromWeiBNToEthPrecision(mainnetEth, 4);
  var match$1 = Web3Utils.fromWeiBNToEthPrecision(maticDai, 2);
  var optExplainerString;
  var exit = 0;
  if (match === "0" && match$1 === "0") {
    optExplainerString = undefined;
  } else {
    exit = 1;
  }
  if (exit === 1) {
    optExplainerString = match$1 === "0" ? "(" + match + " ETH)" : (
        match === "0" ? undefined : "(" + match + " ETH + " + match$1 + "DAI)"
      );
  }
  return React.createElement(React.Fragment, undefined, Curry._2(populateElement, usdRaisedStr, Belt_Option.map(optExplainerString, (function (prim) {
                        return prim;
                      }))));
}

var Basic = {
  make: Amounts$Basic
};

export {
  stringToArray ,
  toDollarCentsFixedNoRounding ,
  AmountRaised ,
  Basic ,
  
}
/* Css Not a pure module */
