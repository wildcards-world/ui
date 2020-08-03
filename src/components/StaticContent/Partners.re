open Globals;
open ReasonApolloHooks;

let ubisoftLogo = "/img/logos/Ubisoft.png";
let ethCapeTownLogo = "/img/logos/EthCapeTown.png";
let cvLabsLogo = "/img/logos/cvlabszug.jpg";

module LoadPatronNoDecode = [%graphql
  {|
query ActivePartners {
  organisations(where: {onboarding_status: {_in: [live,signed,listed]}}) {
    logo
    id
    name
  }
}
  |}
];
type partner = {
  logo: string,
  id: string,
  name: string,
};
let usePartners = () => {
  let (simple, _) = ApolloHooks.useQuery(LoadPatronNoDecode.definition);
  switch (simple) {
  | Data(orgs) =>
    orgs##organisations
    ->Array.map(org => {logo: org##logo, id: org##id, name: org##name})
    ->Some
  | Error(_)
  | Loading
  | NoData => None
  };
};

let blueBackground = Css.(style([backgroundColor(`hex("73C8D7"))]));
let cardStyle = Css.(style([height(`percent(100.))]));
let logoStyle =
  Css.(style([marginLeft(`percent(10.)), width(`percent(80.))]));
let centerText = Css.(style([textAlign(`center)]));

module OrgDetails = {
  [@react.component]
  let make = (~conservation) => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
    // let orgImage = Animal.useGetOrgImage(conservationId);
    let {logo, id, name} = conservation;

    <Rimble.Card className=cardStyle>
      <a
        onClick={e => {
          ReactEvent.Mouse.stopPropagation(e);
          ReactEvent.Mouse.preventDefault(e);
          clearAndPush("#org/" ++ id);
        }}>
        <img
          className=Css.(
            style([marginLeft(`percent(10.)), width(`percent(80.))])
          )
          src={Animal.cdnBase ++ logo}
          alt=name
        />
      </a>
    </Rimble.Card>;
  };
};

[@react.component]
let make = () => {
  let newConservationPartners = usePartners();

  let orgBox = (content, ~key) =>
    <Rimble.Box key mt=20 mb=20 width=[|0.45, 0.45, 0.18|] color="black">
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
      {switch (newConservationPartners) {
       | Some(conservationPartners) =>
         <>
           {conservationPartners
            ->Array.map(conservation =>
                orgBox(<OrgDetails conservation />, ~key=conservation.id)
              )
            ->React.array}
           // These empty org boxes keep positioning in a uniform grid
           {orgBox(React.null, ~key="a")}
           {orgBox(React.null, ~key="b")}
           {orgBox(React.null, ~key="c")}
           {orgBox(React.null, ~key="d")}
         </>
       //  "helloWorld"->React.string;
       | None => React.null
       }}
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
