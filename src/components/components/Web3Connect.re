[@react.component]
let make = () => {
  let connectWeb3 = RootProvider.useConnectWeb3();
  let networkId = RootProvider.useNetworkId();
  let currentUser =
    RootProvider.useCurrentUser()->Belt.Option.mapWithDefault("", a => a);

  let userNameLogin: string => string =
    currentUser => {
      let userId = UserProvider.useUserNameOrTwitterHandle(currentUser);
      let username = UserProvider.useUserName(userId);
      username;
    };

  let connectionMessageState = networkId =>
    switch (networkId) {
    | Some(1) => "MAINNET"->React.string
    | Some(5) => "GOERLI"->React.string
    | Some(_) => "Unknown Network"->React.string
    | None => "Connect"->React.string
    };

  <div className=Styles.loginButton>
    <Rimble.Tooltip
      message={
        String.length(currentUser) > 0
          ? userNameLogin(currentUser) : "Connect"
      }
      placement="bottom">
      <Rimble.Button
        mainColor="#72C7D7"
        // disabled={!(networkId == None)} Tooltip doesnt work with disabled
        onClick={_e => {
          ReasonReactRouter.push("#");
          connectWeb3(RootProviderTypes.NoAction);
        }}>
        {connectionMessageState(networkId)}
      </Rimble.Button>
    </Rimble.Tooltip>
  </div>;
  // for when the logout button is used, to be put inside the button closing tag and remove disabled
  //" "->React.string
  // <Rimble.Icon name="ExitToApp" size="16px" />
};