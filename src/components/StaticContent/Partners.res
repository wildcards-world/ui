let ubisoftLogo = "/img/logos/Ubisoft.png"
let ethCapeTownLogo = "/img/logos/EthCapeTown.png"
let cvLabsLogo = "/img/logos/cvlabszug.jpg"
let kernelLogo = "/img/logos/kernel.gif"

// organisations(where: {onboarding_status: {_in: [live, signed, listed]}, id: {_neq: "wildcards"}, _and: {id: {_neq: "wildcards"}}}) {
module LoadPatronNoDecode = %graphql(`
query ActivePartners {
  organisations(where: {id: {_neq: "wildcards"}, onboarding_status: {_in: [live, signed, listed]}}) {
    name
    logo
    id
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
    organisations
    ->Array.keep(org => !Js.Array2.includes(Config.filterOutOrgs, org.id))
    ->Array.map(org => {logo: org.logo, id: org.id, name: org.name})
    ->Some
  | _ => None
  }

let blueBackground = {
  open CssJs
  style(. [backgroundColor(#hex("73C8D7"))])
}
let cardStyle = {
  open CssJs
  style(. [height(#percent(100.)), display(#flex), important(padding(#percent(0.)))])
}
let logoStyle = {
  open CssJs
  style(. [margin(#percent(10.)), width(#percent(80.))])
}
let corporatePartnerTextStyle = {
  open CssJs
  style(. [textAlign(#center), marginBottom(#percent(10.))])
}
let viewMoreButton = {
  open CssJs
  style(. [margin(auto), display(block)])
}
let viewMoreButtonContainer = {
  open CssJs
  style(. [width(#percent(100.))])
}
let orgContainerStyles = {
  open CssJs
  style(. [width(#percent(100.))])
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
  let allConservationPartners = usePartners()
  let partnersToDisplay =
    allConservationPartners->Option.map(partners =>
      partners->Array.shuffle->Array.slice(~offset=0, ~len=5)
    )
  let numberOfPartners = allConservationPartners->Option.map(partners => partners->Array.length)
  let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute()

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
      <Rimble.Flex
        flexWrap="wrap"
        justifyContent="space-evenly"
        alignItems="stretch"
        pb=10
        px=50
        className=orgContainerStyles>
        {switch partnersToDisplay {
        | Some(conservationPartners) =>
          <>
            {conservationPartners
            ->Array.map(conservation => orgBox(<OrgDetails conservation />, ~key=conservation.id))
            ->React.array}
          </>
        | None => React.null
        }}
      </Rimble.Flex>
      <Rimble.Flex
        flexWrap="wrap"
        justifyContent="center"
        alignItems="stretch"
        className=viewMoreButtonContainer
        pb=30>
        <Rimble.Button
          className=viewMoreButton
          onClick={event => {
            ReactEvent.Form.preventDefault(event)
            clearAndPush("#organisations")
          }}>
          {numberOfPartners
          ->Option.mapWithDefault("View All Partners", numPartners =>
            `View All ${numPartners->string_of_int} Partners`
          )
          ->React.string}
        </Rimble.Button>
      </Rimble.Flex>
    </Rimble.Flex>
    <div className=Styles.infoBackground>
      <Rimble.Flex flexWrap="wrap" justifyContent="space-around" alignItems="stretch" pt=50 px=50>
        <h1
          className={
            open CssJs
            style(. [
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
