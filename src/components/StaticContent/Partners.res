let ubisoftLogo = "/img/logos/Ubisoft.png"
let ethCapeTownLogo = "/img/logos/EthCapeTown.png"
let cvLabsLogo = "/img/logos/cvlabszug.jpg"
let kernelLogo = "/img/logos/kernel.gif"

module LoadPatronNoDecode = %graphql(`
  query ActivePartners {
    organisations(where: {onboarding_status: {_in: [live,signed,listed]}}) {
      logo
      id
      name
    }
  }
`)
type partner = {
  logo: string,
  id: string,
  name: string,
}
let usePartners = () =>
  switch LoadPatronNoDecode.use() {
  | {loading: true, _} => None
  | {error: Some(_error), _} => None
  | {data: Some({organisations}), _} =>
    organisations->Array.map(org => {logo: org.logo, id: org.id, name: org.name})->Some
  | _ => None
  }

let blueBackground = {
  open CssJs
  style(.[backgroundColor(#hex("73C8D7"))])
}
let cardStyle = {
  open CssJs
  style(.[height(#percent(100.)), display(#flex), important(padding(#percent(0.)))])
}
let logoStyle = {
  open CssJs
  style(.[margin(#percent(10.)), width(#percent(80.))])
}
let corporatePartnerTextStyle = {
  open CssJs
  style(.[textAlign(#center), marginBottom(#percent(10.))])
}

module OrgDetails = {
  @react.component
  let make = (~conservation) => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute()
    // let orgImage = Animal.useGetOrgImage(conservationId);
    let {logo, id, name} = conservation

    <Rimble.Card className=cardStyle>
      <a
        className={
          open Css
          style(list{display(#flex), width(#percent(100.)), height(#percent(100.))})
        }
        onClick={e => {
          ReactEvent.Mouse.stopPropagation(e)
          ReactEvent.Mouse.preventDefault(e)
          clearAndPush("#org/" ++ id)
        }}>
        <img
          className={
            open Css
            style(list{
              margin(#percent(1.)),
              objectFit(#contain),
              width(#percent(98.)),
              justifyContent(#center),
              alignItems(#center),
              hover(list{
                filter(list{#saturate(150.), #brightness(110.)}),
                overflow(visible),
                boxShadow(Shadow.box(~blur=px(20), ~spread=px(20), rgba(121, 181, 80, #num(0.5)))),
                transform(scale(1.01, 1.01)),
                transition(~duration=100, ~delay=0, ~timingFunction=ease, "all"),
              }),
            })
          }
          src={CONSTANTS.cdnBase ++ logo}
          alt=name
        />
      </a>
    </Rimble.Card>
  }
}

@react.component
let make = () => {
  let newConservationPartners = usePartners()

  let orgBox = (content, ~key) =>
    <Rimble.Box key mt=20 mb=20 width=[0.45, 0.45, 0.18] color="black"> content </Rimble.Box>

  <div width="100%">
    <Rimble.Flex
      flexWrap="wrap"
      justifyContent="space-around"
      alignItems="stretch"
      pt=50
      px=50
      className=blueBackground>
      <h1> {"Conservation Partners"->React.string} </h1>
    </Rimble.Flex>
    <Rimble.Flex
      flexWrap="wrap"
      justifyContent="space-around"
      alignItems="stretch"
      className=blueBackground
      pb=50
      px=50>
      {switch newConservationPartners {
      | Some(conservationPartners) => <>
          {conservationPartners
          ->Array.map(conservation => orgBox(<OrgDetails conservation />, ~key=conservation.id))
          ->React.array}
          {// These empty org boxes keep positioning in a uniform grid
          orgBox(React.null, ~key="a")}
          {orgBox(React.null, ~key="b")}
          {orgBox(React.null, ~key="c")}
          {orgBox(React.null, ~key="d")}
        </>
      | None => React.null
      }}
    </Rimble.Flex>
    <div className=Styles.infoBackground>
      <Rimble.Flex flexWrap="wrap" justifyContent="space-around" alignItems="stretch" pt=50 px=50>
        <h1
          className={
            open CssJs
            style(.[
              unsafe(
                "text-shadow",
                "0.25em 0.25em 0.5em #FFFFFF, -0.25em -0.25em 0.5em #FFFFFF, -0.25em 0.25em 0.5em #FFFFFF, 0.25em -0.25em 0.5em #FFFFFF;",
              ),
            ])
          }>
          {"Corporate partners"->React.string}
        </h1>
      </Rimble.Flex>
      <Rimble.Flex flexWrap="wrap" justifyContent="space-around" alignItems="stretch" px=50>
        <Rimble.Box mt=70 mb=70 width=[1., 1., 0.2] color="black">
          <Rimble.Card className=cardStyle>
            <a href="https://www.ubisoft.com/en-us/company/start-ups/station-f.aspx">
              <img src=ubisoftLogo alt="ubisoft" className=logoStyle />
              <Rimble.Text className=corporatePartnerTextStyle>
                {"Ubisoft's Entrepreneurs Lab, Season 4, participants"->React.string}
              </Rimble.Text>
            </a>
          </Rimble.Card>
        </Rimble.Box>
        <Rimble.Box mt=70 mb=70 width=[1., 1., 0.2] color="black">
          <Rimble.Card className=cardStyle>
            <a href="https://ethcapetown.com/">
              <img src=ethCapeTownLogo alt="eth-cape-town" className=logoStyle />
              <Rimble.Text className=corporatePartnerTextStyle>
                {"Overall winners of EthCapeTown hackathon"->React.string}
              </Rimble.Text>
            </a>
          </Rimble.Card>
        </Rimble.Box>
        <Rimble.Box mt=70 mb=70 width=[1., 1., 0.2] color="black">
          <Rimble.Card className=cardStyle>
            <a href="https://cvvc.com/index.php">
              <img src=cvLabsLogo alt="cv-labs" className=logoStyle />
              <Rimble.Text className=corporatePartnerTextStyle>
                {"CV Labs Incubator Program, Batch 2"->React.string}
              </Rimble.Text>
            </a>
          </Rimble.Card>
        </Rimble.Box>
        <Rimble.Box mt=70 mb=70 width=[1., 1., 0.2] color="black">
          <Rimble.Card className=cardStyle>
            <a href="https://kernel.community/">
              <img src=kernelLogo alt="Kernel Gitcoin" className=logoStyle />
              <Rimble.Text className=corporatePartnerTextStyle>
                {"Gitcoin Kernel genesis block participants"->React.string}
              </Rimble.Text>
            </a>
          </Rimble.Card>
        </Rimble.Box>
      </Rimble.Flex>
    </div>
  </div>
}
