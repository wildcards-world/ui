import React from "react";
import { theme as rimbleTheme } from "rimble-ui";
import { ThemeProvider } from "styled-components";
import TranslationProvider from "./TranslationProvider";
import UserProvider from "./UserProvider";

const theme = {
  ...rimbleTheme,
  colors: {
    ...rimbleTheme.colors,
    primary: "rgb(107, 173, 62)" // This is the primary green used by wildcards theme
  },
  buttons: {
    primary: {
      borderRadius: 0
    }
  }
};

export default ({ children }) => {
  return (
    <ThemeProvider theme={theme}>
      <UserProvider>
        <TranslationProvider>{children}</TranslationProvider>
      </UserProvider>
    </ThemeProvider>
  );
};
