import React, { createContext, useContext, useState } from "react";
import { TranslatorProvider } from "react-translate";
import translations from "../translations/translations";

export const TranslationContext = createContext("");

export const TranslationProvider = ({ children }) => {
  const [translationModeMuggle, setTranslationModeMuggle] = useState(true);

  return (
    <TranslatorProvider translations={translations}>
      <TranslationContext.Provider
        value={{
          translationModeMuggle,
          setTranslationModeMuggle,
          translationMode: translationModeMuggle ? "muggle" : "crypto"
        }}
      >
        {children}
      </TranslationContext.Provider>
    </TranslatorProvider>
  );
};

export const useTranslationModeContext = () => useContext(TranslationContext);

export default TranslationProvider;
