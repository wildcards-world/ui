open Hooks;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

// TODO: there must be a better way of importing images in reason react...
module ShareSocial = {
  [@bs.module "./components/shareSocialMedia"] [@react.component]
  external make: unit => React.element = "default";
};
module CountDownForeclosure = {
  [@bs.module "./components/CountDown"] [@react.component]
  external make: (~endDateMoment: MomentRe.Moment.t) => React.element =
    "default";
};

module EditButton = {
  // let make = () => {
  [@react.component]
  let make = (~gorilla: Gorilla.gorilla) => {
    <Rimble.Button
      onClick={event => {
        ReactEvent.Form.preventDefault(event);
        ReasonReactRouter.push("#details/" ++ Gorilla.getName(gorilla));
      }}>
      {React.string("Edit")}
    </Rimble.Button>;
  };
};

module GorillaOnLandingPage = {
  [@react.component]
  let make = (~gorilla: Gorilla.gorilla, ~owned: bool) => {
    let name = Gorilla.getName(gorilla);

    <Rimble.Box>
      <a
        className=Styles.clickableLink
        onClick={event => {
          ReactEvent.Mouse.preventDefault(event);
          ReasonReactRouter.push("#details/" ++ name);
        }}>
        <img
          className={Styles.headerImg(150.)}
          src={Gorilla.getImage(gorilla)}
        />
        <div className=Styles.gorillaText>
          <h2> {React.string(name)} </h2>
        </div>
      </a>
      <div className=Styles.gorillaText>
        <Offline requireSmartContractsLoaded=true>
          <PriceDisplay gorilla />
          {owned ? <EditButton gorilla /> : <BuyModal gorilla />}
        </Offline>
      </div>
    </Rimble.Box>;
  };
};

module DefaultLook = {
  open Gorilla;

  [@react.component]
  let make = (~areRequirementsLoaded: bool=false) => {
    open Components;
    let setProvider = useSetProvider();
    React.useEffect0(() => {
      open Web3connect.Core;
      let core = getCore(None); // TOGGLE THE ABOVE LINE OUT BEFORE PRODUCTION!!
      core->setOnConnect(setProvider);
      None;
    });

    let (ownVitalik, ownSimon, ownAndy) =
      if (areRequirementsLoaded) {
        // NOTE/TODO: this doesn't take into account token ownership
        let currentPatronVitalik =
          useCurrentPatron()->mapWithDefault("no-patron-defined", a => a);
        let currentPatronSimon =
          useCurrentPatronNew(0)->mapWithDefault("no-patron-defined", a => a);
        let currentPatronAndy =
          useCurrentPatronNew(1)->mapWithDefault("no-patron-defined", a => a);
        let currentAccount =
          useCurrentUser()->mapWithDefault("no-current-account", a => a);

        (
          currentAccount == currentPatronVitalik,
          currentAccount == currentPatronSimon,
          currentAccount == currentPatronAndy,
        );
      } else {
        (false, false, false);
      };
    let url = ReasonReactRouter.useUrl();

    <div className=Styles.rightTopHeader>
      {switch (Js.String.split("/", url.hash)) {
       // switch (url.hash) {
       | [|"details", gorillaStr|] =>
         let gorilla = getGorilla(gorillaStr);
         let tokenId = getId(gorilla);
         switch (gorilla) {
         | NoGorilla =>
           <div>
             <h1>
               {React.string(
                  "We are unable to find a gorilla by the name of "
                  ++ gorillaStr
                  ++ " in our system.",
                )}
             </h1>
             <p> <S> "Please check the spelling and try again." </S> </p>
           </div>
         | _ =>
           <React.Fragment>
             <img className=Styles.ownedGorillaImg src={getImage(gorilla)} />
             <h2> <S> {getName(gorilla)} </S> </h2>
             <Offline requireSmartContractsLoaded=true>
               {
                 let isYours =
                   switch (gorilla) {
                   | Vitalik => ownVitalik
                   | Simon => ownSimon
                   | Andy => ownAndy
                   | _ => false
                   };

                 if (isYours) {
                   <React.Fragment>
                     <UpdatePriceModal gorilla />
                     <br />
                     <UpdateDeposit gorilla />
                     <br />
                     <ShareSocial />
                   </React.Fragment>;
                 } else {
                   <React.Fragment>
                     <PriceDisplay gorilla />
                     <BuyModal gorilla />
                   </React.Fragment>;
                 };
               }
             </Offline>
           </React.Fragment>
         };
       | _ =>
         <React.Fragment>
           <Rimble.Flex className=Styles.gorillaBox>
             <GorillaOnLandingPage gorilla=Simon owned=ownSimon />
             <GorillaOnLandingPage gorilla=Vitalik owned=ownVitalik />
             <GorillaOnLandingPage gorilla=Andy owned=ownAndy />
           </Rimble.Flex>
           <Rimble.Box className=Styles.dappImagesCounteractOffset>
             <Offline requireSmartContractsLoaded=true>
               <TotalRaised />
             </Offline>
           </Rimble.Box>
           <Rimble.Box className=Styles.dappImagesCounteractOffset>
             <p>
               <small>
                 <S> "Having problems buying? " </S>
                 <a
                   href="https://blog.wildcards.world/how-to-buy-a-wildcard-web3-ethereum/"
                   target="_blank">
                   <S> "Read our guide" </S>
                 </a>
               </small>
             </p>
           </Rimble.Box>
         </React.Fragment>
       }}
    </div>;
  };
};

module DefaultLeftPanel = {
  [@react.component]
  let make = () => {
    open ReactTranslate;
    let usedtranslationModeContext = useTranslationModeContext();
    let translation =
      useTranslate(. usedtranslationModeContext->translationMode);
    <React.Fragment>
      <h1 className=Styles.heading>
        <span className=Styles.colorBlue> <S> "Always for sale" </S> </span>
        <br />
        <S> {translation(. "ethereum") ++ " based"} </S>
        <br />
        <span className=Styles.colorGreen> <S> "conservation" </S> </span>
        <S> {" " ++ translation(. "tokens")} </S>
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
type maybeDate =
  | Loading
  | Date(MomentRe.Moment.t);

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
    // let currentPrice = GeneralHooks.use
    let foreclosureTime = GeneralHooks.useForeclosureTimeGorilla(gorilla);
    let definiteTime = foreclosureTime->mapWithDefault(Loading, a => Date(a));
    let (_, _, ratio, _) = Gorilla.pledgeRate(gorilla);
    let currentPrice = Gorilla.useCurrentPriceEth(gorilla);
    let currentPriceUsd = Gorilla.useCurrentPriceUsd(gorilla);
    let monthlyPledgeEth =
      Js.Float.toString(
        Belt.Float.fromString(currentPrice)->Accounting.defaultZeroF *. ratio,
      );
    let monthlyPledgeUsd =
      // Js.Float.toString(
      Js.Float.toFixedWithPrecision(
        Belt.Float.fromString(currentPriceUsd)->Accounting.defaultZeroF
        *. ratio,
        ~digits=2,
      );
    let monthlyRate = Js.Float.toString(ratio *. 100.);
    // TODO: the ethereum address is really terribly displayed. But the default in Rimble UI includes a QR code scanner (which is really ugly).
    // https://rimble.consensys.design/components/rimble-ui/EthAddress#props
    // https://github.com/ConsenSys/rimble-ui/blob/dd470f00374a05c860b558a2cb9317861e4a0d89/src/EthAddress/index.js (maybe make a PR here with some changes)
    <Rimble.Box m=5>
      <div>
        <small>
          <strong>
            <S> {"Monthly Pledge (at " ++ monthlyRate ++ "%): "} </S>
            <Rimble.Tooltip
              message={
                "This is the monthly percentage contribution of "
                ++ Gorilla.getName(gorilla)
                ++ "'s sale price that will go towards conservation of endangered animals. This is deducted continuously from the deposit and paid by the owner of the animal"
              }
              placement="top">
              <span> <S> {js|ⓘ|js} </S> </span>
            </Rimble.Tooltip>
          </strong>
        </small>
        <br />
        <S> {monthlyPledgeEth ++ " ETH"} </S>
        <br />
        <small> <S> {"(" ++ monthlyPledgeUsd ++ " USD)"} </S> </small>
      </div>
      <p>
        <small>
          <strong>
            <S> "Current Patron: " </S>
            <Rimble.Tooltip
              message="This is the address of the current owner (crypto wallet 'account' number)"
              placement="top">
              <span> <S> {js|ⓘ|js} </S> </span>
            </Rimble.Tooltip>
          </strong>
        </small>
        <br />
        <S> currentPatron </S>
      </p>
      // <p> <S> {"Time Held: " ++ timeHeld} </S> </p>
      <p>
        <small>
          <strong>
            <S> "Available Deposit: " </S>
            <Rimble.Tooltip
              message="This is the amount the owner has deposited to pay their monthly contribution"
              placement="top">
              <span> <S> {js|ⓘ|js} </S> </span>
            </Rimble.Tooltip>
          </strong>
        </small>
        <br />
        <S> {depositAvailableToWithdraw ++ " ETH"} </S>
        <br />
        <small>
          <S> {"(" ++ depositAvailableToWithdrawUsd ++ " USD)"} </S>
        </small>
      </p>
      <p>
        <small>
          <strong>
            <S> {Gorilla.getName(gorilla) ++ "'s Patronage: "} </S>
            <Rimble.Tooltip
              message={
                "This is the total contribution that has been raised for the gorilla, "
                ++ Gorilla.getName(gorilla)
              }
              placement="top">
              <span> <S> {js|ⓘ|js} </S> </span>
            </Rimble.Tooltip>
          </strong>
        </small>
        <br />
        <S> {totalPatronage ++ " ETH"} </S>
        <br />
        <small> <S> {"(" ++ totalPatronageUsd ++ " USD)"} </S> </small>
      </p>
      {switch (definiteTime) {
       | Date(date) =>
         <p>
           <small>
             <strong>
               <S> "Foreclosure date: " </S>
               <Rimble.Tooltip
                 message={
                   "This is the date the deposit will run out and the gorilla and the current owner will lose ownership of "
                   ++ Gorilla.getName(gorilla)
                 }
                 placement="top">
                 <span> <S> {js|ⓘ|js} </S> </span>
               </Rimble.Tooltip>
             </strong>
           </small>
           <br />
           <S> {MomentRe.Moment.format("LLLL", date)} </S>
           <br />
           <small>
             <S> "( " </S>
             <CountDownForeclosure endDateMoment=date />
             <S> ")" </S>
           </small>
         </p>
       | Loading => React.null
       }}
    </Rimble.Box>;
  };
};

[@react.component]
// The Offline container here shows the website, but without loading the requirements
let make = () => {
  open Gorilla;
  let url = ReasonReactRouter.useUrl();
  let (isDetailView, gorillaStr) = {
    switch (Js.String.split("/", url.hash)) {
    | [|"details", gorillaStr|] => (true, gorillaStr)
    | _ => (false, "")
    };
  };

  <Rimble.Flex
    flexWrap={isDetailView ? "wrap-reverse" : "wrap"} alignItems="center">
    <Rimble.Box p=1 width=[|1., 1., 0.5|]>
      <React.Fragment>
        {isDetailView
           ? {
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
           }
           : <DefaultLeftPanel />}
      </React.Fragment>
    </Rimble.Box>
    <Rimble.Box p=1 width=[|1., 1., 0.5|]>
      <Providers.UsdPriceProvider>
        <Offline
          requireSmartContractsLoaded=true
          alturnateLoaderWeb3={<DefaultLook key="a" />}
          alturnateLoaderSmartContracts={<DefaultLook key="b" />}>
          <DefaultLook areRequirementsLoaded=true key="c" />
        </Offline>
      </Providers.UsdPriceProvider>
    </Rimble.Box>
  </Rimble.Flex>; //alturnativeNoWeb3=DefaultLook
};