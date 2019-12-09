open Providers.DrizzleProvider;

let flameImg = [%bs.raw {|require('../img/streak-flame.png')|}];

module Streak = {
  [@react.component]
  let make = (~animal: Animal.t) => {
    let animalName = Animal.getName(animal);

    let daysHeld = QlHooks.useDaysHeld(animal);

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

module AnimalOnLandingPage = {
  [@react.component]
  let make = (~animal) => {
    let name = Animal.getName(animal);

    let optAlternateImage = Animal.getAlternateImage(animal);
    let optOrgBadge = Animal.getOrgBadgeImage(animal);

    let normalImage = () =>
      <img
        className={Styles.headerImg(1., 1.)}
        src={Animal.getImage(animal)}
      />;

    let componentWithoutImg = (img, ~hideBadges: bool) => {
      <React.Fragment>
        {img()}
        {if (hideBadges) {
           React.null;
         } else {
           <React.Fragment>
             <div className=Styles.overlayFlameImg> <Streak animal /> </div>
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
              <img className={Styles.headerImg(1., 1.)} src=alternateImage />,
            ~hideBadges=true,
          )}
        />
      };
    };

    <Rimble.Box className=Styles.centerText>
      <div className=Styles.positionRelative>
        imageHoverSwitcher
        <div> <h2> {React.string(name)} </h2> </div>
      </div>
    </Rimble.Box>;
    // {switch (optionEndDateMoment) {
    //  | Some(endDateMoment) =>
    //    <div>
    //      <h3 className=Styles.colorGreen> {React.string("COMING IN")} </h3>
    //      <CountDown endDateMoment leadingZeros=true includeWords=false />
    //    </div>
    //  | None =>
    //    <div>
    //      <Offline requireSmartContractsLoaded=true>
    //        <BasicAnimalDisplay animal isExplorer />
    //      </Offline>
    //    </div>
    //  }}
  };
};

[@react.component]
// The Offline container here shows the website, but without loading the requirements
let make = () => {
  let setProvider = useSetProvider();
  React.useEffect0(() => {
    open Web3connect.Core;
    let core = getCore(None); // TOGGLE THE ABOVE LINE OUT BEFORE PRODUCTION!!
    core->setOnConnect(setProvider);
    None;
  });
  let maxWidth = Css.(style([maxWidth(px(500))]));
  <React.Fragment>
    <Rimble.Flex flexWrap="wrap" alignItems="start">
      <Rimble.Box p=1 width=[|1., 1., 0.5|] className=maxWidth>
        <AnimalOnLandingPage animal=Animal.Vitalik />
      </Rimble.Box>
      <Rimble.Box p=1 width=[|1., 1., 0.5|] className=maxWidth>
        <AnimalOnLandingPage animal=Animal.VitalikNew />
      </Rimble.Box>
    </Rimble.Flex>
    <Offline requireSmartContractsLoaded=true>
      <UpdateVitalikModal />
    </Offline>
  </React.Fragment>;
};
