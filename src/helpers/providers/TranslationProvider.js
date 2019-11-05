import React, { createContext, useContext, useState } from "react";
import { TranslatorProvider } from "react-translate";
import translations from "../translations/translations";

export const TranslationContext = createContext("");

export const TranslationProvider = ({ children }) => {
  const [translationMode, setTranslationMode] = useState("crypto");

  return (
    <TranslatorProvider translations={translations}>
      <TranslationContext.Provider
        value={{ translationMode, setTranslationMode }}
      >
        {children}
      </TranslationContext.Provider>
    </TranslatorProvider>
  );
};

export const useTranslationModeContext = () => useContext(TranslationContext);

export default TranslationProvider;
