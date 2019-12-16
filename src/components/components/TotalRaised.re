open Hooks;
open Providers.UsdPriceProvider;
open Belt.Option;

module TotalRaisedEtherCountup = {
  [@bs.module "./TotalRaisedEtherCountup.js"] [@react.component]
  external make: (~totalRaised: string) => React.element = "default";
};

type patronageRaised =
  | Loaded(string, string)
  | Loading;

let uesTotalPatronage = () => {
  let totalPatronageOtherTokens = useTotalPatronageWeiNew();
  let currentUsdEthPrice = useUsdPrice()->mapWithDefault(0., a => a);

  switch (totalPatronageOtherTokens) {
  | Some(patronageVitalik) =>
    let totalPatronageEth =
      patronageVitalik->BN.toStringGet(.)->Web3Utils.fromWeiToEth;

    let totaPatronageUsd =
      Js.Float.toFixedWithPrecision(
        Belt.Float.fromString(totalPatronageEth)->mapWithDefault(0., a => a)
        *. currentUsdEthPrice,
        ~digits=2,
      );

    Loaded(totalPatronageEth, totaPatronageUsd);
  | _ => Loading
  };
};

[@react.component]
let make = () => {
  let totalPatronageRaised = uesTotalPatronage();

  switch (totalPatronageRaised) {
  | Loaded(totalRaised, totalRaisedUsd) =>
    <Offline requireSmartContractsLoaded=true>
      <p>
        <small>
          <span className={Styles.totalRaisedText(1.5)}>
            {React.string("Wildcards has currently raised ")}
          </span>
          <br />
          <span className={Styles.totalRaisedText(4.)}>
            <TotalRaisedEtherCountup totalRaised />
            <strong> {React.string(" ETH ")} </strong>
          </span>
          <br />
          <span className={Styles.totalRaisedText(2.5)}>
            {React.string("(")}
            {React.string(totalRaisedUsd)}
            <strong> {React.string(" USD")} </strong>
            {React.string(")")}
          </span>
          <br />
          <span className={Styles.totalRaisedText(1.5)}>
            {React.string(" for conservation.")}
          </span>
        </small>
      </p>
    </Offline>
  | Loading => <Rimble.Loader />
  };
};
