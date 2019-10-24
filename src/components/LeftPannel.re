open Hooks;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

// module ShareSocial = {
//   [@bs.module "./components/shareSocialMedia"] [@react.component]
//   external make: unit => React.element = "default";
// };

module DefaultLeftPanel = {
  [@react.component]
  let make = () => {
    <React.Fragment>
      <h1 className=Styles.heading>
        <span className=Styles.colorBlue> <S> "Always for sale" </S> </span>
        <br />
        <S> "ethereum based" </S>
        <br />
        <span className=Styles.colorGreen> <S> "conservation" </S> </span>
        <S> " tokens" </S>
      </h1>
      <hr />
      <h3 className=Styles.subHeading>
        <S>
          "Let your digital assets make a valuable contribution to the world."
        </S>
      </h3>
    </React.Fragment>;
  };
};
module GorillaInfo = {
  [@react.component]
  let make = (~gorilla: Gorilla.gorilla) => {
    let currentPatron =
      GeneralHooks.useCurrentPatronGorilla(gorilla)
      ->mapWithDefault("Loading", a => a);
    let depositAvailableToWithdraw =
      GeneralHooks.useDepositAbleToWithdrawEthGorilla(gorilla)
      ->mapWithDefault("Loading", a => a);
    let depositAvailableToWithdrawUsd =
      GeneralHooks.useDepositAbleToWithdrawUsdGorilla(gorilla)
      ->mapWithDefault("Loading", a => a);
    let totalPatronage = GeneralHooks.useTotalPatronageEthGorilla(gorilla);
    let totalPatronageUsd =
      GeneralHooks.useTotalPatronageUsdGorilla(gorilla)
      ->mapWithDefault("Loading", a => a);

    // TODO: the ethereum address is really terribly displayed. But the default in Rimble UI includes a QR code scanner (which is really ugly).
    // https://rimble.consensys.design/components/rimble-ui/EthAddress#props
    // https://github.com/ConsenSys/rimble-ui/blob/dd470f00374a05c860b558a2cb9317861e4a0d89/src/EthAddress/index.js (maybe make a PR here with some changes)
    <div>
      <p> <S> {"Current Patron: " ++ currentPatron} </S> </p>
      // <p> <S> {"Time Held: " ++ timeHeld} </S> </p>
      <p>
        <S> {"Available Deposit: " ++ depositAvailableToWithdraw} </S>
        <br />
        <small>
          <S> {"(" ++ depositAvailableToWithdrawUsd ++ " USD)"} </S>
        </small>
      </p>
      <p>
        <S> {"Total Patronage: " ++ totalPatronage ++ " ETH"} </S>
        <br />
        <small> <S> {"(" ++ totalPatronageUsd ++ " USD)"} </S> </small>
      </p>
    </div>;
  };
};

[@react.component]
// The Offline container here shows the website, but without loading the requirements
let make = () => {
  open Gorilla;
  let url = ReasonReactRouter.useUrl();
  <React.Fragment>
    {switch (Js.String.split("/", url.hash)) {
     // switch (url.hash) {
     | [|"details", gorillaStr|] =>
       let gorilla = getGorilla(gorillaStr);
       switch (gorilla) {
       | NoGorilla => <DefaultLeftPanel />
       | _ =>
         <Providers.UsdPriceProvider>
           <Offline requireSmartContractsLoaded=true>
             <GorillaInfo gorilla />
           </Offline>
         </Providers.UsdPriceProvider>
       };
     | _ => <DefaultLeftPanel />
     }}
  </React.Fragment>;
};
