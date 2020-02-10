import React, { createContext, useContext, useState } from "react";
import { TranslatorProvider } from "react-translate";
import translations from "../translations/translations";

export const TranslationContext = createContext("");

export const TranslationProvider = ({ children }) => {
  const [translationModeCrypto, settranslationModeCrypto] = useState(false);

  return (
    <TranslatorProvider translations={translations}>
      <TranslationContext.Provider
        value={{
          translationModeCrypto,
          settranslationModeCrypto,
          translationMode: translationModeCrypto ? "crypto" : "muggle"
        }}
      >
        {children}
      </TranslationContext.Provider>
    </TranslatorProvider>
  );
};

export const useTranslationModeContext = () => useContext(TranslationContext);

export default TranslationProvider;
