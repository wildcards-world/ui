module Partners = {
  [@bs.module "./Partners.js"] [@react.component]
  external make: unit => React.element = "default";
};
