open Globals;

// TODO: there must be a better way of importing images in reason react...
let betaBanner = "/img/beta-banner.png";

module AnimalFocusDetails = {
  [@react.component]
  let make = (~animalCarousel) => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
    let isExplorer = Router.useIsExplorer();
    <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
      <div className=Css.(style([width(`percent(5.))]))>
        {switch (animalCarousel) {
         | None => React.null
         | Some((_, previousAnimal)) =>
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
         }}
      </div>
      <div
        className=Css.(
          style([
            width(`percent(90.)),
            // style([width(`percent(animalCarousel->mapd(100., _ => 90.)))])
          ])
        )>
        <Dapp />
      </div>
      <div className=Css.(style([width(`percent(5.))]))>
        {switch (animalCarousel) {
         | None => React.null
         | Some((nextAnimal, _)) =>
           <span
             className={
               Styles.carouselArrow(false, ~absolutePosition=false)
               ++ " "
               ++ Css.(style([width(`percent(3.))]))
             }
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
         }}
      </div>
    </Rimble.Flex>;
  };
};

module HeaderOld = {
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
                       {usedtranslationModeContext->translationModeCrypto
                          ? "EXPERT MODE" : "DEFAULT MODE"}
                       ->restr
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
                     "HOME"->restr
                   </a>}
              <a
                className=Styles.navListText
                onClick={event => {
                  ReactEvent.Mouse.preventDefault(event);
                  clearAndPush({j|/#leaderboards/monthly-contribution|j});
                }}>
                "LEADERBOARDS"->restr
              </a>
              <a
                className=Styles.navListText
                target="_blank"
                rel="noopener noreferrer"
                href="https://blog.wildcards.world/">
                "BLOG"->restr
              </a>
              {isExplorer && !isDetails
                 ? React.null
                 : <Rimble.Button
                     onClick={event => {
                       ReactEvent.Form.preventDefault(event);
                       clearAndPush("#explorer");
                     }}
                     className=Styles.whiteText>
                     "VIEW WILDCARDS"->restr
                   </Rimble.Button>}
            </li>
            <li className=Styles.navListItem> <Web3Connect /> </li>
            <li className=Styles.navListItem> <ProfileIcon /> </li>
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

  let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
  let isExplorer = Router.useIsExplorer();
  let isDetails = Router.useIsDetails();
  let isHome = Router.useIsHome();
  open ReactTranslate;
  let usedtranslationModeContext = useTranslationModeContext();

  <div className=Styles.app>
    <div className=Css.(style([minHeight(vh(88.))]))>
      <Announcement
        nextReleasedAnimals=[|Animal.Mijungla|]
        announcementBannerColor="f49229"
        //2493AD
        //2493AD
        //72D6B5
        //AEE79A
      />
      <div className=Css.(style([position(relative)]))>
        <img src=betaBanner className=Styles.betaBanner />
      </div>
      <Header
        navItems=[|
          {
            shouldDisplay: isHome,
            shouldDisplayMobile: false,
            component: (_, _) =>
              <div className=Styles.navListItemToggle>
                <span className=Styles.someMarginRight>
                  (
                    usedtranslationModeContext->translationModeCrypto
                      ? "EXPERT MODE" : "DEFAULT MODE"
                  )
                  ->restr
                </span>
                <ReactTranslate.Switch
                  onChange={
                    usedtranslationModeContext->settranslationModeCrypto
                  }
                  checked={usedtranslationModeContext->translationModeCrypto}
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
              </div>,
          },
          {
            shouldDisplay: !isHome,
            shouldDisplayMobile: !isHome,
            component: (closeModal, _) =>
              <a
                className=Styles.navListText
                href=""
                onClick={event => {
                  closeModal();
                  ReactEvent.Mouse.preventDefault(event);
                  clearAndPush("#");
                }}>
                "HOME"->restr
              </a>,
          },
          {
            shouldDisplay: true,
            shouldDisplayMobile: true,
            component: (closeModal, _) =>
              <a
                className=Styles.navListText
                onClick={event => {
                  closeModal();
                  ReactEvent.Mouse.preventDefault(event);
                  clearAndPush({j|/#leaderboards/monthly-contribution|j});
                }}>
                "LEADERBOARDS"->restr
              </a>,
          },
          {
            shouldDisplay: true,
            shouldDisplayMobile: true,
            component: (closeModal, _) =>
              <a
                className=Styles.navListText
                target="_blank"
                onClick={_ => closeModal()}
                rel="noopener noreferrer"
                href="https://blog.wildcards.world/">
                "BLOG"->restr
              </a>,
          },
          {
            shouldDisplay: !isExplorer || isDetails,
            shouldDisplayMobile: !isExplorer || isDetails,
            component: (closeModal, _) =>
              <div>
                <Rimble.Button
                  onClick={event => {
                    closeModal();
                    ReactEvent.Form.preventDefault(event);
                    clearAndPush("#explorer");
                  }}
                  className=Styles.whiteText>
                  "VIEW WILDCARDS"->restr
                </Rimble.Button>
              </div>,
          },
          {
            shouldDisplay: true,
            shouldDisplayMobile: true,
            component: (clickAction, _) => <Web3Connect clickAction />,
          },
          {
            shouldDisplay: true,
            shouldDisplayMobile: true,
            component: (clickAction, isMobile) =>
              <ProfileIcon clickAction isMobile />,
          },
        |]
      />
      <UsdPriceProvider>
        {switch (urlState) {
         | VotePage => <VotePage />
         | IncreaseVoteIteration => <IncreaseIterationPage />
         | Explorer(animalPageState) =>
           switch (animalPageState) {
           | DetailView(animalCarousel, _) =>
             <AnimalFocusDetails animalCarousel />
           | NormalView => <BuyGrid />
           }
         | Home(animalPageState) =>
           switch (animalPageState) {
           | DetailView(animalCarousel, _) =>
             <AnimalFocusDetails animalCarousel />
           | NormalView =>
             <React.Fragment>
               <AnimalFocusDetails animalCarousel=None />
               <FeaturedIn />
               <HomepageLeaderBoard />
               <CustomerBenefit />
               <HowItWorks />
               <EmailSignup />
               <FAQs />
               <StaticContent.Partners />
             </React.Fragment>
           }
         | User(userAddress) => <UserProfile userAddress />
         | Leaderboards(leaderboardType) =>
           <Rimble.Flex
             flexWrap="wrap"
             alignItems="center"
             className=Css.(style([padding(em(2.))]))>
             <LeaderBoards leaderboardType />
           </Rimble.Flex>
         }}
      </UsdPriceProvider>
    </div>
    <Footer />
  </div>;
};
