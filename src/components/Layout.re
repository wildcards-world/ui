open Components;

// TODO: there must be a better way of importing images in reason react...
let smallIcon = [%bs.raw {|require('../img/logos/wild-cards-small.png')|}];
let betaBanner = [%bs.raw {|require('../img/beta-banner.png')|}];

// type DetailView = | Details(contentWidth, nextAnimal, previousAnimal)
// | NoDetails(contentWidth)

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  let animalCarousel =
    switch (Js.String.split("/", url.hash)) {
    | [|"details", gorillaStr|] =>
      let optionAnimal = Animal.getAnimal(gorillaStr);

      optionAnimal->Belt.Option.map(animal => Animal.getNextPrev(animal));
    | _ => None
    };
  open ReactTranslate;
  let usedtranslationModeContext = useTranslationModeContext();

  <div className=Styles.app>
    <img src=betaBanner className=Styles.betaBanner />
    <header className=Styles.header>
      <nav className=Styles.nav>
        <div className=Styles.navBox>
          <a
            className=Styles.clickableLink
            onClick={event => {
              ReactEvent.Mouse.preventDefault(event);
              ReasonReactRouter.push("#");
            }}>
            <div className=Styles.headerLogo> <img src=smallIcon /> </div>
          </a>
          <ul className=Styles.navList>
            <li className=Styles.navListItem>
              <div className=Styles.navListItemToggle>
                <span className=Styles.someMarginRight>
                  <S>
                    {usedtranslationModeContext->translationModeCrypto
                       ? "EXPERT MODE" : "SIMPLE MODE"}
                  </S>
                </span>
                <ReactTranslate.Switch
                  onChange={
                    usedtranslationModeContext->settranslationModeCrypto
                  }
                  checked={usedtranslationModeContext->translationModeCrypto}
                  onColor="#6BAD3D"
                  onHandleColor="#346D4C"
                  uncheckedIcon=false
                  checkedIcon=false
                  className=Styles.translationSwitch
                />
              </div>
            </li>
            <li className=Styles.navListItem>
              {switch (animalCarousel) {
               | None => React.null
               | Some(_) =>
                 <a
                   className=Styles.navListText
                   onClick={event => {
                     ReactEvent.Mouse.preventDefault(event);
                     ReasonReactRouter.push("#");
                   }}>
                   <S> "HOME" </S>
                 </a>
               }}
              <a
                className=Styles.navListText
                target="_blank"
                href="https://blog.wildcards.world/">
                <S> "BLOG" </S>
              </a>
              <Rimble.Button
                className=Styles.whiteText
                _as="a"
                href="#signup"
                target="\_blank">
                <S> "Subscribe" </S>
              </Rimble.Button>
            </li>
          </ul>
        </div>
      </nav>
    </header>
    <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
      {switch (animalCarousel) {
       | None => React.null
       | Some((_, previousAnimal)) =>
         <Rimble.Box p=1 width=[|0.05, 0.05, 0.05|]>
           <Rimble.Button
             className=Styles.forwardBackButton
             onClick={InputHelp.handleEvent(() =>
               ReasonReactRouter.push(
                 "#details/" ++ Animal.getName(previousAnimal),
               )
             )}>
             <S> {js|◄|js} </S>
           </Rimble.Button>
         </Rimble.Box>
       }}
      <Rimble.Box
        width={
          animalCarousel->Belt.Option.mapWithDefault([|1.|], _ => [|0.9|])
        }>
        <Dapp />
      </Rimble.Box>
      {switch (animalCarousel) {
       | None => React.null
       | Some((nextAnimal, _)) =>
         <Rimble.Box p=1 width=[|0.05, 0.05, 0.05|]>
           <Rimble.Button
             className=Styles.forwardBackButton
             onClick={InputHelp.handleEvent(() =>
               ReasonReactRouter.push(
                 "#details/" ++ Animal.getName(nextAnimal),
               )
             )}>
             <S> {js|►|js} </S>
           </Rimble.Button>
         </Rimble.Box>
       }}
    </Rimble.Flex>
    <StaticContent.CustomerBenefit />
    <StaticContent.HowItWorks />
    <StaticContent.About />
    <StaticContent.CoreConcepts />
    <StaticContent.EmailSignup />
    <StaticContent.Partners />
    <StaticContent.Footer />
  </div>;
};
