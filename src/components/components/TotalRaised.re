open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

module TotalRaisedEtherCountup = {
  [@bs.module "./TotalRaisedEtherCountup.js"] [@react.component]
  external make: (~totalRaised: string) => React.element = "default";
};

let getTotalPatronage = vitalikPatronage => {
  let totalPatronageOtherTokens = useTotalPatronageWeiNew();

  let totalPatronageEth =
    BN.new_(vitalikPatronage)
    ->BN.addGet(. totalPatronageOtherTokens)
    ->BN.toStringGet(.)
    ->Web3Utils.fromWeiToEth;

  let currentUsdEthPrice = useUsdPrice()->mapWithDefault(0., a => a);
  let totaPatronageUsd =
    Js.Float.toFixedWithPrecision(
      Belt.Float.fromString(totalPatronageEth)->mapWithDefault(0., a => a)
      *. currentUsdEthPrice,
      2,
    );

  (Some(totalPatronageEth), totaPatronageUsd);
};

[@react.component]
let make = () => {
  let totalPatronageVitalik = useTotalPatronageWei();

  let (allTokensTotalRaisedEth, allTokensTotalRaisedUsd) = {
    switch (totalPatronageVitalik) {
    | Some(vitalikPatronage) => getTotalPatronage(vitalikPatronage)
    | None =>
      getTotalPatronage("0");
      (None, "Loading");
    };
  };

  <Offline requireSmartContractsLoaded=true>
    <p>
      <small>
        {React.string("Wildcards has currently raised ")}
        <br />
        <span className={Styles.totalRaisedText(4.)}>
          {switch (allTokensTotalRaisedEth) {
           | Some(totalRaised) => <TotalRaisedEtherCountup totalRaised />
           | None => React.string("Loading")
           }}
          <strong> {React.string(" ETH ")} </strong>
        </span>
        <br />
        <span className={Styles.totalRaisedText(2.5)}>
          {React.string("(")}
          {React.string(allTokensTotalRaisedUsd)}
          <strong> {React.string(" USD")} </strong>
          {React.string(")")}
        </span>
        <br />
        {React.string(" for conservation.")}
      </small>
    </p>
  </Offline>;
};
