open Globals;
open Belt;

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
    let previouslyOwnedTokens =
      patronQueryResult##patron
      ->Option.flatMap(patron => patron##previouslyOwnedTokens);
    let currentlyOwnedTokens =
      patronQueryResult##patron
      ->Option.flatMap(patron => Some(patron##tokens));
    let optProfile = UserProvider.(optThreeBoxData >>= profile);
    let image: string =
      UserProvider.(
        (
          optProfile
          >>= image
          >>= (img => img->Array.get(0))
          <$> contentUrl
          >>= (content => Js.Dict.get(content, "/"))
        )
        ->Option.mapWithDefault(Blockie.makeBlockie(. userAddress), hash =>
            "https://ipfs.infura.io/ipfs/" ++ hash
          )
      );
    let optName = UserProvider.(optProfile >>= name);
    let optDescription = UserProvider.(optProfile >>= description);
    let optTwitter =
      UserProvider.(
        optThreeBoxData >>= verifications >>= twitter <$> username
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
          width=[|1., 1., 0.33|]
          className=Css.(style([textAlign(`center)]))>
          <img
            className=Css.(
              style([borderRadius(`percent(100.)), maxWidth(`vh(50.))])
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
        <Rimble.Box p=1 width=[|1., 1., 0.33|]>
          {optMonthlyCotribution->reactMap(((ethValue, optUsdValue)) =>
             <React.Fragment>
               <h2> "Monthly Contribution"->restr </h2>
               <p>
                 ethValue->restr
                 {optUsdValue->reactMap(usdValue =>
                    ("(" ++ usdValue ++ " USD)")->restr
                  )}
               </p>
             </React.Fragment>
           )}
        </Rimble.Box>
        <Rimble.Box p=1 width=[|1., 1., 0.33|]>
          <Rimble.Heading>
            "Currently owned tokens"->React.string
          </Rimble.Heading>
          <Rimble.Flex>
            {currentlyOwnedTokens->Option.mapWithDefault(
               <p> "No currently owned tokens"->React.string </p>, a =>
               ReasonReact.array(
                 a->Array.mapWithIndex((_i, token) =>
                   <Token tokenId={token##id} />
                 ),
               )
             )}
          </Rimble.Flex>
          <br />
          <br />
          <br />
          <Rimble.Heading>
            "Previously owned tokens"->React.string
          </Rimble.Heading>
          <Rimble.Flex>
            {previouslyOwnedTokens->Option.mapWithDefault(
               <p> "No previously owned tokens"->React.string </p>, a =>
               ReasonReact.array(
                 a->Array.mapWithIndex((_i, token) =>
                   <Token tokenId={token##id} />
                 ),
               )
             )}
          </Rimble.Flex>
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
