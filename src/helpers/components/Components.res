module HoverToggle = {
  @dead("HoverToggle.+make") @module("./HoverToggle.js") @react.component
  external make: (
    ~_ComponentHover: React.element,
    ~_ComponentNoHover: React.element,
  ) => React.element = "default"
}
