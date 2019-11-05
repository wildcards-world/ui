[@bs.module "react-translate"]
external useTranslate: (string, string) => string = "useTranslate";
[@bs.deriving {abstract: light}]
type translationModeContext = {
  translationMode: string,
  setTranslationMode: string => unit,
};
[@bs.module "../../helpers/providers/TranslationProvider"]
external useTranslationModeContext: unit => translationModeContext =
  "useTranslationModeContext";