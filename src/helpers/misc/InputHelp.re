open Belt.Option;

let onlyUpdateValueIfPositiveFloat = (currentValue, updateFunction, value) => {
  let (newValue, didUpdate: bool) =
    // IF the new number isn't a float, don't update.
    switch (Belt.Float.fromString(value)) {
    | Some(valueFloat) =>
      // IF the new number isn't positive, don't update.
      if (valueFloat >= 0.) {
        (value, true);
      } else {
        (currentValue, false);
      }
    | None =>
      // If the new value is an empty string let it through.
      if (value == "") {
        (value, true);
      } else {
        (currentValue, false);
      }
    };
  updateFunction(_ => newValue);
  (newValue, didUpdate);
};

let onlyUpdateValueIfInRangeFloat =
    (min, max, currentValue, updateFunction, value) => {
  let newValue =
    // IF the new number isn't a float, don't update.
    switch (Belt.Float.fromString(value)) {
    | Some(valueFloat) =>
      // IF the new number isn't positive, don't update.
      if (valueFloat >= min && valueFloat <= max) {
        value;
      } else {
        currentValue;
      }
    | None =>
      // If the new value is an empty string let it through.
      if (value == "") {
        value;
      } else {
        currentValue;
      }
    };
  updateFunction(_ => newValue);
};

let onlyUpdateIfPositiveFloat = (currentValue, updateFunction, event) => {
  ReactEvent.Form.preventDefault(event);

  let value = ReactEvent.Form.target(event)##value->getWithDefault("");
  onlyUpdateValueIfPositiveFloat(currentValue, updateFunction, value);
};

let onlyUpdateIfInRangeFloat = (min, max, currentValue, updateFunction, event) => {
  ReactEvent.Form.preventDefault(event);

  let value = ReactEvent.Form.target(event)##value->getWithDefault("");
  onlyUpdateValueIfInRangeFloat(
    min,
    max,
    currentValue,
    updateFunction,
    value,
  );
};
