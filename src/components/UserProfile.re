open Globals;
open Belt;

// TODO:: check that the address is valid:
// Something like this maybe? https://docs.ethers.io/ethers.js/html/api-utils.html
module Token = {
  [@react.component]
  let make = (~tokenId) => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
    let animal = Animal.getNameFromId(tokenId);

    <div>
      <img
        className=Css.(style([maxWidth(vh(15.))]))
        onClick={_e => clearAndPush("/#details/" ++ animal)}
        src={
          tokenId
          ->Animal.getAnimalFromId
          ->Option.mapWithDefault("", a => Animal.getImage(a))
        }
      />
    </div>;
  };
};

module UserDetails = {
  [@react.component]
  let make =
      (
        ~patronQueryResult,
        ~optThreeBoxData: option(UserProvider.threeBoxUserInfo),
        ~userAddress: string,
      ) => {
    let currentlyOwnedTokens =
      (patronQueryResult##patron <$> (patron => patron##tokens))
      ->Option.mapWithDefault([||], a => a->Array.map(token => token##id));
    let allPreviouslyOwnedTokens: array(string) =
      (patronQueryResult##patron >>= (patron => patron##previouslyOwnedTokens))
      ->Option.mapWithDefault([||], a => a->Array.map(token => token##id));
    let uniquePreviouslyOwnedTokens =
      Set.String.fromArray(allPreviouslyOwnedTokens)
      ->Set.String.removeMany(currentlyOwnedTokens)
      ->Set.String.toArray;

    let optProfile = optThreeBoxData >>= (a => a.profile);
    let image: string =
      (
        optProfile
        >>= (a => a.image)
        >>= (img => img->Array.get(0))
        <$> (a => a.contentUrl)
        >>= (content => Js.Dict.get(content, "/"))
      )
      ->Option.mapWithDefault(Blockie.makeBlockie(. userAddress), hash =>
          "https://ipfs.infura.io/ipfs/" ++ hash
        );
    let optName = optProfile >>= (a => a.name);
    let optDescription = optProfile >>= (a => a.description);
    let optTwitter =
      UserProvider.(
        optThreeBoxData
        >>= (a => a.verifications)
        >>= (a => a.twitter)
        <$> (a => a.username)
      );
    let etherScanUrl = RootProvider.useEtherscanUrl();

    let optUsdPrice = UsdPriceProvider.useUsdPrice();

    let optMonthlyCotribution = {
      patronQueryResult##patron
      <$> (
        patron =>
          patron##patronTokenCostScaledNumerator
          ->BN.mulGet(. BN.new_("2592000")) // A month with 30 days has 2592000 seconds
          ->BN.divGet(.
              // BN.new_("1000000000000")->BN.mulGet(. BN.new_("31536000")),
              BN.new_("31536000000000000000"),
            )
      )
      <$> (
        monthlyContributionWei => {
          let monthlyContributionEth =
            monthlyContributionWei->Web3Utils.fromWeiBNToEthPrecision(
              ~digits=4,
            );
          let optMonthlyContributionUsd =
            optUsdPrice
            <$> (
              currentUsdEthPrice =>
                Js.Float.toFixedWithPrecision(
                  Float.fromString(monthlyContributionEth)
                  ->Option.mapWithDefault(0., a => a)
                  *. currentUsdEthPrice,
                  ~digits=2,
                )
            );
          (monthlyContributionEth, optMonthlyContributionUsd);
        }
      );
    };

    <div className=Css.(style([width(`percent(100.))]))>
      <Rimble.Flex alignItems="start">
        <Rimble.Box
          p=1
          width=[|1., 1., 0.3333|]
          className=Css.(style([textAlign(`center)]))>
          <img
            className=Css.(
              style([
                borderRadius(`percent(100.)),
                width(`vh(25.)),
                height(`vh(25.)),
                objectFit(`cover),
              ])
            )
            src=image
          />
          <br />
          {optName->reactMap(name => <h2> name->restr </h2>)}
          {optTwitter->reactMap(twitterHandle =>
             <a
               className=Styles.navListText
               target="_blank"
               href={"https://twitter.com/" ++ twitterHandle}>
               {("@" ++ twitterHandle)->restr}
             </a>
           )}
          <br />
          {optDescription->reactMap(description => <p> description->restr </p>)}
          <a
            className=Styles.navListText
            target="_blank"
            href={"https://" ++ etherScanUrl ++ "/address/" ++ userAddress}>
            {Helper.elipsify(userAddress, 10)->restr}
          </a>
        </Rimble.Box>
        <Rimble.Box p=1 width=[|1., 1., 0.3333|]>
          <h2> "Monthly Contribution"->restr </h2>
          <p>
            {optMonthlyCotribution->reactMapWithDefault(
               {
                 "0 ETH";
               }
               ->restr,
               ((ethValue, optUsdValue)) =>
               <React.Fragment>
                 {j|$ethValue ETH\xa0|j}->restr
                 {optUsdValue->reactMap(usdValue =>
                    <small> {j|($usdValue USD)|j}->restr </small>
                  )}
               </React.Fragment>
             )}
          </p>
        </Rimble.Box>
        <Rimble.Box p=1 width=[|1., 1., 0.3333|]>
          {switch (currentlyOwnedTokens) {
           | [||] => <p> "User has never owned a wildcard."->restr </p>
           | currentlyOwnedTokens =>
             <React.Fragment>
               <Rimble.Heading>
                 "Currently owned tokens"->React.string
               </Rimble.Heading>
               <Rimble.Flex>
                 {ReasonReact.array(
                    currentlyOwnedTokens->Array.mapWithIndex((i, tokenId) =>
                      <Token key={i->string_of_int} tokenId />
                    ),
                  )}
               </Rimble.Flex>
               <br />
               <br />
               <br />
               {switch (uniquePreviouslyOwnedTokens) {
                | [||] => <p> "User has never owned a wildcard."->restr </p>
                | uniquePreviouslyOwnedTokens =>
                  <React.Fragment>
                    <Rimble.Heading>
                      "Previously owned tokens"->React.string
                    </Rimble.Heading>
                    <Rimble.Flex>
                      {ReasonReact.array(
                         uniquePreviouslyOwnedTokens->Array.mapWithIndex(
                           (i, tokenId) =>
                           <Token key={i->string_of_int} tokenId />
                         ),
                       )}
                    </Rimble.Flex>
                  </React.Fragment>
                }}
             </React.Fragment>
           }}
        </Rimble.Box>
      </Rimble.Flex>
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
       <UserDetails optThreeBoxData patronQueryResult userAddress />
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
