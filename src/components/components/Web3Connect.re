open Belt;

[@react.component]
let make = () => {
  let connectWeb3 = RootProvider.useConnectWeb3();
  let deactivateWeb3 = RootProvider.useDeactivateWeb3();
  let networkIdOpt = RootProvider.useNetworkId();
  let currentUser = RootProvider.useCurrentUser();
  let userId =
    UserProvider.useUserNameOrTwitterHandle(
      currentUser->Option.mapWithDefault("loading", a => a),
    );
  let username = UserProvider.useUserName(userId);

  let connectedNetworkName = networkId =>
    switch (networkId) {
    | 1 => "MAINNET"->React.string
    | 5 => "GOERLI"->React.string
    | _ => "Unknown Network"->React.string
    };

  let web3Button =
    switch (networkIdOpt) {
    | Some(networkId) =>
      <Rimble.Button mainColor="#72C7D7" onClick={_e => deactivateWeb3()}>
        {connectedNetworkName(networkId)}
        <Rimble.Icon name="ExitToApp" size="16px" />
      </Rimble.Button>
    | None =>
      <Rimble.Button
        mainColor="#72C7D7"
        onClick={_e => {
          ReasonReactRouter.push("#");
          connectWeb3(RootProviderTypes.NoAction);
        }}>
        "Connect"->React.string
      </Rimble.Button>
    };

  let message =
    switch (networkIdOpt, currentUser) {
    | (None, _) => "Connect to network"
    | (Some(_), None) => "Loading user"
    | (Some(_), Some(_)) => username
    };

  <div className=Styles.loginButton>
    <Rimble.Tooltip message placement="bottom"> web3Button </Rimble.Tooltip>
  </div>;
};
