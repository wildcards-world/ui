open Globals;

[@decco.decode]
type orgDescriptionArray = array(string);

module OrgPage = {
  [@react.component]
  let make = (~orgData, ~orgId) => {
    let orgName = orgData##name;
    let orgDescription = orgData##description->orgDescriptionArray_decode;
    let orgAnimals = orgData##wildcard;
    let orgAnimalsArray = orgAnimals->Array.map(animal => animal##id);
    let currentUsdEthPrice = UsdPriceProvider.useUsdPrice();
    let totalCollected = QlHooks.useTotalRaisedAnimalGroup(orgAnimalsArray);
    // let totalCollectedString =
    //   totalCollected->oMap(a => a->BN.toStringGet(.)) |||| "Loading";
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
    //  let orgName = data##name;
    <div>
      <p> "profile"->restr </p>
      <img src=orgImage />
      <p> orgName->restr </p>
      {switch (orgDescription) {
       | Ok(descriptionArray) =>
         ReasonReact.array(
           descriptionArray->Array.mapWithIndex((i, paragraphText) =>
             <p key={i->string_of_int}> paragraphText->React.string </p>
           ),
         )
       | Error(_) => <p> "ok"->restr </p>
       }}
      <p> "totalRaised:"->restr </p>
      <p> {(totalPatronage ++ "ETH")->restr} </p>
      <p> {(totalPatronageUsd ++ "USD")->restr} </p>
      <a href=orgWebsite> "Website"->restr </a>
      {switch (orgAnimals) {
       | [||] => React.null
       | uniquePreviouslyOwnedTokens =>
         <React.Fragment>
           <Rimble.Heading>
             "Organisations animals"->React.string
           </Rimble.Heading>
           <Rimble.Flex
             flexWrap="wrap" className=UserProfile.centreAlignOnMobile>
             {ReasonReact.array(
                uniquePreviouslyOwnedTokens->Array.mapWithIndex((i, animal) => {
                  <UserProfile.Token
                    key={i->string_of_int}
                    tokenId={animal##id}
                  />
                }),
              )}
           </Rimble.Flex>
         </React.Fragment>
       }}
    </div>;
  };
};

[@react.component]
let make = (~orgId: string) => {
  let orgData = QlHooks.useLoadOrganisationData(orgId);

  Js.log(orgData);

  // let (totalPatronage, totalPatronageUsd) =
  //   QlHooks.useAmountRaisedToken(animal)
  //   ->mapd(("Loading", "Loading"), a =>
  //       (
  //         (a->Eth.get(Eth.Eth(`ether))->Float.fromString |||| 0.0)
  //         ->toFixedWithPrecisionNoTrailingZeros(~digits=9),
  //         currentUsdEthPrice->mapd("Loading", usdEthRate =>
  //           a->Eth.get(Eth.Usd(usdEthRate, 2))
  //         ),
  //       )
  //     );
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
           "Loading user profile:"->React.string
         </Rimble.Heading>
         <Rimble.Loader />
       </div>
     }}
  </Rimble.Flex>;
};
