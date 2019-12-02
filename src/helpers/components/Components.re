module S = {
  [@react.component]
  let make = (~children: string) => {
    React.string(children);
  };
};

module HoverToggle = {
  [@bs.module "./HoverToggle.js"] [@react.component]
  external make:
    (~_ComponentHover: React.element, ~_ComponentNoHover: React.element) =>
    React.element =
    "default";
};
