open Globals;

let ubisoftLogo = "/img/logos/Ubisoft.png";
let ethCapeTownLogo = "/img/logos/EthCapeTown.png";
let cvLabsLogo = "/img/logos/cvlabszug.jpg";

let blueBackground = Css.(style([backgroundColor(`hex("73C8D7"))]));
let cardStyle = Css.(style([height(`percent(100.))]));
let logoStyle =
  Css.(style([marginLeft(`percent(10.)), width(`percent(80.))]));
let centerText = Css.(style([textAlign(`center)]));

module OrgDetails = {
  [@react.component]
  let make = (~conservationId) => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
    let orgImage = Animal.useGetOrgImage(conservationId);

    <Rimble.Card className=cardStyle>
      <a
        onClick={e => {
          ReactEvent.Mouse.stopPropagation(e);
          ReactEvent.Mouse.preventDefault(e);
          clearAndPush("#org/" ++ conservationId);
        }}>
        <img
          className=Css.(
            style([marginLeft(`percent(10.)), width(`percent(80.))])
          )
          src=orgImage
          alt=conservationId
        />
      </a>
    </Rimble.Card>;
  };
};

[@react.component]
let make = () => {
  let conservationPartners = [|
    "pangolinafrica",
    "sharkspotters",
    "lemurconservationnetwork",
    "bdi",
    "careforwild",
    "greatwhaleconservancy",
    "sendaverde",
    "wildtomorrow",
    "darwinanimaldoctors",
  |];

  let orgBox = content =>
    <Rimble.Box mt=20 mb=20 width=[|0.45, 0.45, 0.18|] color="black">
      content
    </Rimble.Box>;

  <div width="100%">
    <Rimble.Flex
      flexWrap="wrap"
      justifyContent="space-around"
      alignItems="stretch"
      pt=50
      px=50
      className=blueBackground>
      <h1> "Conservation Partners"->restr </h1>
    </Rimble.Flex>
    <Rimble.Flex
      flexWrap="wrap"
      justifyContent="space-around"
      alignItems="stretch"
      className=blueBackground
      pb=50
      px=50>
      {conservationPartners
       ->Array.map(conservationId => orgBox(<OrgDetails conservationId />))
       ->React.array}
      // These empty org boxes keep positioning in a uniform grid
      {orgBox(React.null)}
      {orgBox(React.null)}
      {orgBox(React.null)}
      {orgBox(React.null)}
    </Rimble.Flex>
    <Rimble.Flex
      flexWrap="wrap"
      justifyContent="space-around"
      alignItems="stretch"
      className=Styles.infoBackground
      px=50>
      <Rimble.Box mt=70 mb=70 width=[|1., 1., 0.2|] color="black">
        <Rimble.Card className=cardStyle>
          <a
            href="https://www.ubisoft.com/en-us/company/start-ups/station-f.aspx">
            <img src=ubisoftLogo alt="ubisoft" className=logoStyle />
            <Rimble.Text className=centerText>
              "Ubisoft's Entrepreneurs Lab, Season 4, participants"->restr
            </Rimble.Text>
          </a>
        </Rimble.Card>
      </Rimble.Box>
      <Rimble.Box mt=70 mb=70 width=[|1., 1., 0.2|] color="black">
        <Rimble.Card className=cardStyle>
          <a href="https://ethcapetown.com/">
            <img src=ethCapeTownLogo alt="eth-cape-town" className=logoStyle />
            <Rimble.Text className=centerText>
              "Overall winners of EthCapeTown hackathon"->restr
            </Rimble.Text>
          </a>
        </Rimble.Card>
      </Rimble.Box>
      <Rimble.Box mt=70 mb=70 width=[|1., 1., 0.2|] color="black">
        <Rimble.Card className=cardStyle>
          <a href="https://cvvc.com/index.php">
            <img src=cvLabsLogo alt="cv-labs" className=logoStyle />
            <Rimble.Text className=centerText>
              "CV Labs Incubator Program, Batch 2"->restr
            </Rimble.Text>
          </a>
        </Rimble.Card>
      </Rimble.Box>
    </Rimble.Flex>
  </div>;
};
