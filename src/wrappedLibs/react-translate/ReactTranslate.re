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
      ~offHandleColor: string=?,
      ~uncheckedIcon: bool=?,
      ~checkedIcon: bool=?,
      ~height: int=?,
      ~width: int=?,
      ~handleDiameter: int=?,
      ~className: string=?
    ) =>
    React.element =
    "default";
};

type translationModeContext = {
  translationMode: string,
  translationModeCrypto: bool,
  setTranslationModeCrypto: bool => unit,
};
[@bs.module "../../helpers/providers/TranslationProvider"]
external useTranslationModeContext: unit => translationModeContext =
  "useTranslationModeContext";
