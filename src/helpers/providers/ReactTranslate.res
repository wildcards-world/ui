type translations

@module("react-translate")
external useTranslate: (. string, . string) => string = "useTranslate"

type translationModeContext = {
  translationMode: string,
  translationModeCrypto: bool,
  setTranslationModeCrypto: bool => unit,
}

module TranslatorProvider = {
  @module("react-translate") @react.component
  external make: (~children: React.element, ~translations: translations) => React.element =
    "TranslatorProvider"
}

let cryptoMuggleTranslations: translations = %raw(`
    {
      locale: "en",
      crypto: {
      },
      muggle: {
      }
    }
`)

module TranslationContext = {
  let translationContext = React.createContext({
    translationMode: "undefined",
    translationModeCrypto: false,
    setTranslationModeCrypto: _ => (),
  })

  let makeProps = (~value, ~children, ()) =>
    {
      "value": value,
      "children": children,
    }

  let make = React.Context.provider(translationContext)
}
let useTranslationModeContext = () => React.useContext(TranslationContext.translationContext)

@react.component
let make = (~children) => {
  let (translationModeCrypto, setTranslationModeCrypto) = React.useState(_ => false)

  <TranslatorProvider translations=cryptoMuggleTranslations>
    <TranslationContext
      value={
        translationMode: translationModeCrypto ? "crypto" : "muggle",
        translationModeCrypto: translationModeCrypto,
        setTranslationModeCrypto: newMode => setTranslationModeCrypto(_ => newMode),
      }>
      children
    </TranslationContext>
  </TranslatorProvider>
}
