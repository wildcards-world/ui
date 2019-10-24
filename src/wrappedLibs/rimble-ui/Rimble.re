type width =
  | AnyStr(string)
  | AnyStryArray(array(string))
  | Float(float)
  | FloatArray(array(float));

module Button = {
  [@bs.module "rimble-ui"] [@react.component]
  external make:
    (
      ~_as: string=?,
      ~className: string=?,
      ~href: string=?,
      ~target: string=?,
      ~children: React.element,
      ~onClick: ReactEvent.Form.t => unit=?,
      ~width: float=?
    ) =>
    React.element =
    "Button";

  module Text = {
    [@bs.module "rimble-ui"] [@bs.scope "Button"] [@react.component]
    external make:
      (
        ~_as: string=?,
        ~className: string=?,
        ~href: string=?,
        ~target: string=?,
        ~children: React.element=?,
        ~onClick: ReactEvent.Form.t => unit=?,
        ~icononly: bool=?,
        ~icon: string=?,
        ~color: string,
        ~position: string,
        ~top: int,
        ~right: int,
        ~m: int
      ) =>
      React.element =
      "Text";
  };
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
  external make: (~className: string=?) => React.element = "Loader";
};

module Modal = {
  [@bs.module "rimble-ui"] [@react.component]
  external make: (~isOpen: bool, ~children: React.element) => React.element =
    "Modal";
};
module Card = {
  [@bs.module "rimble-ui"] [@react.component]
  external make:
    (
      ~width: width=?,
      ~className: string=?,
      ~p: int=?,
      ~children: React.element
    ) =>
    React.element =
    "Card";
};
module Heading = {
  [@bs.module "rimble-ui"] [@react.component]
  external make:
    (
      ~_as: string=?,
      ~fontSize: int=?,
      ~children: React.element,
      ~className: string=?
    ) =>
    React.element =
    "Heading";
};
module HeadingS = {
  [@bs.module "rimble-ui"] [@react.component]
  external make:
    (~_as: string=?, ~fontSize: int=?, ~children: string) => React.element =
    "Heading";
};
module Box = {
  [@bs.module "rimble-ui"] [@react.component]
  external make:
    (
      ~p: int=?,
      ~mb: int=?,
      ~children: React.element,
      ~width: array(float)=?,
      ~color: string=?,
      ~bg: string=?,
      ~className: string=?,
      ~onClick: unit => unit=?
    ) =>
    React.element =
    "Box";
};
module Text = {
  [@bs.module "rimble-ui"] [@react.component]
  external make:
    (
      ~children: React.element,
      ~color: string=?,
      ~fontFamily: string=?,
      ~fontSize: string=?,
      ~fontWeight: string=?,
      ~lineHeight: string=?,
      ~textAlign: string=?
    ) =>
    React.element =
    "Text";
};
module TextS = {
  [@bs.module "rimble-ui"] [@react.component]
  external make: (~children: string) => React.element = "Text";
};

module Flex = {
  [@bs.module "rimble-ui"] [@react.component]
  external make:
    (
      ~children: React.element,
      ~flexWrap: string=?,
      ~alignItems: string=?,
      ~className: string=?
    ) =>
    React.element =
    "Flex";
};

module Form = {
  [@bs.module "rimble-ui"] [@react.component]
  external make:
    (
      ~onSubmit: ReactEvent.Form.t => unit=?,
      ~children: React.element,
      ~action: string=?,
      ~method: string=?
    ) =>
    React.element =
    "Form";

  module Input = {
    [@bs.module "rimble-ui"] [@bs.scope "Form"] [@react.component]
    external make:
      (
        ~_type: string,
        ~required: bool=?,
        ~placeholder: string=?,
        ~name: string=?,
        ~className: string=?,
        ~onChange: ReactEvent.Form.t => unit=?,
        ~value: string=?,
        ~width: float=?
      ) =>
      React.element =
      "Input";
  };
};
