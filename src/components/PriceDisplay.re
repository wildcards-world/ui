open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

[@react.component]
let make = (~gorilla: Gorilla.gorilla) => {
  let currentPrice = Gorilla.useCurrentPriceEth(gorilla);
  let currentPriceUsd = Gorilla.useCurrentPriceUsd(gorilla);

  <React.Fragment>
    <p className={Styles.noMarginTop ++ " " ++ Styles.noMarginBottom}>
      <S> {currentPrice ++ " ETH"} </S>
    </p>
    <p className=Styles.noMarginTop>
      <small> <S> {"(" ++ currentPriceUsd ++ " USD)"} </S> </small>
    </p>
  </React.Fragment>;
};
