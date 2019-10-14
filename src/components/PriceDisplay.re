open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

[@react.component]
let make = (~tokenId: option(string)) => {
  let currentPrice =
    (
      switch (tokenId) {
      | None => useCurrentPriceEth()
      | Some(tokenIdSet) => useCurrentPriceEthNew(tokenIdSet)
      }
    )
    ->mapWithDefault("loading", a => a);
  let currentPriceUsd =
    (
      switch (tokenId) {
      | None => useCurrentPriceUsd()
      | Some(tokenIdSet) => useCurrentPriceUsdNew(tokenIdSet)
      }
    )
    ->mapWithDefault("loading", a => a);

  <React.Fragment>
    <p className={Styles.noMarginTop ++ " " ++ Styles.noMarginBottom}>
      <S> {currentPrice ++ " ETH"} </S>
    </p>
    <p className=Styles.noMarginTop>
      <small> <S> {"(" ++ currentPriceUsd ++ " USD)"} </S> </small>
    </p>
  </React.Fragment>;
};
