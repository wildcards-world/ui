// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Curry from "rescript/lib/es6/curry.js";
import * as Belt_Float from "rescript/lib/es6/belt_Float.js";
import * as Belt_Option from "rescript/lib/es6/belt_Option.js";

function getPagePrefix(isExplorer) {
  if (isExplorer) {
    return "explorer/";
  } else {
    return "";
  }
}

function handleEvent(action, $$event) {
  $$event.preventDefault();
  return Curry._1(action, undefined);
}

function handleMouseEvent(action, $$event) {
  $$event.preventDefault();
  return Curry._1(action, undefined);
}

function onlyUpdateValueIfPositiveFloat(currentValue, updateFunction, value) {
  var valueFloat = Belt_Float.fromString(value);
  var match = valueFloat !== undefined ? (
      valueFloat >= 0 ? [
          value,
          true
        ] : [
          currentValue,
          false
        ]
    ) : (
      value === "" ? [
          "0",
          true
        ] : [
          currentValue,
          false
        ]
    );
  var newValue = match[0];
  Curry._1(updateFunction, (function (param) {
          return newValue;
        }));
  return [
          newValue,
          match[1]
        ];
}

function onlyUpdateValueIfInRangeFloat(min, max, currentValue, updateFunction, value) {
  var valueFloat = Belt_Float.fromString(value);
  var match = valueFloat !== undefined ? (
      valueFloat >= min && valueFloat <= max ? [
          value,
          true
        ] : [
          currentValue,
          false
        ]
    ) : (
      value === "" ? [
          min.toString(),
          true
        ] : [
          currentValue,
          false
        ]
    );
  var newValue = match[0];
  Curry._1(updateFunction, (function (param) {
          return newValue;
        }));
  return [
          newValue,
          match[1]
        ];
}

function onlyUpdateIfPositiveFloat(currentValue, updateFunction, $$event) {
  $$event.preventDefault();
  var value = Belt_Option.getWithDefault($$event.target.value, "");
  return onlyUpdateValueIfPositiveFloat(currentValue, updateFunction, value);
}

function onlyUpdateIfInRangeFloat(min, max, currentValue, updateFunction, $$event) {
  $$event.preventDefault();
  var value = Belt_Option.getWithDefault($$event.target.value, "");
  return onlyUpdateValueIfInRangeFloat(min, max, currentValue, updateFunction, value);
}

export {
  getPagePrefix ,
  handleEvent ,
  handleMouseEvent ,
  onlyUpdateValueIfPositiveFloat ,
  onlyUpdateValueIfInRangeFloat ,
  onlyUpdateIfPositiveFloat ,
  onlyUpdateIfInRangeFloat ,
}
/* No side effect */
