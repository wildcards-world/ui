open Globals;

[@decco.decode]
type orgDescriptionArray = array(string);

module YoutubeVid = {
  [@bs.module "./StaticContent//YoutubeVideo.js"] [@react.component]
  external make: (~videoCode: string) => React.element = "default";
};

module OrgPage = {
  [@react.component]
  let make = (~orgData, ~orgId) => {
    let orgName = orgData##name;
    let orgDescription = orgData##description->orgDescriptionArray_decode;
    let orgAnimals = orgData##wildcard;
    let orgAnimalsArray = orgAnimals->Array.map(animal => animal##id);
    let currentUsdEthPrice = UsdPriceProvider.useUsdPrice();
    let totalCollected = QlHooks.useTotalRaisedAnimalGroup(orgAnimalsArray);

    let (totalPatronage, totalPatronageUsd) =
      totalCollected->mapd(("Loading", "Loading"), a =>
        (
          (a->Eth.get(Eth.Eth(`ether))->Float.fromString |||| 0.0)
          ->toFixedWithPrecisionNoTrailingZeros(~digits=9),
          currentUsdEthPrice->mapd("Loading", usdEthRate =>
            a->Eth.get(Eth.Usd(usdEthRate, 2))
          ),
        )
      );
    let orgWebsite = orgData##website;
    let orgImage = Animal.useGetOrgImage(orgId);

    <div>
      <div className=Css.(style([width(`percent(100.))]))>
        <Rimble.Flex flexWrap="wrap" alignItems="start">
          <Rimble.Box
            p=1
            width=[|1., 1., 0.3333|]
            className=Css.(style([textAlign(`center)]))>
            <img
              className=Css.(
                style([
                  // borderRadius(`percent(100.)),
                  width(`vh(25.)),
                  height(`vh(25.)),
                  objectFit(`cover),
                ])
              )
              src=orgImage
            />
            <br />
            <a
              className={Cn.make([
                Styles.navListText,
                Css.(style([fontSize(em(3.))])),
              ])}
              target="_blank"
              rel="noopener noreferrer"
              href=orgWebsite>
              orgName->restr
            </a>
            <br />
            {switch (orgDescription) {
             | Ok(descriptionArray) =>
               React.array(
                 descriptionArray->Array.mapWithIndex((i, paragraphText) =>
                   <p key={i->string_of_int}> paragraphText->React.string </p>
                 ),
               )
             | Error(_) => <p> "ok"->restr </p>
             }}
            // {optTwitter->reactMap(twitterHandle =>
            //    <a
            //      className=Styles.navListText
            //      target="_blank"
            //      rel="noopener noreferrer"
            //      href={"https://twitter.com/" ++ twitterHandle}>
            //      {("@" ++ twitterHandle)->restr}
            //    </a>
            //  )}
            <br />
          </Rimble.Box>
          <Rimble.Box p=1 width=[|1., 1., 0.3333|]>
            // <YoutubeVid videoCode="y5er2RXg3C8" />

              <h2> "Total Rasied"->restr </h2>
              <p> {(totalPatronage ++ "ETH")->restr} </p>
              <p> {(totalPatronageUsd ++ "USD")->restr} </p>
            </Rimble.Box>
          <Rimble.Box p=1 width=[|1., 1., 0.3333|]>
            {switch (orgAnimals) {
             | [||] => React.null
             | uniquePreviouslyOwnedTokens =>
               <React.Fragment>
                 <Rimble.Heading>
                   "Organisations animals"->React.string
                 </Rimble.Heading>
                 <Rimble.Flex
                   flexWrap="wrap" className=UserProfile.centreAlignOnMobile>
                   {React.array(
                      uniquePreviouslyOwnedTokens->Array.map(animal => {
                        <UserProfile.Token
                          key={animal##id->TokenId.toString}
                          tokenId={animal##id}
                        />
                      }),
                    )}
                 </Rimble.Flex>
               </React.Fragment>
             }}
          </Rimble.Box>
        </Rimble.Flex>
      </div>
    </div>;
  };
};

[@react.component]
let make = (~orgId: string) => {
  let orgData = QlHooks.useLoadOrganisationData(orgId);

  <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
    {switch (orgData) {
     | Some(data) =>
       let orgData = data##organisations_by_pk;
       switch (orgData) {
       | Some(orgData) => <OrgPage orgId orgData />
       | None =>
         <div>
           <Rimble.Heading>
             "Could not find an organisation with that ID."->React.string
           </Rimble.Heading>
         </div>
       };
     | None =>
       <div>
         <Rimble.Heading>
           "Loading Organisation Profile"->React.string
         </Rimble.Heading>
         <Rimble.Loader />
       </div>
     }}
  </Rimble.Flex>;
};
