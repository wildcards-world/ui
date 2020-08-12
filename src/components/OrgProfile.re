open Globals;

[@decco.decode]
type orgDescriptionArray = array(string);

module YoutubeVid = {
  [@bs.module "./StaticContent//YoutubeVideo.js"] [@react.component]
  external make: (~videoCode: string) => React.element = "default";
};

module ComingSoonAnimal = {
  [@react.component]
  let make = (~image, ~onClick) => {
    <div className=Css.(style([width(`percent(32.))]))>
      <img
        className=Css.(style([width(`percent(100.))]))
        onClick={_e => onClick()}
        src=image
      />
    </div>;
  };
};

module OrgPage = {
  [@react.component]
  let make = (~orgData, ~orgId) => {
    let orgName = orgData##name;
    let orgDescription = orgData##description->orgDescriptionArray_decode;
    let orgAnimals = orgData##wildcard;
    let orgCommingSoon = orgData##unlaunched;
    let (selectedComingSoonAnimal, setSelectedComingSoonAnimal) =
      React.useState(() => None);
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

    // let (isModalOpen, animal) = selectedComingSoonAnimal->Option.map(selectedAnimal => orgCommingSoon[selectedAnimal])->Option.mapWithDefault((false, 0), animal)
    <div>
      <Rimble.Modal isOpen={selectedComingSoonAnimal->Option.isSome}>
        <p> "Inside"->React.string </p>
        <Rimble.Card width={Rimble.AnyStr("80vw")} p=0>
          <Rimble.Button.Text
            icononly=true
            icon="Close"
            color="moon-gray"
            position="absolute"
            right=0
            m=1
            top=0
            onClick={_ => setSelectedComingSoonAnimal(_ => None)}
          />
          <Rimble.Box p=1 mb=1>
            {switch (selectedComingSoonAnimal) {
             | Some(selectedComingSoonAnimal) =>
               let animal =
                 orgCommingSoon->Array.getUnsafe(selectedComingSoonAnimal);

               <div>
                 <h3 className=Css.(style([textAlign(center)]))>
                   {(
                      animal##name->Option.getWithDefault("Unamed")
                      ++ animal##commonName
                         ->Option.mapWithDefault("", commonName =>
                             " - " ++ commonName
                           )
                    )
                    ->React.string}
                 </h3>
                 <Rimble.Flex
                   flexWrap="wrap"
                   alignItems="start"
                   alignContent="space-arround">
                   <Rimble.Box
                     width=[|1., 1., 0.49|]
                     className=Css.(
                       style([
                         textAlign(`center),
                         alignSelf(center),
                         padding(em(0.5)),
                       ])
                     )>
                     {React.array(
                        animal##description
                        ->QlHooks.animalDescription_decode
                        ->Belt.Result.getWithDefault([||])
                        ->Array.mapWithIndex((i, paragraphText) =>
                            <p key={i->string_of_int}>
                              paragraphText->React.string
                            </p>
                          ),
                      )}
                   </Rimble.Box>
                   //  {animal##real_wc_photos
                   //   ->Array.map(photo =>
                   //       <img
                   //         className=Css.(style([width(`percent(100.))]))
                   //         // onClick={_e => onClick()}
                   //         src={Animal.cdnBase ++ photo##image}
                   //       />
                   //     )
                   //   ->React.array}
                   <Rimble.Box
                     width=[|1., 1., 0.45|]
                     className=Css.(
                       style([
                         textAlign(`center),
                         alignSelf(center),
                         padding(em(2.)),
                       ])
                     )>
                     {let photo = animal##real_wc_photos->Array.getUnsafe(0);
                      <img
                        className=Css.(style([width(`percent(100.))]))
                        // onClick={_e => onClick()}
                        src={Animal.cdnBase ++ photo##image}
                      />}
                   </Rimble.Box>
                 </Rimble.Flex>
               </div>;
             | None => React.null
             }}
          </Rimble.Box>
        </Rimble.Card>
      </Rimble.Modal>
      //  {animal->Array.map(animal => animal##)}
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
                  objectFit(`contain),
                  // objectFit(`cover),
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
             }}
            {switch (orgCommingSoon) {
             | [||] => React.null
             | _orgAnimals =>
               <React.Fragment>
                 <Rimble.Heading> "Coming soon"->React.string </Rimble.Heading>
                 <Rimble.Flex
                   flexWrap="wrap" className=UserProfile.centreAlignOnMobile>
                   React.null
                   {React.array(
                      orgCommingSoon->Array.mapWithIndex((key, animal) => {
                        animal##real_wc_photos[0]
                        ->Option.mapWithDefault(React.null, photos =>
                            <ComingSoonAnimal
                              key={key->string_of_int}
                              image={Animal.cdnBase ++ photos##image}
                              onClick={() =>
                                setSelectedComingSoonAnimal(_ => Some(key))
                              }
                            />
                          )
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
