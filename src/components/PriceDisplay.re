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
      | None => useCurrentPriceUsd()
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
    <p> <S> {currentPrice ++ " ETH"} </S> </p>
    <p> <S> {"(" ++ currentPriceUsd ++ " USD)"} </S> </p>
  </React.Fragment>;
};
