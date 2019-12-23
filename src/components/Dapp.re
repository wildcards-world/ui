open Hooks;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

// Load styles for the carousel and react-tabs
[%bs.raw {|require('@wildcards/react-carousel/lib/style.css')|}];
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
  let make = (~animal: Animal.t, ~isExplorer) => {
    <Rimble.Button
      onClick={event => {
        ReactEvent.Form.preventDefault(event);
        ReasonReactRouter.push(
          "#"
          ++ InputHelp.getPagePrefix(isExplorer)
          ++ "details/"
          ++ animal->Animal.getName->Js.Global.encodeURI,
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

    let daysHeld = QlHooks.useDaysHeld(animal);

    // Js.log("Streak");
    // Js.log(daysHeld);

    switch (daysHeld) {
    | Some((daysHeldFloat, _timeAquired)) =>
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
  let make = (~animal: Animal.t, ~isExplorer) => {
    let owned = animal->QlHooks.useIsAnimalOwened;
    let currentPatron =
      QlHooks.usePatron(animal)->mapWithDefault("Loading", a => a);
    let userId = UserProvider.useUserNameOrTwitterHandle(currentPatron);

    let userIdComponent = UserProvider.useUserComponent(userId);

    <React.Fragment>
      <PriceDisplay animal />
      userIdComponent
      <br />
      {owned ? <EditButton animal isExplorer /> : <BuyModal animal />}
    </React.Fragment>;
  };
};

module AnimalOnLandingPage = {
  [@react.component]
  let make =
      (
        ~animal,
        ~scalar: float=1.,
        ~enlargement: float=1.,
        ~optionEndDateMoment: option(MomentRe.Moment.t),
        ~isExplorer,
      ) => {
    let name = Animal.getName(animal);

    let optAlternateImage = Animal.getAlternateImage(animal);
    let orgBadge = Animal.getOrgBadgeImage(animal);

    let normalImage = () =>
      <img
        className={Styles.headerImg(enlargement, scalar)}
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
                <div className=Styles.overlayFlameImg> <Streak animal /> </div>
              }}
             {<div className=Styles.overlayBadgeImg>
                <img className=Styles.flameImg src=orgBadge />
              </div>}
           </React.Fragment>;
         }}
      </React.Fragment>;
    };

    let imageHoverSwitcher = {
      switch (optAlternateImage) {
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
                className={Styles.headerImg(enlargement, scalar)}
                src=alternateImage
              />,
            ~hideBadges=true,
          )}
        />
      };
    };

    <Rimble.Box className=Styles.centerText>
      <div className=Styles.positionRelative>
        <a
          className=Styles.clickableLink
          onClick={event => {
            ReactEvent.Mouse.preventDefault(event);
            ReasonReactRouter.push(
              "#"
              ++ InputHelp.getPagePrefix(isExplorer)
              ++ "details/"
              ++ name->Js.Global.encodeURI,
            );
          }}>
          imageHoverSwitcher
          <div> <h2> {React.string(name)} </h2> </div>
        </a>
      </div>
      {switch (optionEndDateMoment) {
       | Some(endDateMoment) =>
         <div>
           <h3 className=Styles.colorGreen> {React.string("COMING IN")} </h3>
           <CountDown endDateMoment leadingZeros=true includeWords=false />
         </div>
       | None => <div> <BasicAnimalDisplay animal isExplorer /> </div>
       }}
    </Rimble.Box>;
  };
};

module CarouselAnimal = {
  [@react.component]
  let make = (~animal, ~scalar, ~isExplorer, ~enlargement: float=1.) => {
    let isLaunched = animal->Animal.isLaunched;

    switch (animal) {
    | Animal.Andy =>
      Js.log("render AnimalCarousel root again: " ++ animal->Animal.getName)
    //  | Animal.Simon => Js.log("render AnimalCarousel root again: " ++ animal->Animal.getName);
    | _ => ()
    };

    switch (isLaunched) {
    | Animal.Launched =>
      <AnimalOnLandingPage
        animal
        scalar
        optionEndDateMoment=None
        isExplorer
        enlargement
      />
    | Animal.LaunchDate(endDateMoment) =>
      <DisplayAfterDate
        endDateMoment
        afterComponent={
          <AnimalOnLandingPage
            animal
            isExplorer
            scalar
            optionEndDateMoment=None
            enlargement
          />
        }
        beforeComponent={
          <AnimalOnLandingPage
            animal
            scalar
            isExplorer
            optionEndDateMoment={Some(endDateMoment)}
            enlargement
          />
        }
      />
    };
  };
};

module AnimalCarousel = {
  [@react.component]
  let make = (~isExplorer) => {
    let (carouselIndex, setCarouselIndex) = React.useState(() => 17);
    let numItems = Animal.orderedArray->Array.length;

    Js.log("render AnimalCarousel root again");

    <Rimble.Box className=Styles.positionRelative>
      <Carousel
        className=Styles.carousel
        slidesPerPage=5
        centered=true
        value=carouselIndex
        animationSpeed=1000
        infinite=true
        autoPlay=5000
        arrowLeft={
          <span
            className={Styles.carouselArrow(true)}
            onClick={event => {
              ReactEvent.Mouse.preventDefault(event);
              setCarouselIndex(_ => carouselIndex - 1);
              ReactEvent.Mouse.stopPropagation(event);
            }}>
            {js|◄|js}->React.string
          </span>
        }
        arrowRight={
          <span
            className={Styles.carouselArrow(false)}
            onClick={event => {
              ReactEvent.Mouse.preventDefault(event);
              setCarouselIndex(_ => carouselIndex + 1);
              ReactEvent.Mouse.stopPropagation(event);
            }}>
            {js|►|js}->React.string
          </span>
        }
        arrows=true
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
                 <CarouselAnimal animal isExplorer scalar enlargement=1.5 />
               </div>;
             },
             Animal.orderedArray,
           ),
         )}
      </Carousel>
    </Rimble.Box>;
  };
};

module AnimalActionsOnDetailsPage = {
  [@react.component]
  let make = (~animal) => {
    let owned = animal->QlHooks.useIsAnimalOwened;
    let currentAccount = useCurrentUser()->mapWithDefault("loading", a => a);
    let currentPatron =
      QlHooks.usePatron(animal)->mapWithDefault("Loading", a => a);
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
        <PriceDisplay animal />
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

module DetailsView = {
  [@react.component]
  let make = (~animalStr) => {
    let optionAnimal = Animal.getAnimal(animalStr);

    switch (optionAnimal) {
    | None =>
      <div>
        <h1>
          {React.string(
             "We are unable to find a animal by the name of \""
             ++ {
               animalStr->Js.Global.decodeURI;
             }
             ++ "\" in our system.",
           )}
        </h1>
        <p> <S> "Please check the spelling and try again." </S> </p>
      </div>
    | Some(animal) =>
      let normalImage = animal =>
        <img className=Styles.ownedAnimalImg src={Animal.getImage(animal)} />;
      let optAlternateImage = Animal.getAlternateImage(animal);
      let orgBadge = Animal.getOrgBadgeImage(animal);

      let isLaunched = animal->Animal.isLaunched;

      let displayAnimal = animalImage =>
        <div className=Styles.positionRelative>
          {animalImage()}
          {switch (isLaunched) {
           | Animal.Launched =>
             <div className=Styles.overlayFlameImg> <Streak animal /> </div>
           | Animal.LaunchDate(endDateMoment) =>
             <DisplayAfterDate
               endDateMoment
               afterComponent={
                 <div className=Styles.overlayFlameImg>
                   <Streak animal />
                 </div>
               }
               beforeComponent=React.null
             />
           }}
          {<div className=Styles.overlayBadgeImg>
             <img className=Styles.flameImg src=orgBadge />
           </div>}
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
  };
};

module DefaultLook = {
  [@react.component]
  let make = (~isExplorer, ~isGqlLoaded) => {
    open Components;
    let setProvider = useSetProvider();
    React.useEffect0(() => {
      open Web3connect.Core;
      let core = getCore(None); // TOGGLE THE ABOVE LINE OUT BEFORE PRODUCTION!!
      core->setOnConnect(setProvider);
      None;
    });

    let url = ReasonReactRouter.useUrl();

    <div className=Styles.centerText>
      {switch (Js.String.split("/", url.hash)) {
       | [|"details", animalStr|]
       | [|"explorer", "details", animalStr|]
       | [|"explorer", "details", animalStr, ""|] => <DetailsView animalStr />
       | _ =>
         <React.Fragment>
           {isGqlLoaded ? <AnimalCarousel isExplorer /> : React.null}
           <Rimble.Box className=Styles.dappImagesCounteractOffset>
             {isGqlLoaded ? <TotalRaised /> : React.null}
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

    let daysHeld = QlHooks.useDaysHeld(animal);

    let currentPatron =
      QlHooks.usePatron(animal)->mapWithDefault("Loading", a => a);
    let userId = UserProvider.useUserNameOrTwitterHandle(currentPatron);
    let userIdType =
      switch (userId) {
      | EthAddress(_) => "public address"
      | TwitterHandle(_) => "verified twitter account"
      };
    let userIdComponent = UserProvider.useUserComponent(userId);
    let currentUsdEthPrice = Providers.UsdPriceProvider.useUsdPrice();
    let (depositAvailableToWithdrawEth, depositAvailableToWithdrawUsd) =
      // GeneralHooks.useDepositAbleToWithdrawEthAnimal(animal)
      QlHooks.useRemainingDepositEth(currentPatron)
      ->mapWithDefault(("Loading", "Loading"), a =>
          (
            a->Eth.get(Eth.Eth(`ether)),
            currentUsdEthPrice->Belt.Option.mapWithDefault(
              "Loading", usdEthRate =>
              a->Eth.get(Eth.Usd(usdEthRate, 2))
            ),
          )
        );

    let (totalPatronage, totalPatronageUsd) =
      QlHooks.useAmountRaisedToken(animal)
      ->mapWithDefault(("Loading", "Loading"), a =>
          (
            a->Eth.get(Eth.Eth(`ether)),
            currentUsdEthPrice->Belt.Option.mapWithDefault(
              "Loading", usdEthRate =>
              a->Eth.get(Eth.Usd(usdEthRate, 2))
            ),
          )
        );
    let foreclosureTime = QlHooks.useForeclosureTime(currentPatron);
    let definiteTime = foreclosureTime->mapWithDefault(Loading, a => Date(a));
    let (_, _, ratio, _) = Animal.pledgeRate(animal);

    let optCurrentPrice = PriceDisplay.uesTotalPatronage(animal);

    let (optMonthlyPledgeEth, optMonthlyPledgeUsd) =
      switch (optCurrentPrice) {
      | Some((priceEth, optPriceUsd)) => (
          Some(
            Js.Float.toString(
              Belt.Float.fromString(priceEth)->Accounting.defaultZeroF
              *. ratio,
            ),
          ),
          switch (optPriceUsd) {
          | Some(priceUsd) => None

          | None => None
          },
        )
      | None => (None, None)
      };

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
        {switch (optMonthlyPledgeEth) {
         | Some(monthlyPledgeEth) => <S> {monthlyPledgeEth ++ " ETH"} </S>
         | None => <Rimble.Loader />
         }}
        <br />
        <small>
          {switch (optMonthlyPledgeUsd) {
           | Some(monthlyPledgeUsd) =>
             <S> {"(" ++ monthlyPledgeUsd ++ " USD)"} </S>
           | None => React.null
           }}
        </small>
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
        <S> {depositAvailableToWithdrawEth ++ " ETH"} </S>
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
                 message={j|This is the amount of time $animalName has been held. It was acquired on the $timeAquiredString.|j}
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
             Array.mapi(
               (i, paragraphText) =>
                 <p key={i->string_of_int}> paragraphText->React.string </p>,
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
  let isGqlLoaded = QlHooks.useIsInitialized();
  let (isDetailView, animalStr, isExplorer) = {
    switch (Js.String.split("/", url.hash)) {
    | [|"explorer", "details", animalStr|]
    | [|"explorer", "details", animalStr, ""|] => (true, animalStr, true)
    | [|"details", animalStr|] => (true, animalStr, false)
    | urlArray
        when
          Belt.Array.get(urlArray, 0)
          ->Belt.Option.mapWithDefault(false, a => a == "explorer") => (
        false,
        "",
        true,
      )
    | _ => (false, "", false)
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
               <Offline requireSmartContractsLoaded=true>
                 <AnimalInfo animal />
               </Offline>
             };
           }
           : <DefaultLeftPanel />}
      </React.Fragment>
    </Rimble.Box>
    <Rimble.Box p=1 width=[|1., 1., 0.5|]>
      <DefaultLook isExplorer isGqlLoaded />
    </Rimble.Box>
  </Rimble.Flex>;
  // <CarouselTestApp />
};
