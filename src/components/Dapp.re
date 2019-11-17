open Hooks;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

let flameImg = [%bs.raw {|require('../img/streak-flame.png')|}];

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
  [@react.component]
  let make = (~animal: Animal.t) => {
    <Rimble.Button
      onClick={event => {
        ReactEvent.Form.preventDefault(event);
        ReasonReactRouter.push("#details/" ++ Animal.getName(animal));
      }}>
      {React.string("Edit")}
    </Rimble.Button>;
  };
};

module Streak = {
  [@react.component]
  let make = (~animal: Animal.t) => {
    let animalName = Animal.getName(animal);

    let daysHeld =
      PureHooks.useTimeAcquiredAnimal(animal)
      ->Belt.Option.map(dateAquired =>
          MomentRe.diff(MomentRe.momentNow(), dateAquired, `days)
        );

    switch (daysHeld) {
    | Some(daysHeldFloat) =>
      let numDaysStr = daysHeldFloat->Js.Float.toFixed;

      <Rimble.Tooltip
        message={j|$animalName has been held for $numDaysStr days by the same owner.|j}
        placement="top">
        <div className=Styles.positionRelative>
          <img className=Styles.flameImg src=flameImg />
          <p className=Styles.streakText>
            <strong> {React.string(numDaysStr)} </strong>
          </p>
        </div>
      </Rimble.Tooltip>;
    | None => React.null
    };
  };
};

//  <Rimble.Flex className=Styles.animalBox>
//            <Rimble.Box>
//              <div className=Styles.animalBack>
//                <img className=Styles.headerImg src=animal2 />
//                <div className=Styles.animalText>
//                  <h2> {React.string("Simon")} </h2>
//                  <h3 className=Styles.colorGreen>
//                    {React.string("COMING IN")}
//                  </h3>
//                  <Countdown />
//                </div>
//              </div>
//            </Rimble.Box>
//            //  <Offline requireSmartContractsLoaded=true>
//            //   //  <PriceDisplay tokenId={Some("0")} />
//            //   //  <BuyModal tokenId={Some("0")} />
//            //  </Offline>
//            <Rimble.Box>
//              <img className=Styles.headerImg src=animal1 />
//              <div>
//                <div className=Styles.animalText>
//                  <h2> {React.string("Vitalik")} </h2>
//                  <Offline requireSmartContractsLoaded=true>
//                    <PriceDisplay tokenId=None />
//                    <BuyModal tokenId=None />
//                  </Offline>
//                </div>
//              </div>
//            </Rimble.Box>
//            <Rimble.Box>
//              <div className=Styles.animalBack>
//                <img className=Styles.headerImg src=animal3 />
//                <div className=Styles.animalText>
//                  <h2> {React.string("Andy")} </h2>
//                  <h3 className=Styles.colorGreen>
//                    {React.string("COMING IN")}
//                  </h3>
//                  <Countdown />
//                </div>
//              </div>
//            </Rimble.Box>
//          </Rimble.Flex>

module AnimalOnLandingPage = {
  [@react.component]
  let make = (~animal: Animal.t, ~owned: bool) => {
    let name = Animal.getName(animal);

    <Rimble.Box>
      <a
        className=Styles.clickableLink
        onClick={event => {
          ReactEvent.Mouse.preventDefault(event);
          ReasonReactRouter.push("#details/" ++ name);
        }}>
        <div className=Styles.positionRelative>
          <img
            className={Styles.headerImg(150.)}
            src={Animal.getImage(animal)}
          />
          <div className=Styles.overlayFlameImg>
            <Offline requireSmartContractsLoaded=true>
              <Streak animal />
            </Offline>
          </div>
        </div>
        <div className=Styles.animalText>
          <h2> {React.string(name)} </h2>
        </div>
      </a>
      <div className=Styles.animalText>
        <Offline requireSmartContractsLoaded=true>
          <PriceDisplay animal />
          {owned ? <EditButton animal /> : <BuyModal animal />}
        </Offline>
      </div>
    </Rimble.Box>;
  };
};

module DefaultLook = {
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

    let (ownVitalik, ownSimon, ownAndy, currentAccount) =
      if (areRequirementsLoaded) {
        // NOTE/TODO: this doesn't take into account token ownership
        let currentPatronVitalik =
          useCurrentPatron()->mapWithDefault("no-patron-defined", a => a);
        let currentPatronSimon =
          useCurrentPatronNew(0)->mapWithDefault("no-patron-defined", a => a);
        let currentPatronAndy =
          useCurrentPatronNew(1)->mapWithDefault("no-patron-defined", a => a);
        let currentAccount =
          useCurrentUser()->mapWithDefault("loading", a => a);

        (
          currentAccount == currentPatronVitalik,
          currentAccount == currentPatronSimon,
          currentAccount == currentPatronAndy,
          currentAccount,
        );
      } else {
        (false, false, false, "loading");
      };
    let url = ReasonReactRouter.useUrl();

    <div className=Styles.rightTopHeader>
      {switch (Js.String.split("/", url.hash)) {
       | [|"details", animalStr|] =>
         let animal = Animal.getAnimal(animalStr);

         switch (animal) {
         | NoAnimal =>
           <div>
             <h1>
               {React.string(
                  "We are unable to find a animal by the name of "
                  ++ animalStr
                  ++ " in our system.",
                )}
             </h1>
             <p> <S> "Please check the spelling and try again." </S> </p>
           </div>
         | _ =>
           <React.Fragment>
             <img
               className=Styles.ownedAnimalImg
               src={Animal.getImage(animal)}
             />
             <h2> <S> {Animal.getName(animal)} </S> </h2>
             <Offline requireSmartContractsLoaded=true>
               {
                 let isYours =
                   switch (animal) {
                   | Vitalik => ownVitalik
                   | Simon => ownSimon
                   | Andy => ownAndy
                   | _ => false
                   };

                 if (isYours) {
                   <React.Fragment>
                     <UpdatePriceModal animal />
                     <br />
                     <UpdateDeposit animal />
                     <br />
                     {UserProvider.useIsUserValidated(currentAccount)
                        ? <ShareSocial /> : <ValidateModal />}
                   </React.Fragment>;
                 } else {
                   <React.Fragment>
                     <PriceDisplay animal />
                     <BuyModal animal />
                   </React.Fragment>;
                 };
               }
             </Offline>
           </React.Fragment>
         };
       | _ =>
         <React.Fragment>
           <Rimble.Flex className=Styles.animalBox>
             <AnimalOnLandingPage animal=Simon owned=ownSimon />
             <AnimalOnLandingPage animal=Vitalik owned=ownVitalik />
             <AnimalOnLandingPage animal=Andy owned=ownAndy />
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
        <S> {translation(. "subHeading")} </S>
      </h3>
    </React.Fragment>;
  };
};
type maybeDate =
  | Loading
  | Date(MomentRe.Moment.t);

module AnimalInfo = {
  [@react.component]
  let make = (~animal: Animal.t) => {
    let animalName = Animal.getName(animal);

    let daysHeld =
      PureHooks.useTimeAcquiredAnimal(animal)
      ->Belt.Option.map(dateAquired =>
          (
            MomentRe.diff(MomentRe.momentNow(), dateAquired, `days),
            dateAquired,
          )
        );

    let currentPatron =
      GeneralHooks.useCurrentPatronAnimal(animal)
      ->mapWithDefault("Loading", a => a);
    let userId = UserProvider.useUserNameOrTwitterHandle(currentPatron);
    let userIdType =
      switch (userId) {
      | EthAddress(_) => "public address"
      | TwitterHandle(_) => "verified twitter account"
      };
    let userIdComponent = UserProvider.useUserComponent(userId);
    let depositAvailableToWithdraw =
      GeneralHooks.useDepositAbleToWithdrawEthAnimal(animal)
      ->mapWithDefault("Loading", a => a);
    let depositAvailableToWithdrawUsd =
      GeneralHooks.useDepositAbleToWithdrawUsdAnimal(animal)
      ->mapWithDefault("Loading", a => a);
    let totalPatronage = GeneralHooks.useTotalPatronageEthAnimal(animal);
    let totalPatronageUsd =
      GeneralHooks.useTotalPatronageUsdAnimal(animal)
      ->mapWithDefault("Loading", a => a);
    let foreclosureTime = GeneralHooks.useForeclosureTimeAnimal(animal);
    let definiteTime = foreclosureTime->mapWithDefault(Loading, a => Date(a));
    let (_, _, ratio, _) = Animal.pledgeRate(animal);
    let currentPrice = Animal.useCurrentPriceEth(animal);
    let currentPriceUsd = Animal.useCurrentPriceUsd(animal);
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
                ++ animalName
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
              message={j|This is the $userIdType of the current owner|j}
              placement="top">
              <span> <S> {js|ⓘ|js} </S> </span>
            </Rimble.Tooltip>
          </strong>
        </small>
        <br />
        userIdComponent
      </p>
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
            <S> {animalName ++ "'s Patronage: "} </S>
            <Rimble.Tooltip
              message={
                "This is the total contribution that has been raised thanks to the wildcard, "
                ++ animalName
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
                   "This is the date the deposit will run out and the animal and the current owner will lose ownership of "
                   ++ animalName
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
      {switch (daysHeld) {
       | Some((daysHeldFloat, timeAquired)) =>
         let timeAquiredString = timeAquired->MomentRe.Moment.toISOString;
         <p>
           <small>
             <strong>
               <S> "Days Held: " </S>
               <Rimble.Tooltip
                 message={j|This is the amount of time $animalName has been held. It was aquired on the $timeAquiredString.|j}
                 placement="top">
                 <span> <S> {js|ⓘ|js} </S> </span>
               </Rimble.Tooltip>
             </strong>
           </small>
           <br />
           <S> daysHeldFloat->Js.Float.toFixed </S>
           <br />
         </p>;
       | None => React.null
       }}
    </Rimble.Box>;
  };
};

[@react.component]
// The Offline container here shows the website, but without loading the requirements
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let (isDetailView, animalStr) = {
    switch (Js.String.split("/", url.hash)) {
    | [|"details", animalStr|] => (true, animalStr)
    | _ => (false, "")
    };
  };

  <Rimble.Flex
    flexWrap={isDetailView ? "wrap-reverse" : "wrap"} alignItems="center">
    <Rimble.Box p=1 width=[|1., 1., 0.5|]>
      <React.Fragment>
        {isDetailView
           ? {
             let animal = Animal.getAnimal(animalStr);
             switch (animal) {
             | NoAnimal => <DefaultLeftPanel />
             | _ =>
               <Providers.UsdPriceProvider>
                 <Offline requireSmartContractsLoaded=true>
                   <AnimalInfo animal />
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
