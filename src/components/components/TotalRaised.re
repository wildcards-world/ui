open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

module TotalRaisedEtherContup = {
  [@bs.module "./TotalRaisedEtherCountup.js"] [@react.component]
  external make: (~totalRaised: string) => React.element = "default";
};

[@react.component]
let make = () => {
  <Offline requireSmartContractsLoaded=true>
    <p>
      <small>
        {React.string("Wildcards has currently raised ")}
        <br />
        <TotalRaisedEtherContup
          totalRaised={
            useTotalPatronageEth()->mapWithDefault("loading", a => a)
          }
        />
        <strong> {React.string(" ETH ")} </strong>
        {React.string("(")}
        {React.string(
           useTotalPatronageUsd()->mapWithDefault("loading", a => a),
         )}
        <strong> {React.string(" USD")} </strong>
        {React.string(")")}
        <br />
        {React.string(" for conservation.")}
      </small>
    </p>
  </Offline>;
};
