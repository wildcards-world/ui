module Button = {
  [@bs.module "rimble-ui"] [@react.component]
  external make:
    (
      ~_as: string=?,
      ~href: string=?,
      ~target: string=?,
      ~children: React.element,
      ~onClick: ReactEvent.Form.t => unit=?
    ) =>
    React.element =
    "Button";
};

module Input = {
  [@bs.module "rimble-ui"] [@react.component]
  external make:
    (
      ~_type: string,
      ~required: bool=?,
      ~placeholder: string=?,
      ~className: string=?,
      ~onChange: ReactEvent.Form.t => unit=?,
      ~value: string=?
    ) =>
    React.element =
    "Input";
};

module Loader = {
  [@bs.module "rimble-ui"] [@react.component]
  external make: unit => React.element = "Loader";
};
