open Belt.Option;

let onlyUpdateIfPositiveFloat = (currentValue, updateFunction, event) => {
  ReactEvent.Form.preventDefault(event);

  let value = ReactEvent.Form.target(event)##value->getWithDefault("");
  let newValue =
    // IF the new number isn't a float, don't update.
    switch (Belt.Float.fromString(value)) {
    | Some(valueFloat) =>
      // IF the new number isn't positive, don't update.
      if (valueFloat >= 0.) {
        value;
      } else {
        currentValue;
      }
    | None =>
      // If the new value is an empty string let it through.
      if (value == "") {
        Js.log("using the NEW value");
        value;
      } else {
        Js.log("using the OLD value");
        currentValue;
      }
    };
  updateFunction(_ => newValue);
};
