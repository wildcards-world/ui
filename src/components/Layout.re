open StaticContent;

open Components;

// TODO: there must be a better way of importing images in reason react...
let smallIcon = [%bs.raw {|require('../img/logos/wild-cards-small.png')|}];

[@react.component]
let make = () =>
  <div className=Styles.app>
    <header className=Styles.header>
      <nav className=Styles.nav>
        <div className=Styles.navBox>
          <img className=Styles.headerLogo src=smallIcon />
          <ul className=Styles.navList>
            <li className=Styles.navListItem>
              <a
                className=Styles.navListText
                target="_blank"
                href="https://blog.wildcards.world/">
                <S> "BLOG" </S>
              </a>
              <Rimble.Button _as="a" href="/signup" target="\_blank">
                <S> "Subscribe" </S>
              </Rimble.Button>
            </li>
          </ul>
        </div>
      </nav>
    </header>
    <Rimble.Flex flexWrap="wrap" className=Styles.topBody>
      <Rimble.Box p=1 width=[|1., 1., 0.5|]>
        <h1 className=Styles.heading>
          <span className=Styles.colorBlue> <S> "Always for sale" </S> </span>
          <br />
          <S> "ethereum based" </S>
          <br />
          <span className=Styles.colorGreen> <S> "conservation" </S> </span>
          <S> " tokens" </S>
        </h1>
        <hr />
        <h3 className=Styles.subHeading>
          <S>
            "Let your digital assets make a valuable contribution to the world."
          </S>
        </h3>
      </Rimble.Box>
      <Rimble.Box p=1 width=[|1., 1., 0.5|]> <Dapp /> </Rimble.Box>
    </Rimble.Flex>
    <CustomerBenefit />
    <HowItWorks />
    <About />
    <CoreConcepts />
    <EmailSignup />
    <Partners />
    <Footer />
  </div>;
