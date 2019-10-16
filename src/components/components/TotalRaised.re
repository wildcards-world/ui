open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

let getTotalPatronage = vitalikPatronage => {
  let totalPatronageOtherTokens = useTotalPatronageWeiNew();
  Js.log(vitalikPatronage);
  Js.log(totalPatronageOtherTokens);
  let totalPatronageEth =
    BN.new_(vitalikPatronage)
    ->BN.addGet(. totalPatronageOtherTokens)
    ->BN.toStringGet(.)
    ->Web3Utils.fromWeiToEth;

  let currentUsdEthPrice = useUsdPrice()->mapWithDefault(0., a => a);
  Js.log2(totalPatronageEth, currentUsdEthPrice);
  let totaPatronageUsd =
    Js.Float.toFixedWithPrecision(
      Belt.Float.fromString(totalPatronageEth)->mapWithDefault(0., a => a)
      *. currentUsdEthPrice,
      2,
    );

  (totalPatronageEth, totaPatronageUsd);
};

[@react.component]
let make = () => {
  let totalPatronageVitalik = useTotalPatronageWei();

  let (allTokensTotalRaisedEth, allTokensTotalRaisedUsd) = {
    switch (totalPatronageVitalik) {
    | Some(vitalikPatronage) => getTotalPatronage(vitalikPatronage)
    | None =>
      getTotalPatronage("0");
      ("Loading", "Loading");
    };
  };

  <Offline requireSmartContractsLoaded=true>
    <p>
      <small>
        {React.string("Wildcards has currently raised ")}
        <br />
        {React.string(allTokensTotalRaisedEth)}
        <strong> {React.string(" ETH ")} </strong>
        {React.string("(")}
        {React.string(allTokensTotalRaisedUsd)}
        <strong> {React.string(" USD")} </strong>
        {React.string(")")}
        <br />
        {React.string(" for conservation.")}
      </small>
    </p>
  </Offline>;
};
