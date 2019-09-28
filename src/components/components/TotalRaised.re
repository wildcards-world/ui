open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

[@react.component]
let make = () => {
  <Offline requireSmartContractsLoaded=true>
    <h3>
      <S>
        {"Wildcards has currently raised "
         ++ useTotalPatronageEth()->mapWithDefault("loading", a => a)
         ++ "ETH("
         ++ useTotalPatronageUsd()->mapWithDefault("loading", a => a)
         ++ "USD) for conservation."}
      </S>
    </h3>
  </Offline>;
};
