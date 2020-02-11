open ReasonApolloHooks;

module LoadMostContributed = [%graphql
  {|
    query {
      patrons(first: 20, orderBy: totalContributed, orderDirection: desc) {
        id
        patronTokenCostScaledNumerator  @bsDecoder(fn: "QlHooks.decodeBN")
        totalContributed @bsDecoder(fn: "QlHooks.decodeBN")
        lastUpdated @bsDecoder(fn: "QlHooks.decodeBN")
      }
    }
  |}
];

let useLoadMostContributed = () =>
  ApolloHooks.useSubscription(LoadMostContributed.definition);
let useLoadMostContributedData = () => {
  let (simple, _) = useLoadMostContributed();
  let currentTimestamp = QlHooks.useCurrentTime();
  switch (simple) {
  | Data(largestContributors) =>
    let dailyContributions =
      largestContributors##patrons
      |> Js.Array.map(patron => {
           let timeElapsed =
             BN.new_(currentTimestamp)->BN.subGet(. patron##lastUpdated);

           let amountContributedSinceLastUpdate =
             patron##patronTokenCostScaledNumerator
             ->BN.mulGet(. timeElapsed) // A month with 30 days has 2592000 seconds
             ->BN.divGet(.
                 // BN.new_("1000000000000")->BN.mulGet(. BN.new_("31536000")),
                 BN.new_("31536000000000000000"),
               );

           let totalContributedWei =
             patron##totalContributed
             ->BN.addGet(. amountContributedSinceLastUpdate);

           (patron##id, totalContributedWei);
         });
    Array.sort(
      ((_, first), (_, second)) => {second->BN.cmpGet(. first)},
      dailyContributions,
    );
    Some(dailyContributions);
  | _ => None
  };
};

module ContributorsRow = {
  [@react.component]
  let make = (~contributor, ~amount, ~index) => {
    UserProvider.useUserInfoContext().update(contributor, false);

    let optThreeBoxData = UserProvider.use3BoxUserData(contributor);
    let optUserName =
      Belt.Option.(
        optThreeBoxData
        ->flatMap(threeBoxData => threeBoxData.profile)
        ->flatMap(threeBoxData => threeBoxData.name)
      );

    <div>
      <div> {(index + 1)->string_of_int->React.string} </div>
      {switch (optUserName) {
       | Some(name) => <div> name->React.string </div>
       | None => <div> contributor->React.string </div>
       }}
      <div> {(amount ++ " ETH")->React.string} </div>
    </div>;
  };
};

module MostContributed = {
  [@react.component]
  let make = (~highestContributors) => {
    ReasonReact.array(
      Array.mapi(
        (index, (contributor, amount)) => {
          <ContributorsRow
            contributor
            amount={amount->Web3Utils.fromWeiBNToEthPrecision(~digits=4)}
            index
          />
        },
        highestContributors,
      ),
    );
  };
};

[@react.component]
let make = () => {
  let highestContributorsOpt = useLoadMostContributedData();

  <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
    <div>
      {switch (highestContributorsOpt) {
       | Some(highestContributors) => <MostContributed highestContributors />
       | None => React.null
       }}
    </div>
  </Rimble.Flex>;
};
