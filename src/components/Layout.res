module AnimalFocusDetails = {
  @react.component
  let make = (~currentAnimal: option<TokenId.t>, ~showForwardBackButtons) => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute()
    let animalDetails = QlHooks.useDetailsPageNextPrevious(
      Option.getWithDefault(currentAnimal, TokenId.fromStringUnsafe("0")),
    )
    <div
      className={Cn.fromList(list{
        Styles.topBody,
        {
          open Css
          style(list{position(relative)})
        },
      })}>
      {currentAnimal->Option.mapWithDefault(false, _ => true)
        ? <Rimble.Button.Text
            icononly=true
            icon="Close"
            color="black"
            className={
              open Css
              style(list{zIndex(10000)})
            }
            position="absolute"
            top=0
            right=0
            m=1
            size="large"
            onClick={_ => clearAndPush("#" ++ (showForwardBackButtons ? "" : "explorer"))}
          />
        : React.null}
      <Rimble.Flex flexWrap="wrap" alignItems="center">
        {showForwardBackButtons
          ? <div
              className={
                open Css
                style(list{width(#percent(5.))})
              }>
              <span
                className={Styles.carouselArrow(true, ~absolutePosition=false)}
                onClick={InputHelp.handleMouseEvent(() =>
                  clearAndPush("#details/" ++ animalDetails.prev->TokenId.toString)
                )}>
                {`◄`->React.string}
              </span>
            </div>
          : React.null}
        <div
          className={
            open Css
            style(list{
              width(showForwardBackButtons ? #percent(90.) : #percent(100.)),
              // style([width(`percent(animalCarousel->Option.getWithDefault(100., _ => 90.)))])
            })
          }>
          <Dapp currentAnimal />
        </div>
        {showForwardBackButtons
          ? <div
              className={
                open Css
                style(list{width(#percent(5.))})
              }>
              <span
                className={Styles.carouselArrow(false, ~absolutePosition=false) ++
                (" " ++
                {
                  open Css
                  style(list{width(#percent(3.))})
                })}
                onClick={InputHelp.handleMouseEvent(() =>
                  clearAndPush("#details/" ++ animalDetails.next->TokenId.toString)
                )}>
                {`►`->React.string}
              </span>
            </div>
          : React.null}
      </Rimble.Flex>
    </div>
  }
}

@react.component
let make = () => {
  let urlState = Router.useUrlState()

  // TODO: add code that scrolls to the correct height on mobile (to buy etc).
  // let ref = React.useRef(Js.Nullable.null);
  // ref.current.scrollHeight;
  // ReactDOMRe.Ref.domRef(ref);
  /* ref={ReactDOMRe.Ref.domRef(ref)} */

  let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute()
  let isExplorer = Router.useIsExplorer()
  let isDetails = Router.useIsDetails()
  let isHome = Router.useIsHome()

  <div className=Styles.app>
    <div
      className={
        open Css
        style(list{minHeight(vh(88.))})
      }>
      <Announcement announcementBannerColor="72D6B5">
        <span>
          <a href="/#explorer/2nd-edition" className=AnimalAnnouncement.linkToAnimal>
            {"New wildcards"->React.string}
          </a>
          {" launched on the "->React.string}
          <a
            href="https://matic.network"
            rel="noopener noreferrer"
            className=AnimalAnnouncement.linkToAnimal>
            {"MATIC network. "->React.string}
          </a>
        </span>
      </Announcement>
      // <AnimalAnnouncement
      //   nextReleasedAnimals=[|TokenId.makeFromInt(25)|]
      //   announcementBannerColor="f49229" //next color
      //   // FFCD47 - Pendo
      //   // 2493AD - Star
      //   // D6564B - whacky
      //   // 0624a6 - Arthur
      //   // DE4C38
      //   // f49229
      //   // 2493AD
      //   // 72D6B5
      //   // AEE79A
      // />
      <Header
        navItems=[
          {
            shouldDisplay: !isHome,
            shouldDisplayMobile: !isHome,
            component: (closeModal, _) =>
              <a
                className=Styles.navListText
                href=""
                onClick={event => {
                  closeModal()
                  ReactEvent.Mouse.preventDefault(event)
                  clearAndPush("#")
                }}>
                {"HOME"->React.string}
              </a>,
          },
          {
            shouldDisplay: true,
            shouldDisplayMobile: true,
            component: (closeModal, _) =>
              <a
                className=Styles.navListText
                onClick={event => {
                  closeModal()
                  ReactEvent.Mouse.preventDefault(event)
                  clearAndPush(j`/#leaderboards/monthly-contribution`)
                }}>
                {"TOP GUARDIANS"->React.string}
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
                {"BLOG"->React.string}
              </a>,
          },
          {
            shouldDisplay: !isExplorer || isDetails,
            shouldDisplayMobile: !isExplorer || isDetails,
            component: (closeModal, _) =>
              <div>
                <Rimble.Button
                  onClick={event => {
                    closeModal()
                    ReactEvent.Form.preventDefault(event)
                    clearAndPush("#explorer")
                  }}
                  className=Styles.whiteText>
                  {"VIEW WILDCARDS"->React.string}
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
            component: (clickAction, isMobile) => <ProfileIcon clickAction isMobile />,
          },
        ]
      />
      {switch urlState {
      | VotePage => <VotePage chain=Client.MainnetQuery />
      | Team => <Team />
      | IncreaseVoteIteration => React.null // DEPRECATED FUNCTIONALITY. // <IncreaseIterationPage />
      | Explorer(wildcardsEdition, subState) =>
        switch subState {
        | DetailView(currentAnimal) =>
          <AnimalFocusDetails currentAnimal showForwardBackButtons=false />
        | NormalView => <BuyGrid wildcardsEdition />
        }
      | Home(animalPageState) =>
        switch animalPageState {
        | DetailView(currentAnimal) =>
          <AnimalFocusDetails currentAnimal showForwardBackButtons=true />
        | NormalView =>
          <React.Fragment>
            <AnimalFocusDetails currentAnimal=None showForwardBackButtons=false />
            <FeaturedIn />
            <HomepageLeaderBoard />
            <CustomerBenefit />
            <HowItWorks />
            <EmailSignup />
            <FAQs />
            <Partners />
          </React.Fragment>
        }
      | User(userAddress) => <UserProfile chain=Client.MainnetQuery userAddress />
      | Artist(artistIdentifier) => <ArtistProfiles artistIdentifier />
      | Org(orgId) => <OrgProfile orgId />
      | Leaderboards(leaderboardType) =>
        <Rimble.Flex
          flexWrap="wrap"
          alignItems="center"
          className={
            open Css
            style(list{padding(em(2.))})
          }>
          <LeaderBoards leaderboardType />
        </Rimble.Flex>
      }}
    </div>
    <Footer />
  </div>
}
