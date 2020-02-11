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

module TopContributors = {
  [@react.component]
  let make = (~topContributors) => {
    ReasonReact.array(
      Array.mapi(
        (index, (contributor, amount)) => {
          <ContributorsRow contributor amount index />
        },
        topContributors,
      ),
    );
  };
};

[@react.component]
let make = () => {
  let topContributorsOpt = QlHooks.useLoadTopContributorsData();

  <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
    <div> "hello"->React.string </div>
    <div>
      {switch (topContributorsOpt) {
       | Some(topContributors) => <TopContributors topContributors />
       | None => React.null
       }}
    </div>
  </Rimble.Flex>;
};
