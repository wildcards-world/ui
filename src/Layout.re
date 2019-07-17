// TODO: there must be a better way of importing images in reason react...
let smallIcon = [%bs.raw {|require('./img/logos/wild-cards-small.png')|}];

module JsDemo = {
  [@bs.module "./Demo"] [@react.component]
  external make: unit => React.element = "default";
};

[@react.component]
let make = () =>
  <div className=Styles.app>
    <header className=Styles.header>
      <nav className=Styles.nav>
        <div className=Styles.navBox>
          <img src=smallIcon />
          <ul className=Styles.navList>
            <li className=Styles.navListItem>
              <span className=Styles.navListText>
                <a
                  href="https://ventureburn.com/2019/05/ethcapetown-blockchain-winning-applications/">
                  {ReasonReact.string("BACKGROUND")}
                </a>
              </span>
              <Rimble.Button _as="a" href="/signup" target="\_blank">
                {ReasonReact.string("Test")}
              </Rimble.Button>
            </li>
          </ul>
          <li />
        </div>
      </nav>
      <div className="container container--large header-23__container">
        <div className="header-23__left">
          <div className="header-23__left_content" />
        </div>
      </div>
    </header>
    <div className=Styles.topBody>
      <div className="header-23__left">
        <div className="header-23__left_content">
          <h1 className="heading heading--accent header-23__heading">
            {ReasonReact.string("Ethereum based")}
            <br />
            <span> {ReasonReact.string("conservation")} </span>
            {ReasonReact.string("tokens")}
          </h1>
          <hr />
          <h3 className="header-23__text">
            {ReasonReact.string(
               "Let your digital assets make a valuable contribution to the world.",
             )}
          </h3>
        </div>
      </div>
      <Providers.UsdPriceProvider>
        <Offline requireSmartContractsLoaded=true> <Dapp /> </Offline>
        <Offline requireSmartContractsLoaded=true> <JsDemo /> </Offline>
      </Providers.UsdPriceProvider>
    </div>
  </div>;
