let ubisoftLogo = "/img/logos/Ubisoft.png"
let ethCapeTownLogo = "/img/logos/EthCapeTown.png"
let cvLabsLogo = "/img/logos/cvlabszug.jpg"
let kernelLogo = "/img/logos/kernel.gif"

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
  let make = (~conservation: Partners.partner) => {
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
  let newConservationPartners = Partners.usePartners()

  let orgBox = (content, ~key) =>
    <Rimble.Box key mt=20 mb=20 width=[0.45, 0.45, 0.18] color="black"> content </Rimble.Box>

  <div width="100%">
    <Rimble.Flex flexWrap="wrap" justifyContent="space-around" alignItems="stretch" pt=50 px=50>
      <h1> {"Conservation Partners"->React.string} </h1>
      <Rimble.Flex
        flexWrap="wrap"
        justifyContent="space-evenly"
        alignItems="stretch"
        pb=10
        px=50
        className=orgContainerStyles>
        {switch newConservationPartners {
        | Some(conservationPartners) =>
          <>
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
    </Rimble.Flex>
  </div>
}
