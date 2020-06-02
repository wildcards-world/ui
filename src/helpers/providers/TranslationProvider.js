import React, { createContext, useContext, useState } from "react";
import { TranslatorProvider } from "react-translate";
import translations from "../translations/translations";

export const TranslationContext = createContext("");

export const TranslationProvider = ({ children }) => {
  const [translationModeCrypto, setTranslationModeCrypto] = useState(false);
  console.log("translationModeCrypto");
  console.log(translationModeCrypto);
  console.log(setTranslationModeCrypto);

  return (
    <TranslatorProvider translations={translations}>
      <TranslationContext.Provider
        value={{
          translationModeCrypto,
          setTranslationModeCrypto,
          translationMode: translationModeCrypto ? "crypto" : "muggle",
        }}
      >
        {children}
      </TranslationContext.Provider>
    </TranslatorProvider>
  );
};

export const useTranslationModeContext = () => useContext(TranslationContext);

export default TranslationProvider;
