open Hooks;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

// Load styles for the carousel and react-tabs
[%bs.raw {|require('@brainhubeu/react-carousel/lib/style.css')|}];
[%bs.raw {|require('react-tabs/style/react-tabs.css')|}];

let flameImg = [%bs.raw {|require('../img/streak-flame.png')|}];

// TODO: there must be a better way of importing images in reason react...
module ShareSocial = {
  [@bs.module "./components/shareSocialMedia"] [@react.component]
  external make: unit => React.element = "default";
};
module CountDown = {
  [@bs.module "./components/CountDown"] [@react.component]
  external make:
    (
      ~endDateMoment: MomentRe.Moment.t,
      ~includeWords: bool=? /*default true*/,
      ~leadingZeros: bool=?
    ) /*default false*/ =>
    React.element =
    "default";
};

module EditButton = {
  [@react.component]
  let make = (~animal: Animal.t) => {
    <Rimble.Button
      onClick={event => {
        ReactEvent.Form.preventDefault(event);
        ReasonReactRouter.push(
          "#details/" ++ animal->Animal.getName->Js.Global.encodeURI,
        );
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
      ->Belt.Option.map(dateAquired => {
          let days = MomentRe.diff(MomentRe.momentNow(), dateAquired, `days);
          days > 18000. ? 0. : days;
        });

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

module DisplayAfterDate = {
  [@react.component]
  let make = (~endDateMoment, ~beforeComponent, ~afterComponent) => {
    let isBeforeDate = () =>
      MomentRe.diff(endDateMoment, MomentRe.momentNow(), `seconds) > 0.;

    let (beforeDate, setIsBeforeDate) = React.useState(() => isBeforeDate());
    React.useEffect0(() => {
      let _timout =
        Js.Global.setTimeout(
          () => setIsBeforeDate(_ => isBeforeDate()),
          1500,
        );
      None;
    });

    beforeDate
      ? {
        beforeComponent;
      }
      : {
        afterComponent;
      };
  };
};

module BasicAnimalDisplay = {
  [@react.component]
  let make = (~animal: Animal.t) => {
    let owned = animal->Animal.useIsAnimalOwened;
    let currentPatron =
      GeneralHooks.useCurrentPatronAnimal(animal)
      ->mapWithDefault("Loading", a => a);
    let userId = UserProvider.useUserNameOrTwitterHandle(currentPatron);

    let userIdComponent = UserProvider.useUserComponent(userId);

    <React.Fragment>
      <PriceDisplay animal />
      userIdComponent
      {owned ? <EditButton animal /> : <BuyModal animal />}
    </React.Fragment>;
  };
};

module AnimalOnLandingPage = {
  [@react.component]
  let make =
      (
        ~animal,
        ~scalar: float=1.,
        ~optionEndDateMoment: option(MomentRe.Moment.t),
      ) => {
    let name = Animal.getName(animal);

    let optAlternateImage = Animal.getAlternateImage(animal);
    let optOrgBadge = Animal.getOrgBadgeImage(animal);

    let normalImage = () =>
      <img
        className={Styles.headerImg(1.5, scalar)}
        src={Animal.getImage(animal)}
      />;

    let componentWithoutImg = (img, ~hideBadges: bool) => {
      <React.Fragment>
        {img()}
        {if (hideBadges) {
           React.null;
         } else {
           <React.Fragment>
             {switch (optionEndDateMoment) {
              | Some(_endDateMoment) => React.null
              | None =>
                <div className=Styles.overlayFlameImg>
                  <Offline requireSmartContractsLoaded=true>
                    <Streak animal />
                  </Offline>
                </div>
              }}
             {switch (optOrgBadge) {
              | None => React.null
              | Some(orgBadge) =>
                <div className=Styles.overlayBadgeImg>
                  <img className=Styles.flameImg src=orgBadge />
                </div>
              }}
           </React.Fragment>;
         }}
      </React.Fragment>;
    };

    <Rimble.Box>
      <div className=Styles.positionRelative>
        {switch (optAlternateImage) {
         | None => componentWithoutImg(normalImage, ~hideBadges=false)
         | Some(alternateImage) =>
           <Components.HoverToggle
             _ComponentNoHover={componentWithoutImg(
               normalImage,
               ~hideBadges=false,
             )}
             _ComponentHover={componentWithoutImg(
               () =>
                 <img
                   className={Styles.headerImg(1.5, scalar)}
                   src=alternateImage
                 />,
               ~hideBadges=true,
             )}
           />
         }}
        <a
          className=Styles.clickableLink
          onClick={event => {
            ReactEvent.Mouse.preventDefault(event);
            ReasonReactRouter.push("#details/" ++ name->Js.Global.encodeURI);
          }}>
          <div> <h2> {React.string(name)} </h2> </div>
        </a>
      </div>
      {switch (optionEndDateMoment) {
       | Some(endDateMoment) =>
         <div>
           <h3 className=Styles.colorGreen> {React.string("COMING IN")} </h3>
           <CountDown endDateMoment leadingZeros=true includeWords=false />
         </div>
       | None =>
         <div>
           <Offline requireSmartContractsLoaded=true>
             <BasicAnimalDisplay animal />
           </Offline>
         </div>
       }}
    </Rimble.Box>;
  };
};

module CarouselAnimal = {
  [@react.component]
  let make = (~animal, ~scalar) => {
    let isLaunched = animal->Animal.isLaunched;
    switch (isLaunched) {
    | Animal.Launched =>
      <AnimalOnLandingPage animal scalar optionEndDateMoment=None />
    | Animal.LaunchDate(endDateMoment) =>
      <DisplayAfterDate
        endDateMoment
        afterComponent={
          <AnimalOnLandingPage animal scalar optionEndDateMoment=None />
        }
        beforeComponent={
          <AnimalOnLandingPage
            animal
            scalar
            optionEndDateMoment={Some(endDateMoment)}
          />
        }
      />
    };
  };
};

let animalCarouselArray = [|
  Animal.Apthapi,
  // Animal.Ajayu,
  Animal.Andy,
  Animal.Vitalik,
  Animal.Simon,
  Animal.Dlala,
  Animal.Cubai,
  Animal.Verano,
  // Animal.Tarkus,
  Animal.Pancho,
  // Animal.Mijungla,
  Animal.Llajuita,
  // Animal.Espumita,
  Animal.CatStevens,
  Animal.Aruma,
  Animal.Nonhlanhla,
  Animal.Isisa,
|];

module AnimalCarousel = {
  // let make = (~ownedAnimal: array(Animal)) => {
  [@react.component]
  let make = () => {
    let (carouselIndex, setCarouselIndex) = React.useState(() => 17);
    let numItems = animalCarouselArray->Array.length;
    <Rimble.Box className=Styles.positionRelative>
      <Carousel
        className=Styles.carousel
        slidesPerPage=5
        centered=true
        value=carouselIndex
        animationSpeed=1000
        infinite=true
        autoPlay=5000
        onChange={test => setCarouselIndex(_ => test)}>
        {ReasonReact.array(
           Array.mapi(
             (index, animal) => {
               let (opacity, scalar) =
                 switch (index) {
                 | x when x == carouselIndex mod numItems => (1., 1.0)
                 | x
                     when
                       x == (carouselIndex - 1)
                       mod numItems
                       || x == (carouselIndex + 1)
                       mod numItems => (
                     0.8,
                     0.8,
                   )
                 | x
                     when
                       x == (carouselIndex - 2)
                       mod numItems
                       || x == (carouselIndex + 2)
                       mod numItems => (
                     0.1,
                     0.7,
                   )
                 | _ => (0., 0.6)
                 };
               <div className={Styles.fadeOut(opacity)}>
                 <CarouselAnimal animal scalar />
               </div>;
             },
             animalCarouselArray,
           ),
         )}
      </Carousel>
    </Rimble.Box>;
  };
};

module AnimalActionsOnDetailsPage = {
  [@react.component]
  let make = (~animal) => {
    let owned = animal->Animal.useIsAnimalOwened;
    let currentAccount = useCurrentUser()->mapWithDefault("loading", a => a);
    let currentPatron =
      GeneralHooks.useCurrentPatronAnimal(animal)
      ->mapWithDefault("Loading", a => a);
    let userId = UserProvider.useUserNameOrTwitterHandle(currentPatron);
    let userIdComponent = UserProvider.useUserComponent(userId);

    let price = () =>
      <React.Fragment>
        userIdComponent
        <PriceDisplay animal />
        <BuyModal animal />
      </React.Fragment>;

    if (owned) {
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
        {switch (animal->Animal.isLaunched) {
         | Launched => price()

         | LaunchDate(endDateMoment) =>
           <DisplayAfterDate
             endDateMoment
             afterComponent={price()}
             beforeComponent={
               <React.Fragment> <CountDown endDateMoment /> </React.Fragment>
             }
           />
         }}
      </React.Fragment>;
    };
  };
};

module DefaultLook = {
  [@react.component]
  let make = () => {
    open Components;
    let setProvider = useSetProvider();
    React.useEffect0(() => {
      open Web3connect.Core;
      let core = getCore(None); // TOGGLE THE ABOVE LINE OUT BEFORE PRODUCTION!!
      core->setOnConnect(setProvider);
      None;
    });

    let url = ReasonReactRouter.useUrl();

    <div className=Styles.rightTopHeader>
      {switch (Js.String.split("/", url.hash)) {
       | [|"details", animalStr|] =>
         let optionAnimal = Animal.getAnimal(animalStr);

         switch (optionAnimal) {
         | None =>
           <div>
             <h1>
               {React.string(
                  "We are unable to find a animal by the name of \""
                  ++ {animalStr->Js.Global.decodeURI}
                  ++ "\" in our system.",
                )}
             </h1>
             <p> <S> "Please check the spelling and try again." </S> </p>
           </div>
         | Some(animal) =>
           let normalImage = animal =>
             <img
               className=Styles.ownedAnimalImg
               src={Animal.getImage(animal)}
             />;
           let optAlternateImage = Animal.getAlternateImage(animal);
           let optOrgBadge = Animal.getOrgBadgeImage(animal);

           let isLaunched = animal->Animal.isLaunched;

           let displayAnimal = animalImage =>
             <div className=Styles.positionRelative>
               {animalImage()}
               {switch (isLaunched) {
                | Animal.Launched =>
                  <div className=Styles.overlayFlameImg>
                    <Offline requireSmartContractsLoaded=true>
                      <Streak animal />
                    </Offline>
                  </div>
                | Animal.LaunchDate(endDateMoment) =>
                  <DisplayAfterDate
                    endDateMoment
                    afterComponent={
                      <div className=Styles.overlayFlameImg>
                        <Offline requireSmartContractsLoaded=true>
                          <Streak animal />
                        </Offline>
                      </div>
                    }
                    beforeComponent=React.null
                  />
                }}
               {switch (optOrgBadge) {
                | None => React.null
                | Some(orgBadge) =>
                  <div className=Styles.overlayBadgeImg>
                    <img className=Styles.flameImg src=orgBadge />
                  </div>
                }}
             </div>;

           <React.Fragment>
             {switch (optAlternateImage) {
              | None => displayAnimal(() => normalImage(animal))
              | Some(alternateImage) =>
                <Components.HoverToggle
                  _ComponentHover={
                    <img className=Styles.ownedAnimalImg src=alternateImage />
                  }
                  _ComponentNoHover={displayAnimal(() => normalImage(animal))}
                />
              }}
             <h2> <S> {Animal.getName(animal)} </S> </h2>
             <Offline requireSmartContractsLoaded=true>
               <AnimalActionsOnDetailsPage animal />
             </Offline>
           </React.Fragment>;
         };
       | _ =>
         <React.Fragment>
           <AnimalCarousel />
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

module AnimalInfoStats = {
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

    <React.Fragment>
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
             <CountDown endDateMoment=date />
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
    </React.Fragment>;
  };
};

module AnimalInfo = {
  [@react.component]
  let make = (~animal: Animal.t) => {
    // TODO: the ethereum address is really terribly displayed. But the default in Rimble UI includes a QR code scanner (which is really ugly).
    // https://rimble.consensys.design/components/rimble-ui/EthAddress#props
    // https://github.com/ConsenSys/rimble-ui/blob/dd470f00374a05c860b558a2cb9317861e4a0d89/src/EthAddress/index.js (maybe make a PR here with some changes)
    <Rimble.Box m=5>
      <ReactTabs>
        <ReactTabs.TabList>
          <ReactTabs.Tab> "Story"->React.string </ReactTabs.Tab>
          <ReactTabs.Tab> "Details"->React.string </ReactTabs.Tab>
        </ReactTabs.TabList>
        <ReactTabs.TabPanel>
          <h2> "Story"->React.string </h2>
          {ReasonReact.array(
             Array.map(
               paragraphText => <p> paragraphText->React.string </p>,
               Animal.getStoryParagraphs(animal),
             ),
           )}
        </ReactTabs.TabPanel>
        <ReactTabs.TabPanel>
          {switch (animal->Animal.isLaunched) {
           | Launched => <AnimalInfoStats animal />
           | LaunchDate(endDateMoment) =>
             <DisplayAfterDate
               endDateMoment
               afterComponent={<AnimalInfoStats animal />}
               beforeComponent={
                 <React.Fragment>
                   <h2> "This animal will launch in:"->React.string </h2>
                   <CountDown endDateMoment />
                 </React.Fragment>
               }
             />
           }}
        </ReactTabs.TabPanel>
      </ReactTabs>
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
    flexWrap={isDetailView ? "wrap-reverse" : "wrap"} alignItems="start">
    <Rimble.Box p=1 width=[|1., 1., 0.5|]>
      <React.Fragment>
        {isDetailView
           ? {
             let optionAnimal = Animal.getAnimal(animalStr);
             switch (optionAnimal) {
             | None => <DefaultLeftPanel />
             | Some(animal) =>
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
        <DefaultLook />
      </Providers.UsdPriceProvider>
    </Rimble.Box>
  </Rimble.Flex>;
};
