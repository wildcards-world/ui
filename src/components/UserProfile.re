// TODO:: check that the address is valid:
// Something like this maybe? https://docs.ethers.io/ethers.js/html/api-utils.html

module UserDetails = {
  [@react.component]
  let make = (~patronQueryResult) => {
    let monthlyContributionWei =
      patronQueryResult##patron
      ->Belt.Option.flatMap(patron =>
          Some(
            patron##patronTokenCostScaledNumerator
            ->BN.mulGet(. BN.new_("2592000"))
            ->BN.divGet(. BN.new_("1000000000000")),
          )
        );

    <div>
      <p> "Monthly Contribution in WEI"->React.string </p>
      <p>
        {monthlyContributionWei
         ->Belt.Option.mapWithDefault(" -- ", a => a->BN.toStringGet(.))
         ->React.string}
      </p>
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
  let previousTokens = QlHooks.usePatronQuery(userAddress);

  <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
    {switch (previousTokens) {
     | Some(patronQueryResult) => <UserDetails patronQueryResult />
     | None =>
       <div>
         <p> "the new page"->React.string </p>
         <p> "the new page"->React.string </p>
       </div>
     }}
  </Rimble.Flex>;
};
