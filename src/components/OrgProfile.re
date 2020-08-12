open Globals;

[@decco.decode]
type orgDescriptionArray = array(string);

module YoutubeVid = {
  [@bs.module "./StaticContent//YoutubeVideo.js"] [@react.component]
  external make: (~videoCode: string) => React.element = "default";
};

// module Token = {
//   [@react.component]
//   let make = (~tokenId: TokenId.t) => {
//     let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
//     let image = Animal.useAvatar(tokenId);

//     <div className=Css.(style([width(vh(12.))]))>
//       <img
//         className=Css.(style([width(`percent(100.))]))
//         onClick={_e =>
//           clearAndPush("/#details/" ++ tokenId->TokenId.toString)
//         }
//         src=image
//       />
//     </div>;
//   };
// };

module OrgPage = {
  [@react.component]
  let make = (~orgData, ~orgId) => {
    let orgName = orgData##name;
    let orgDescription = orgData##description->orgDescriptionArray_decode;
    let orgAnimals = orgData##wildcard;
    //     unlaunched: wildcard (where: {id: {_is_null: true}}) {
    // real_wc_photos {
    //     image
    //     photographer
    let orgCommingSoon = orgData##unlaunched;
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
    let optOrgYoutubeVid = orgData##youtube_vid;
    let orgImage = Animal.useGetOrgImage(orgId);

    <div>
      <div className=Css.(style([width(`percent(100.))]))>
        <Rimble.Flex
          flexWrap="wrap" alignItems="start" alignContent="space-arround">
          <Rimble.Box
            width=[|1., 1., 0.3333|]
            className=Css.(
              style([
                textAlign(`center),
                alignSelf(center),
                padding(em(2.)),
              ])
            )>
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
            <div
              className=Css.(
                style([maxHeight(`em(15.)), overflow(`scroll)])
              )>
              {switch (orgDescription) {
               | Ok(descriptionArray) =>
                 React.array(
                   descriptionArray->Array.mapWithIndex((i, paragraphText) =>
                     <p key={i->string_of_int}>
                       paragraphText->React.string
                     </p>
                   ),
                 )
               | Error(_) => <p> "error loading description"->restr </p>
               }}
            </div>
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
          <Rimble.Box
            width=[|1., 1., 0.3333|]
            className=Css.(style([alignSelf(center), padding(em(2.))]))>
            {switch (optOrgYoutubeVid) {
             | Some(videoCode) => <YoutubeVid videoCode />
             | None => React.null
             }}
            <h2> "Total Raised"->restr </h2>
            {(totalPatronage ++ "ETH")->restr}
            <br />
            <small> {(totalPatronageUsd ++ "USD")->restr} </small>
          </Rimble.Box>
          <Rimble.Box
            width=[|1., 1., 0.3333|]
            className=Css.(style([alignSelf(center), padding(em(2.))]))>
            {switch (orgAnimals) {
             | [||] =>
               <p>
                 "This organisation doesn't have any wildcards yet"
                 ->React.string
               </p>
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
                 //  <Rimble.Heading> "Coming soon"->React.string </Rimble.Heading>
                 //  <Rimble.Flex
                 //    flexWrap="wrap" className=UserProfile.centreAlignOnMobile>
                 //    {React.array(
                 //       orgCommingSoon->Array.mapWithIndex((key, animal) => {
                 //         <UserProfile.Token
                 //           key={key->string_of_int}
                 //           tokenId={animal##id}
                 //         />
                 //       }),
                 //     )}
                 //  </Rimble.Flex>
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
