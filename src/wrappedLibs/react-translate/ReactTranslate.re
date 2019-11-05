[@bs.module "react-translate"]
external useTranslate: (. string) => (. string) => string = "useTranslate";

module Switch = {
  [@bs.module "react-switch"] [@react.component]
  external make:
    (
      ~onChange: bool => unit=?,
      ~checked: bool=?,
      ~onColor: string=?,
      ~onHandleColor: string=?,
      ~uncheckedIcon: bool=?,
      ~checkedIcon: bool=?,
      ~className: string=?
    ) =>
    React.element =
    "default";
};

[@bs.deriving {abstract: light}]
type translationModeContext = {
  translationMode: string,
  translationModeMuggle: bool,
  setTranslationModeMuggle: bool => unit,
};
[@bs.module "../../helpers/providers/TranslationProvider"]
external useTranslationModeContext: unit => translationModeContext =
  "useTranslationModeContext";