// TODO: there must be a better way of importing images in reason react...
let smallIcon = [%bs.raw {|require('./img/logos/wild-cards-small.png')|}];

[@react.component]
let make = () =>
  <div className=Styles.app>
    <header className=Styles.header>
      <nav className=Styles.nav>
        <div className=Styles.navBox>
          <img src=smallIcon />
          <ul className=Styles.navList>
            <li className=Styles.navListItem>
              <a
                className=Styles.navListText
                href="https://ventureburn.com/2019/05/ethcapetown-blockchain-winning-applications/">
                {ReasonReact.string("BACKGROUND")}
              </a>
              // className=Styles.highlightGreen
              <Rimble.Button _as="a" href="/signup" target="\_blank">
                {ReasonReact.string("Subscribe")}
              </Rimble.Button>
            </li>
          </ul>
          <li />
        </div>
      </nav>
    </header>
    <div className=Styles.topBody>
      // TODO: we should be using a library for some of this, eg : https://rimble.consensys.design/components/rimble-ui/Flex

        <div className=Styles.leftTopHeader>
          <h1 className=Styles.heading>
            {ReasonReact.string("Ethereum based")}
            <br />
            <span className=Styles.colorGreen>
              {ReasonReact.string("conservation")}
            </span>
            {ReasonReact.string(" tokens")}
          </h1>
          <hr />
          <h3 className=Styles.subHeading>
            {ReasonReact.string(
               "Let your digital assets make a valuable contribution to the world.",
             )}
          </h3>
        </div>
        <Dapp />
      </div>
  </div>;
// <Offline requireSmartContractsLoaded=true> <JsDemo /> </Offline>
