open Components;

// TODO: there must be a better way of importing images in reason react...
let betaBanner = [%bs.raw {|require('../img/beta-banner.png')|}];

module BuyGrid = {
  [@bs.module "./BuyGrid.js"] [@react.component]
  external make: (~animalArray: array(unit => React.element)) => React.element =
    "default";
};

module AnimalFocusDetails = {
  [@react.component]
  let make = (~animalCarousel) => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
    let isExplorer = Router.useIsExplorer();

    <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
      {switch (animalCarousel) {
       | None => React.null
       | Some((_, previousAnimal)) =>
         <Rimble.Box p=1 width=[|0.05, 0.05, 0.05|]>
           <span
             className={Styles.carouselArrow(true, ~absolutePosition=false)}
             onClick={InputHelp.handleMouseEvent(() =>
               clearAndPush(
                 "#"
                 ++ InputHelp.getPagePrefix(isExplorer)
                 ++ "details/"
                 ++ previousAnimal->Animal.getName->Js.Global.encodeURI,
               )
             )}>
             {js|◄|js}->React.string
           </span>
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
           <span
             className={Styles.carouselArrow(false, ~absolutePosition=false)}
             onClick={InputHelp.handleMouseEvent(() =>
               clearAndPush(
                 "#"
                 ++ InputHelp.getPagePrefix(isExplorer)
                 ++ "details/"
                 ++ nextAnimal->Animal.getName->Js.Global.encodeURI,
               )
             )}>
             {js|►|js}->React.string
           </span>
         </Rimble.Box>
       }}
    </Rimble.Flex>;
  };
};

module Header = {
  [@react.component]
  let make = (~animalCarousel) => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
    let isExplorer = Router.useIsExplorer();
    let isDetails = Router.useIsDetails();
    open ReactTranslate;
    let usedtranslationModeContext = useTranslationModeContext();
    <header className=Styles.header>
      <nav className=Styles.nav>
        <div className=Styles.navBox>
          <a
            className=Styles.clickableLink
            onClick={event => {
              ReactEvent.Mouse.preventDefault(event);
              clearAndPush("#");
            }}>
            <div className=Styles.headerLogo>
              <WildcardsLogo maxWidth="258px" />
            </div>
          </a>
          <ul className=Styles.navList>
            <li className=Styles.navListItem>
              {isExplorer || isDetails
                 ? React.null
                 : <div className=Styles.navListItemToggle>
                     <span className=Styles.someMarginRight>
                       <S>
                         {usedtranslationModeContext->translationModeCrypto
                            ? "EXPERT MODE" : "DEFAULT MODE"}
                       </S>
                     </span>
                     <ReactTranslate.Switch
                       onChange={
                         usedtranslationModeContext->settranslationModeCrypto
                       }
                       checked={
                         usedtranslationModeContext->translationModeCrypto
                       }
                       onColor="#6BAD3D"
                       onHandleColor="#346D4C"
                       uncheckedIcon=false
                       checkedIcon=false
                       className=Styles.translationSwitch
                     />
                   </div>}
            </li>
            <li className=Styles.navListItem>
              {switch (animalCarousel) {
               | None => React.null
               | Some(_) =>
                 <a
                   className=Styles.navListText
                   onClick={event => {
                     ReactEvent.Mouse.preventDefault(event);
                     clearAndPush("#");
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
              {isExplorer && !isDetails
                 ? <a
                     className=Styles.navListText
                     target="_blank"
                     href=""
                     onClick={event => {
                       ReactEvent.Mouse.preventDefault(event);
                       clearAndPush("#");
                     }}>
                     <S> "HOME" </S>
                   </a>
                 : <Rimble.Button
                     onClick={event => {
                       ReactEvent.Form.preventDefault(event);
                       clearAndPush("#explorer");
                     }}
                     className=Styles.whiteText>
                     <S> "VIEW ALL WILDCARDS" </S>
                   </Rimble.Button>}
            </li>
          </ul>
        </div>
      </nav>
    </header>;
  };
};

[@react.component]
let make = () => {
  let urlState = Router.useUrlState();

  // TODO: add code that scrolls to the correct height on mobile (to buy etc).
  // let ref = React.useRef(Js.Nullable.null);
  // ref.current.scrollHeight;
  // ReactDOMRe.Ref.domRef(ref);
  /*ref={ReactDOMRe.Ref.domRef(ref)}*/

  <div className=Styles.app>
    <img src=betaBanner className=Styles.betaBanner />
    <Web3Connect />
    <Header animalCarousel={Some((Animal.Andy, Animal.Simon))} />
    <Providers.UsdPriceProvider>
      {switch (urlState) {
       | Explorer(animalPageState) =>
         switch (animalPageState) {
         | DetailView(animalCarousel, _) =>
           <AnimalFocusDetails animalCarousel />
         | NormalView =>
           <BuyGrid
             animalArray={
               Animal.orderedArray->Belt.Array.map((animal, ()) =>
                 <Dapp.CarouselAnimal animal isGqlLoaded=true scalar=1. />
               )
             }
           />
         }
       | Home(animalPageState) =>
         switch (animalPageState) {
         | DetailView(animalCarousel, _) =>
           <AnimalFocusDetails animalCarousel />
         | NormalView =>
           <React.Fragment>
             <AnimalFocusDetails animalCarousel=None />
             <StaticContent.CustomerBenefit />
             <StaticContent.HowItWorks />
             <StaticContent.About />
             <StaticContent.CoreConcepts />
             <StaticContent.EmailSignup />
             <StaticContent.FAQs />
             <StaticContent.Partners />
           </React.Fragment>
         }
       | User(userAddress) => <UserProfile userAddress />
       }}
    </Providers.UsdPriceProvider>
    <StaticContent.Footer />
  </div>;
};
