open Globals;

// Load styles for the carousel and react-tabs
// [%bs.raw {|require('@wildcards/react-carousel/lib/style.css')|}];
// [%bs.raw {|require('react-tabs/style/react-tabs.css')|}];

let flameImg = "/img/streak-flame.png";

// TODO: there must be a better way of importing images in reason react...
module ShareSocial = {
  [@bs.module "./components/shareSocialMedia"] [@react.component]
  external make: unit => React.element = "default";
};
module EditButton = {
  [@react.component]
  let make = (~animal: TokenId.t) => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
    let isExplorer = Router.useIsExplorer();

    <Rimble.Button
      onClick={event => {
        ReactEvent.Form.preventDefault(event);
        clearAndPush(
          "#"
          ++ InputHelp.getPagePrefix(isExplorer)
          ++ "details/"
          ++ animal->TokenId.toString,
        );
      }}>
      {React.string("Edit")}
    </Rimble.Button>;
  };
};

module Streak = {
  [@react.component]
  let make = (~animal: TokenId.t) => {
    let animalName = QlHooks.useWildcardName(animal) |||| "Loading";

    let daysHeld = QlHooks.useDaysHeld(animal);

    switch (daysHeld) {
    | Some((daysHeldFloat, _timeAquired)) =>
      let numDaysStr = daysHeldFloat->Js.Float.toFixed;

      <Rimble.Tooltip
        className=Css.(style([width(`em(20.))]))
        message={j|$animalName has been held for $numDaysStr days by the same owner.|j}>
        // placement="bottom"

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
    let isBeforeDate =
      React.useCallback1(
        () =>
          MomentRe.diff(endDateMoment, MomentRe.momentNow(), `seconds) > 0.,
        [|endDateMoment|],
      );

    let (beforeDate, setIsBeforeDate) = React.useState(() => isBeforeDate());
    React.useEffect2(
      () => {
        let timeout =
          Js.Global.setTimeout(
            () => setIsBeforeDate(_ => isBeforeDate()),
            1500,
          );
        Some(() => Js.Global.clearTimeout(timeout));
      },
      (setIsBeforeDate, isBeforeDate),
    );

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
  let make = (~animal: TokenId.t) => {
    let owned = animal->QlHooks.useIsAnimalOwened;
    let currentPatron = QlHooks.usePatron(animal) |||| "Loading";
    let displayName = UserProvider.useDisplayName(currentPatron);

    let displayNameStr = UserProvider.displayNameToString(displayName);
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
    let nonUrlRouting = RootProvider.useNonUrlState();

    <React.Fragment>
      <PriceDisplay animal />
      <a
        onClick={e => {
          ReactEvent.Mouse.preventDefault(e);
          clearAndPush({j|/#user/$currentPatron|j});
        }}>
        displayNameStr->restr
      </a>
      <br />
      {switch (nonUrlRouting) {
       | UserVerificationScreen
       | UpdateDepositScreen
       | UpdatePriceScreen(_)
       | BuyScreen(_) => React.null
       | LoginScreen(_)
       | NoExtraState =>
         owned ? <EditButton animal /> : <ActionButtons.Buy animal />
       }}
    </React.Fragment>;
  };
};

module AnimalOnLandingPage = {
  [@react.component]
  let make =
      (
        ~animal: TokenId.t,
        ~scalar: float=1.,
        ~enlargement: float=1.,
        ~optionEndDateMoment: option(MomentRe.Moment.t),
        ~isGqlLoaded,
      ) => {
    let name = QlHooks.useWildcardName(animal) |||| "Loading";
    let isExplorer = Router.useIsExplorer();

    let orgBadge = Animal.useGetOrgBadgeImage(animal);
    let orgId = "wildtomorrow";

    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();

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
             {isGqlLoaded
                ? switch (optionEndDateMoment) {
                  | Some(_endDateMoment) => React.null
                  | None =>
                    <div className=Styles.overlayFlameImg>
                      <Streak animal />
                    </div>
                  }
                : React.null}
             {<div
                onClick={e => {
                  ReactEvent.Mouse.stopPropagation(e);
                  ReactEvent.Mouse.preventDefault(e);
                  clearAndPush(
                    "#"
                    ++ InputHelp.getPagePrefix(isExplorer)
                    ++ "org/"
                    ++ orgId,
                  );
                }}
                className=Styles.overlayBadgeImg>
                <img className=Styles.flameImg src=orgBadge />
              </div>}
           </React.Fragment>;
         }}
      </React.Fragment>;
    };

    <Rimble.Box className=Styles.centerText>
      <div className=Styles.positionRelative>
        <a
          className=Styles.clickableLink
          onClick={event => {
            ReactEvent.Mouse.preventDefault(event);
            Js.log("CLICKED OUTER IMAGE!!!");
            clearAndPush(
              "#"
              ++ InputHelp.getPagePrefix(isExplorer)
              ++ "details/"
              ++ animal->TokenId.toString,
            );
          }}>
          {componentWithoutImg(normalImage, ~hideBadges=false)}
          <div> <h2> {React.string(name)} </h2> </div>
        </a>
      </div>
      {switch (optionEndDateMoment) {
       | Some(endDateMoment) =>
         <div>
           <h3 className=Styles.colorGreen> {React.string("COMING IN")} </h3>
           <CountDown endDateMoment displayUnits=false />
         </div>
       | None =>
         isGqlLoaded ? <div> <BasicAnimalDisplay animal /> </div> : React.null
       }}
    </Rimble.Box>;
  };
};

module CarouselAnimal = {
  [@react.component]
  let make =
      (~animal: TokenId.t, ~scalar, ~enlargement: float=1., ~isGqlLoaded) => {
    let isLaunched = animal->Animal.isLaunched;

    let makeAnimalOnLandingPage = optionEndDateMoment =>
      <AnimalOnLandingPage
        animal
        scalar
        optionEndDateMoment
        enlargement
        isGqlLoaded
      />;
    switch (isLaunched) {
    | Animal.Launched => makeAnimalOnLandingPage(None)
    | Animal.LaunchDate(endDateMoment) =>
      <DisplayAfterDate
        endDateMoment
        afterComponent={makeAnimalOnLandingPage(None)}
        beforeComponent={makeAnimalOnLandingPage(Some(endDateMoment))}
      />
    };
  };
};

module AnimalCarousel = {
  [@react.component]
  let make = (~isGqlLoaded) => {
    let (carouselIndex, setCarouselIndex) = React.useState(() => 17);
    let homePageAnimals = QlHooks.useHomePageAnimalArray();
    let numItems = homePageAnimals->Array.length;
    // let numItems = Animal.orderedArray->Array.length;

    <Rimble.Box className=Styles.positionRelative>
      <Carousel
        className=Styles.carousel
        slidesPerPage=5
        centered=true
        value=carouselIndex
        animationSpeed=1000
        infinite=true
        autoPlay=500000
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
           homePageAnimals->Array.mapWithIndex((index, animalInfo) => {
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
               <CarouselAnimal
                 animal={animalInfo.id}
                 isGqlLoaded
                 scalar
                 enlargement=1.5
               />
             </div>;
           }),
         )}
      </Carousel>
    </Rimble.Box>;
  };
};

module AnimalActionsOnDetailsPage = {
  [@react.component]
  let make = (~animal) => {
    let owned = animal->QlHooks.useIsAnimalOwened;
    // let currentAccount =
    //   RootProvider.useCurrentUser()->mapWithDefault("loading", a => a);
    let currentPatron = QlHooks.usePatron(animal) |||| "Loading";
    let displayName = UserProvider.useDisplayName(currentPatron);
    let displayNameStr = UserProvider.displayNameToString(displayName);
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();

    let nonUrlRouting = RootProvider.useNonUrlState();

    let price = () =>
      <React.Fragment>
        <a
          onClick={e => {
            ReactEvent.Mouse.preventDefault(e);
            clearAndPush({j|/#user/$currentPatron|j});
          }}>
          displayNameStr->restr
        </a>
        <PriceDisplay animal />
        {switch (nonUrlRouting) {
         | UserVerificationScreen
         | UpdateDepositScreen
         | UpdatePriceScreen(_)
         | BuyScreen(_) => React.null
         | LoginScreen(_)
         | NoExtraState => <ActionButtons.Buy animal />
         }}
      </React.Fragment>;

    if (owned) {
      <React.Fragment>
        <PriceDisplay animal />
        <ActionButtons.UpdatePrice animal />
        <br />
        <ActionButtons.UpdateDeposit />
        <br />
        // {UserProvider.useIsUserValidated(currentAccount)
        //    ? <ShareSocial /> : <Validate />}
        <Validate />
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
  let make = (~optionAnimal: option(TokenId.t)) => {
    [%log.info "optionAnimal"; ("a", optionAnimal)];

    switch (optionAnimal) {
    | None =>
      <div>
        <h1>
          {React.string("We are unable to find that animal in our system.")}
        </h1>
        <p> "Please check the spelling and try again."->restr </p>
      </div>
    | Some(animal) =>
      let normalImage = animal =>
        <img className=Styles.ownedAnimalImg src={Animal.getImage(animal)} />;
      // let optAlternateImage = Animal.getAlternateImage(animal);
      let orgBadge = Animal.useGetOrgBadgeImage(animal);

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
        {displayAnimal(() => normalImage(animal))}
        <h2>
          {{
             QlHooks.useWildcardName(animal) |||| "Loading";
           }
           ->restr}
        </h2>
        <AnimalActionsOnDetailsPage animal />
      </React.Fragment>;
    };
  };
};

module DefaultLook = {
  [@react.component]
  let make = (~isGqlLoaded) => {
    let url = ReasonReactRouter.useUrl();

    <div className=Styles.centerText>
      {switch (Js.String.split("/", url.hash)) {
       | [|"details", animalStr|]
       | [|"explorer", "details", animalStr|]
       | [|"explorer", "details", animalStr, ""|] =>
         [%log.info "the animalString"; ("a", animalStr)];

         <DetailsView optionAnimal={TokenId.make(animalStr)} />;
       | _ =>
         <React.Fragment>
           <AnimalCarousel isGqlLoaded />
           <Rimble.Box className=Styles.dappImagesCounteractOffset>
             {isGqlLoaded ? <TotalRaised /> : React.null}
           </Rimble.Box>
         </React.Fragment>
       }}
    </div>;
  };
};

module DefaultLeftPanel = {
  [@react.component]
  let make = () => {
    let translationModeContext = ReactTranslate.useTranslationModeContext();
    let translation =
      ReactTranslate.useTranslate(. translationModeContext.translationMode);
    <React.Fragment>
      <h1 className=Styles.heading>
        <span className=Styles.colorBlue>
          {{
             translation(. "bluetext");
           }
           ->restr}
        </span>
        <br />
        {{
           translation(. "ethereum");
         }
         ->restr}
        <br />
        <span className=Styles.colorGreen> "conservation"->restr </span>
        {{
           " " ++ translation(. "tokens");
         }
         ->restr}
      </h1>
      <hr />
      <h3 className=Styles.subHeading>
        {{
           translation(. "subHeading");
         }
         ->restr}
      </h3>
    </React.Fragment>;
  };
};

type maybeDate =
  | Loading
  | Date(MomentRe.Moment.t);

module AnimalInfoStats = {
  [@react.component]
  let make = (~animal: TokenId.t) => {
    let animalName = QlHooks.useWildcardName(animal) |||| "Loading";

    let daysHeld = QlHooks.useDaysHeld(animal);

    let currentPatron = QlHooks.usePatron(animal) |||| "Loading";
    let userId = UserProvider.useDisplayName(currentPatron);
    let displayName = UserProvider.useDisplayName(currentPatron);
    let displayNameStr = UserProvider.displayNameToString(displayName);
    let userIdType =
      switch (userId) {
      | EthAddress(_) => "public address"
      | TwitterHandle(_) => "verified twitter account"
      | ThreeBoxName(_) => "3box name"
      };
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();

    let currentUsdEthPrice = UsdPriceProvider.useUsdPrice();
    let (depositAvailableToWithdrawEth, depositAvailableToWithdrawUsd) =
      QlHooks.useRemainingDepositEth(currentPatron)
      ->mapd(("Loading", "Loading"), a =>
          (
            (a->Eth.get(Eth.Eth(`ether))->Float.fromString |||| 0.0)
            ->toFixedWithPrecisionNoTrailingZeros(~digits=9),
            currentUsdEthPrice->mapd("Loading", usdEthRate =>
              a->Eth.get(Eth.Usd(usdEthRate, 2))
            ),
          )
        );

    let (totalPatronage, totalPatronageUsd) =
      QlHooks.useAmountRaisedToken(animal)
      ->mapd(("Loading", "Loading"), a =>
          (
            (a->Eth.get(Eth.Eth(`ether))->Float.fromString |||| 0.0)
            ->toFixedWithPrecisionNoTrailingZeros(~digits=9),
            currentUsdEthPrice->mapd("Loading", usdEthRate =>
              a->Eth.get(Eth.Usd(usdEthRate, 2))
            ),
          )
        );
    let foreclosureTime = QlHooks.useForeclosureTime(currentPatron);
    let definiteTime = foreclosureTime->mapd(Loading, a => Date(a));
    let ratio = QlHooks.usePledgeRate(animal);

    let optCurrentPrice = PriceDisplay.usePrice(animal);

    let (optMonthlyPledgeEth, optMonthlyPledgeUsd) =
      switch (optCurrentPrice) {
      | Some((priceEth, optPriceUsd)) => (
          Some(
            toFixedWithPrecisionNoTrailingZeros(
              Float.fromString(priceEth)->Accounting.defaultZeroF *. ratio,
              ~digits=4,
            ),
          ),
          switch (optPriceUsd) {
          | Some(_priceUsd) => None

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
            {{
               "Monthly Pledge (at " ++ monthlyRate ++ "%): ";
             }
             ->restr}
            <Rimble.Tooltip
              message={
                "This is the monthly percentage contribution of "
                ++ animalName
                ++ "'s sale price that will go towards conservation of endangered animals. This is deducted continuously from the deposit and paid by the owner of the animal"
              }
              placement="top">
              <span> {js|ⓘ|js}->restr </span>
            </Rimble.Tooltip>
          </strong>
        </small>
        <br />
        {switch (optMonthlyPledgeEth) {
         | Some(monthlyPledgeEth) =>
           {
             monthlyPledgeEth ++ " ETH";
           }
           ->restr
         | None => <Rimble.Loader />
         }}
        <br />
        <small>
          {switch (optMonthlyPledgeUsd) {
           | Some(monthlyPledgeUsd) =>
             {
               "(" ++ monthlyPledgeUsd ++ " USD)";
             }
             ->restr
           | None => React.null
           }}
        </small>
      </div>
      <p>
        <small>
          <strong>
            "Current Patron: "->restr
            <Rimble.Tooltip
              message={j|This is the $userIdType of the current owner|j}
              placement="top">
              <span> {js|ⓘ|js}->restr </span>
            </Rimble.Tooltip>
          </strong>
        </small>
        <br />
        <a
          onClick={e => {
            ReactEvent.Mouse.preventDefault(e);
            clearAndPush({j|/#user/$currentPatron|j});
          }}>
          displayNameStr->restr
        </a>
      </p>
      <p>
        <small>
          <strong>
            "Available Deposit: "->restr
            <Rimble.Tooltip
              message="This is the amount the owner has deposited to pay their monthly contribution"
              placement="top">
              <span> {js|ⓘ|js}->restr </span>
            </Rimble.Tooltip>
          </strong>
        </small>
        <br />
        {{
           depositAvailableToWithdrawEth ++ " ETH";
         }
         ->restr}
        <br />
        <small>
          {{
             "(" ++ depositAvailableToWithdrawUsd ++ " USD)";
           }
           ->restr}
        </small>
      </p>
      <p>
        <small>
          <strong>
            {{
               animalName ++ "'s Patronage: ";
             }
             ->restr}
            <Rimble.Tooltip
              message={
                "This is the total contribution that has been raised thanks to the wildcard, "
                ++ animalName
              }
              placement="top">
              <span> {js|ⓘ|js}->restr </span>
            </Rimble.Tooltip>
          </strong>
        </small>
        <br />
        {{
           totalPatronage ++ " ETH";
         }
         ->restr}
        <br />
        <small>
          {{
             "(" ++ totalPatronageUsd ++ " USD)";
           }
           ->restr}
        </small>
      </p>
      {switch (definiteTime) {
       | Date(date) =>
         <p>
           <small>
             <strong>
               "Foreclosure date: "->restr
               <Rimble.Tooltip
                 message={
                   "This is the date the deposit will run out and the animal and the current owner will lose ownership of "
                   ++ animalName
                 }
                 placement="top">
                 <span> {js|ⓘ|js}->restr </span>
               </Rimble.Tooltip>
             </strong>
           </small>
           <br />
           {{
              MomentRe.Moment.format("LLLL", date);
            }
            ->restr}
           <br />
           <small>
             "( "->restr
             <CountDown endDateMoment=date />
             ")"->restr
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
               "Days Held: "->restr
               <Rimble.Tooltip
                 message={j|This is the amount of time $animalName has been held. It was acquired on the $timeAquiredString.|j}
                 placement="top">
                 <span> {js|ⓘ|js}->restr </span>
               </Rimble.Tooltip>
             </strong>
           </small>
           <br />
           {daysHeldFloat->Js.Float.toFixed->restr}
           <br />
         </p>;
       | None => React.null
       }}
    </React.Fragment>;
  };
};

module UnlaunchedAnimalInfo = {
  [@react.component]
  let make = (~endDateMoment, ~animal: TokenId.t) => {
    let animalName = QlHooks.useWildcardName(animal) |||| "Loading";

    let ratio = QlHooks.usePledgeRate(animal);
    let monthlyRate = Js.Float.toString(ratio *. 100.);

    <DisplayAfterDate
      endDateMoment
      afterComponent={<AnimalInfoStats animal />}
      beforeComponent={
        <React.Fragment>
          <h2> "This animal will launch in:"->React.string </h2>
          <CountDown endDateMoment />
          <br />
          <br />
          <br />
          <small>
            <strong>
              "Monthly Pledge Rate:"->restr
              <Rimble.Tooltip
                message={
                  "This is the monthly percentage contribution of "
                  ++ animalName
                  ++ "'s sale price that will go towards conservation of endangered animals. This is deducted continuously from the deposit and paid by the owner of the animal"
                }
                placement="top">
                <span> {js|ⓘ|js}->restr </span>
              </Rimble.Tooltip>
            </strong>
          </small>
          <br />
          {(monthlyRate ++ " %")->restr}
        </React.Fragment>
      }
    />;
  };
};

module AnimalInfo = {
  [@react.component]
  let make = (~animal: TokenId.t) => {
    let animalDescription =
      QlHooks.useWildcardDescription(animal) |||| [|"Loading"|];
    let optAnimalMedia = animal->Animal.getAlternateImage;
    // TODO: the ethereum address is really terribly displayed. But the default in Rimble UI includes a QR code scanner (which is really ugly).
    // https://rimble.consensys.design/components/rimble-ui/EthAddress#props
    // https://github.com/ConsenSys/rimble-ui/blob/dd470f00374a05c860b558a2cb9317861e4a0d89/src/EthAddress/index.js (maybe make a PR here with some changes)
    <Rimble.Box m=5>
      <ReactTabs>
        <ReactTabs.TabList>
          <ReactTabs.Tab> "Story"->React.string </ReactTabs.Tab>
          <ReactTabs.Tab> "Details"->React.string </ReactTabs.Tab>
          {optAnimalMedia->mapd(React.null, _ =>
             <ReactTabs.Tab> "Media"->React.string </ReactTabs.Tab>
           )}
        </ReactTabs.TabList>
        <ReactTabs.TabPanel>
          <h2> "Story"->React.string </h2>
          <div
            className=Css.(style([maxHeight(`em(26.)), overflow(`scroll)]))>
            {ReasonReact.array(
               animalDescription->Array.mapWithIndex((i, paragraphText) =>
                 <p key={i->string_of_int}> paragraphText->React.string </p>
               ),
             )}
          </div>
          {animal->TokenId.toString == "13"
             // Glen
             ? <a href="/#dao">
                 <span className=Css.(style([color(hex("72c7d7"))]))>
                   "Vote for your favourite conservation"->restr
                 </span>
               </a>
             : React.null}
        </ReactTabs.TabPanel>
        // [@warning "-102"]
        <ReactTabs.TabPanel>
          {switch (animal->Animal.isLaunched) {
           | Launched => <Info tokenId=animal />
           | LaunchDate(endDateMoment) =>
             <UnlaunchedAnimalInfo endDateMoment animal />
           }}
        </ReactTabs.TabPanel>
        <ReactTabs.TabPanel>
          {optAnimalMedia->mapd(React.null, media =>
             <img
               className=Css.(style([width(`percent(100.))]))
               src=media
             />
           )}
        </ReactTabs.TabPanel>
      </ReactTabs>
    </Rimble.Box>;
  };
};

[@react.component]
let make = () => {
  let isGqlLoaded = QlStateManager.useIsInitialized();
  let nonUrlRouting = RootProvider.useNonUrlState();
  let clearNonUrlState = RootProvider.useClearNonUrlState();
  let isDetailView = Router.useIsDetails();
  let optAnimalForDetails = Router.useAnimalForDetails();
  let animalsQuery = QlHooks.useHomeAnimalsQuery();
  Js.log2("animals...?", animalsQuery);

  <Rimble.Flex
    flexWrap={isDetailView ? "wrap-reverse" : "wrap"} alignItems="start">
    <Rimble.Box p=1 width=[|1., 1., 0.5|]>
      <React.Fragment>
        {switch (nonUrlRouting) {
         | LoginScreen(_followOnAction) => <Login />
         | BuyScreen(animal) =>
           <div className=Css.(style([position(`relative)]))>
             <Rimble.Button.Text
               icononly=true
               icon="Close"
               color="moon-gray"
               position="absolute"
               top=0
               right=0
               m=1
               onClick={_ => clearNonUrlState()}
             />
             <Buy tokenId=animal />
           </div>
         | UserVerificationScreen =>
           <div className=Css.(style([position(`relative)]))>
             <Rimble.Button.Text
               icononly=true
               icon="Close"
               color="moon-gray"
               position="absolute"
               top=0
               right=0
               m=1
               onClick={_ => clearNonUrlState()}
             />
             <React.Suspense fallback={<Rimble.Loader />}>
               <LazyThreeBoxUpdate.Lazy />
             </React.Suspense>
           </div>
         | UpdateDepositScreen =>
           <div className=Css.(style([position(`relative)]))>
             <Rimble.Button.Text
               icononly=true
               icon="Close"
               color="moon-gray"
               position="absolute"
               top=0
               right=0
               m=1
               onClick={_ => clearNonUrlState()}
             />
             <UpdateDeposit />
           </div>
         | UpdatePriceScreen(animal) =>
           <div className=Css.(style([position(`relative)]))>
             <Rimble.Button.Text
               icononly=true
               icon="Close"
               color="moon-gray"
               position="absolute"
               top=0
               right=0
               m=1
               onClick={_ => clearNonUrlState()}
             />
             <UpdatePrice tokenId=animal />
           </div>
         | NoExtraState =>
           switch (optAnimalForDetails) {
           | Some(animal) => <AnimalInfo animal />
           | None => <DefaultLeftPanel />
           }
         }}
      </React.Fragment>
    </Rimble.Box>
    <Rimble.Box
      p=1
      className=Css.(
        style([media("(max-width: 831px)", [overflow(`hidden)])])
      )
      width=[|1., 1., 0.5|]>
      <DefaultLook isGqlLoaded />
    </Rimble.Box>
  </Rimble.Flex>;
};
