// TODO:: check that the address is valid:
// Something like this maybe? https://docs.ethers.io/ethers.js/html/api-utils.html

module Token = {
  [@react.component]
  let make = (~tokenId) => {
    <div>
      <img
        className=Css.(style([maxWidth(px(30))]))
        src={
          tokenId
          ->Animal.getAnimalFromId
          ->Belt.Option.mapWithDefault("", a => Animal.getImage(a))
        }
      />
      <p> tokenId->React.string </p>
    </div>;
  };
};

module UserDetails = {
  [@react.component]
  let make =
      (
        ~patronQueryResult,
        ~optThreeBoxData: option(UserProvider.threeBoxUserInfo),
      ) => {
    let monthlyContributionWei =
      patronQueryResult##patron
      ->Belt.Option.flatMap(patron =>
          Some(
            patron##patronTokenCostScaledNumerator
            ->BN.mulGet(. BN.new_("2592000"))
            ->BN.divGet(. BN.new_("1000000000000")),
          )
        );
    let previouslyOwnedTokens =
      patronQueryResult##patron
      ->Belt.Option.flatMap(patron => patron##previouslyOwnedTokens);
    let currentlyOwnedTokens =
      patronQueryResult##patron
      ->Belt.Option.flatMap(patron => Some(patron##tokens));
    <div>
      {switch (optThreeBoxData) {
       | Some(threeBoxData) =>
         <React.Fragment>
           {switch (threeBoxData.profile) {
            | Some(profile) =>
              <div>
                <p>
                  {profile.name
                   ->Belt.Option.mapWithDefault("", a => a)
                   ->React.string}
                </p>
                <p>
                  {profile.description
                   ->Belt.Option.mapWithDefault("", a => a)
                   ->React.string}
                </p>
              </div>
            | None => <Rimble.Loader />
            }}
           {switch (threeBoxData.verifications) {
            | Some(verification) =>
              switch (verification.twitter) {
              | Some(twitterVerification) =>
                <React.Fragment>
                  <p>
                    {(
                       "Twitter is verified as " ++ twitterVerification.username
                     )
                     ->React.string}
                  </p>
                </React.Fragment>
              | None => <p> "Twitter isn't verified"->React.string </p>
              }
            | None => <p> "Twitter isn't verified"->React.string </p>
            }}
         </React.Fragment>
       | None => <Rimble.Loader />
       }}
      <p> "Monthly Contribution in WEI"->React.string </p>
      <p>
        {monthlyContributionWei
         ->Belt.Option.mapWithDefault(" -- ", a => a->BN.toStringGet(.))
         ->React.string}
      </p>
      <Rimble.Heading>
        "Previously owned tokens"->React.string
      </Rimble.Heading>
      <div>
        {previouslyOwnedTokens->Belt.Option.mapWithDefault(
           <p> "No previously owned tokens"->React.string </p>, a =>
           ReasonReact.array(
             Array.mapi((_i, token) => <Token tokenId={token##id} />, a),
           )
         )}
      </div>
      <Rimble.Heading> "Currently owned tokens"->React.string </Rimble.Heading>
      <div>
        {currentlyOwnedTokens->Belt.Option.mapWithDefault(
           <p> "No currently owned tokens"->React.string </p>, a =>
           ReasonReact.array(
             Array.mapi((_i, token) => <Token tokenId={token##id} />, a),
           )
         )}
      </div>
      <p> "Unstyled JSON"->React.string </p>
      <p>
        {Js.Json.stringifyAny(patronQueryResult##patron)
         ->Belt.Option.mapWithDefault(" Loading ", a => a)
         ->React.string}
      </p>
    </div>;
  };
};

[@react.component]
let make = (~userAddress: string) => {
  let userAddressLowerCase = userAddress->Js.String.toLowerCase;
  let previousTokens = QlHooks.usePatronQuery(userAddressLowerCase);
  let userInfoContext = UserProvider.useUserInfoContext();
  let reloadUser = forceReload =>
    userInfoContext.update(userAddressLowerCase, forceReload); // double check that data is loaded.
  reloadUser(false);
  let optThreeBoxData = UserProvider.use3BoxUserData(userAddressLowerCase);

  <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
    {switch (previousTokens) {
     | Some(patronQueryResult) =>
       <UserDetails optThreeBoxData patronQueryResult />
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
