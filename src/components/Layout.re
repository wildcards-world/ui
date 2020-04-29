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
  let make = () => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
    let isExplorer = Router.useIsExplorer();
    let isDetails = Router.useIsDetails();
    let isHome = Router.useIsHome();
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
              {isHome
                 ? <div className=Styles.navListItemToggle>
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
                       height=16
                       handleDiameter=18
                       width=30
                       onColor="#6BAD3D"
                       onHandleColor="#346D4C"
                       offHandleColor="#cccccc"
                       uncheckedIcon=false
                       checkedIcon=false
                       className=Styles.translationSwitch
                     />
                   </div>
                 : React.null}
            </li>
            <li className=Styles.navListItem>
              {isHome
                 ? React.null
                 : <a
                     className=Styles.navListText
                     href=""
                     onClick={event => {
                       ReactEvent.Mouse.preventDefault(event);
                       clearAndPush("#");
                     }}>
                     <S> "HOME" </S>
                   </a>}
              <a
                className=Styles.navListText
                onClick={event => {
                  ReactEvent.Mouse.preventDefault(event);
                  clearAndPush({j|/#leaderboards/monthly-contribution|j});
                }}>
                <S> "LEADERBOARDS" </S>
              </a>
              <a
                className=Styles.navListText
                target="_blank"
                rel="noopener noreferrer"
                href="https://blog.wildcards.world/">
                <S> "BLOG" </S>
              </a>
              {isExplorer && !isDetails
                 ? React.null
                 : <Rimble.Button
                     onClick={event => {
                       ReactEvent.Form.preventDefault(event);
                       clearAndPush("#explorer");
                     }}
                     className=Styles.whiteText>
                     <S> "VIEW WILDCARDS" </S>
                   </Rimble.Button>}
            </li>
            <li className=Styles.navListItem> <Web3Connect /> </li>
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
    <div className=Css.(style([minHeight(vh(88.))]))>
      <Announcement />
      <div className=Css.(style([position(relative)]))>
        <img src=betaBanner className=Styles.betaBanner />
      </div>
      <Header />
      <UsdPriceProvider>
        {switch (urlState) {
         | VotePage => <VotePage />
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
               <CustomerBenefit />
               <HomepageLeaderBoard />
               <About />
               <HowItWorks />
               <EmailSignup />
               <FAQs />
               <StaticContent.Partners />
             </React.Fragment>
           }
         | User(userAddress) => <UserProfile userAddress />
         | Leaderboards(leaderboardType) =>
           <Rimble.Flex
             flexWrap="wrap" alignItems="center" className=Styles.topBody>
             <LeaderBoards leaderboardType />
           </Rimble.Flex>
         }}
      </UsdPriceProvider>
    </div>
    <Footer />
  </div>;
};