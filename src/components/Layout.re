open Components;

// TODO: there must be a better way of importing images in reason react...
let smallIcon = [%bs.raw {|require('../img/logos/wild-cards-small.png')|}];
let betaBanner = [%bs.raw {|require('../img/beta-banner.png')|}];

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  let (detailView, contentWidths, nextAnimal, previousAnimal) =
    switch (Js.String.split("/", url.hash)) {
    | [|"details", gorillaStr|] =>
      let gorilla = Animal.getAnimal(gorillaStr);
      // let tokenId = Animal.getId(gorilla);
      let (next, previous) = Animal.getNextPrevStr(gorilla);
      switch (gorilla) {
      | NoAnimal => (false, [|1., 1., 1.|], next, previous)
      | _ => (true, [|0.9, 0.9, 0.9|], next, previous)
      };
    | _ => (false, [|1., 1., 1.|], "vitalik", "simon")
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
              {detailView
                 ? <a
                     className=Styles.navListText
                     onClick={event => {
                       ReactEvent.Mouse.preventDefault(event);
                       ReasonReactRouter.push("#");
                     }}>
                     <S> "HOME" </S>
                   </a>
                 : React.null}
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
      {<React.Fragment>
         {detailView
            ? <Rimble.Box p=1 width=[|0.05, 0.05, 0.05|]>
                <Rimble.Button
                  className=Styles.forwardBackButton
                  onClick={InputHelp.handleEvent(() =>
                    ReasonReactRouter.push("#details/" ++ previousAnimal)
                  )}>
                  <S> {js|◄|js} </S>
                </Rimble.Button>
              </Rimble.Box>
            : React.null}
         <Rimble.Box width=contentWidths> <Dapp /> </Rimble.Box>
         {detailView
            ? <Rimble.Box p=1 width=[|0.05, 0.05, 0.05|]>
                <Rimble.Button
                  className=Styles.forwardBackButton
                  onClick={InputHelp.handleEvent(() =>
                    ReasonReactRouter.push("#details/" ++ nextAnimal)
                  )}>
                  <S> {js|►|js} </S>
                </Rimble.Button>
              </Rimble.Box>
            : React.null}
       </React.Fragment>}
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
