open Globals;

let centreAlignOnMobile =
  Css.(
    style([
      media(
        "(max-width: 831px)",
        [alignItems(center), justifyContent(center)],
      ),
    ])
  );

module ArtistDetails = {
  [@react.component]
  let make =
      (
        ~optThreeBoxData: option(UserProvider.threeBoxUserInfo),
        ~artistEthAddress: string,
        ~optArtistName,
        ~optArtistWebsite,
        ~optArtistWildcards,
        ~optArtistOrgs,
      ) => {
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();
    let optProfile = optThreeBoxData >>= (a => a.profile);
    let image: string =
      (
        optProfile
        >>= (a => a.image)
        >>= (img => img->Array.get(0))
        <$> (a => a.contentUrl)
        >>= (content => Js.Dict.get(content, "/"))
      )
      ->Option.mapWithDefault(Blockie.makeBlockie(. artistEthAddress), hash =>
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

    let artistsAnimalsArray =
      optArtistWildcards->Option.mapWithDefault([||], animals =>
        animals->Array.map(animal => animal##id)
      );
    let currentUsdEthPrice = UsdPriceProvider.useUsdPrice();
    let (totalCollectedMainnetEth, totalCollectMaticDai) =
      QlHooks.useTotalRaisedAnimalGroup(artistsAnimalsArray);

    let (totalPatronageUsd, totalBreakdownString) =
      switch (totalCollectedMainnetEth, totalCollectMaticDai) {
      | (Some(mainnetEth), Some(maticDai)) => (
          (
            currentUsdEthPrice->Option.mapWithDefault(0., usdEthRate =>
              mainnetEth->Eth.getFloat(Eth.Usd(usdEthRate, 2))
            )
            +. maticDai->Eth.getFloat(Eth.Eth(`ether))
          )
          ->Js.Float.toFixedWithPrecision(~digits=6),
          mainnetEth->Web3Utils.fromWeiBNToEthPrecision(~digits=4)
          ++ " ETH + "
          ++ maticDai->Web3Utils.fromWeiBNToEthPrecision(~digits=2)
          ++ " DAI",
        )
      | _ => ("loading", "loading")
      };

    let nonUrlState = RootProvider.useNonUrlState();
    let clearNonUrlState = RootProvider.useClearNonUrlState();

    <div className=Css.(style([width(`percent(100.))]))>
      <Rimble.Flex flexWrap="wrap" alignItems="start">
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
          {switch (nonUrlState) {
           | UserVerificationScreen =>
             <div className=Css.(style([position(`relative)]))>
               <Rimble.Button.Text
                 icononly=true
                 icon="Close"
                 color="moon-gray"
                 position="absolute"
                 top=0
                 right=0
                 m=1
                 onClick={_ => clearNonUrlState()}
               />
               <React.Suspense fallback={<Rimble.Loader />}>
                 <LazyThreeBoxUpdate />
               </React.Suspense>
             </div>
           | UpdateDepositScreen
           | LoginScreen(_)
           | UpdatePriceScreen(_)
           | BuyScreen(_)
           | AuctionScreen(_)
           | NoExtraState =>
             <>
               {<h2>
                  {optName
                   ->Option.getWithDefault(
                       optArtistName |||| "Loading artist name",
                     )
                   ->React.string}
                </h2>}
               {optTwitter->reactMap(twitterHandle =>
                  <a
                    className=Styles.navListText
                    target="_blank"
                    rel="noopener noreferrer"
                    href={"https://twitter.com/" ++ twitterHandle}>
                    {("@" ++ twitterHandle)->React.string}
                  </a>
                )}
               <br />
               {optDescription->reactMap(description =>
                  <p> description->React.string </p>
                )}
               {optArtistWebsite->Option.mapWithDefault(React.null, website =>
                  <a
                    className=Styles.navListText
                    target="_blank"
                    rel="noopener noreferrer"
                    href=website>
                    "Artists website"->React.string
                  </a>
                )}
               <br />
               <br />
               <a
                 className=Styles.navListText
                 target="_blank"
                 rel="noopener noreferrer"
                 href={
                   "https://"
                   ++ etherScanUrl
                   ++ "/address/"
                   ++ artistEthAddress
                 }>
                 {Helper.elipsify(artistEthAddress, 10)->React.string}
               </a>
               <br />
             </>
           }}
        </Rimble.Box>
        <Rimble.Box p=1 width=[|1., 1., 0.3333|]>
          <h2> "Total contributed by artist"->React.string </h2>
          {(totalPatronageUsd ++ "USD")->React.string}
          <br />
          <small> totalBreakdownString->React.string </small>
          {switch (optArtistOrgs) {
           | None => <h2> "Loading orgs"->React.string </h2>
           | Some([||]) => React.null
           | Some(orgList) =>
             <>
               <br />
               <br />
               <h4>
                 "Organisations this artist has contributed to:"->React.string
               </h4>
               {orgList
                ->Array.map(org =>
                    QlHooks.(
                      <div
                        className=Css.(
                          style([width(vh(12.)), cursor(`pointer)])
                        )>
                        <img
                          className=Css.(style([width(`percent(100.))]))
                          onClick={_e => clearAndPush("/#org/" ++ org.id)}
                          src={CONSTANTS.cdnBase ++ org.logo}
                        />
                      </div>
                    )
                  )
                ->React.array}
             </>
           }}
        </Rimble.Box>
        <Rimble.Box p=1 width=[|1., 1., 0.3333|]>
          {switch (optArtistWildcards) {
           | None => <p> "loading artists wildcards"->React.string </p>
           | Some([||]) =>
             <p> "Artist hasn't created any wildcards yet"->React.string </p>
           | Some(currentlyOwnedTokens) =>
             <React.Fragment>
               <Rimble.Heading>
                 "Wildcards created by artist"->React.string
               </Rimble.Heading>
               <Rimble.Flex flexWrap="wrap" className=centreAlignOnMobile>
                 {React.array(
                    currentlyOwnedTokens->Array.map(token => {
                      let id = token##id->TokenId.toString;
                      <UserProfile.Token
                        key=id
                        tokenId={TokenId.fromStringUnsafe(id)}
                      />;
                    }),
                  )}
               </Rimble.Flex>
               <br />
               <br />
               <br />
             </React.Fragment>
           }}
        </Rimble.Box>
      </Rimble.Flex>
    </div>;
  };
};

[@react.component]
let make = (~artistIdentifier: string) => {
  // let artistAddress =
  Js.log(artistIdentifier);

  let artistEthAddress =
    (
      QlHooks.useArtistEthAddress(~artistIdentifier)
      |||| CONSTANTS.nullEthAddress
    )
    ->Js.String.toLowerCase;
  let optArtistName = QlHooks.useArtistName(~artistIdentifier);
  let optArtistWebsite = QlHooks.useArtistWebsite(~artistIdentifier);
  let optArtistWildcards = QlHooks.useArtistWildcards(~artistIdentifier);
  let optArtistOrgs = QlHooks.useArtistOrgs(~artistIdentifier);

  let userInfoContext = UserProvider.useUserInfoContext();
  let reloadUser = forceReload =>
    userInfoContext.update(artistEthAddress, forceReload);
  reloadUser(false);
  let optThreeBoxData = UserProvider.use3BoxUserData(artistEthAddress);
  <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
    <ArtistDetails
      artistEthAddress
      optArtistName
      optArtistWebsite
      optArtistWildcards
      optArtistOrgs
      optThreeBoxData
    />
  </Rimble.Flex>;
};
