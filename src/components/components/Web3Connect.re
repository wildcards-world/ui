[@react.component]
let make = () => {
  let connectWeb3 = RootProvider.useConnectWeb3();
  let networkId = RootProvider.useNetworkId();

  let connectionMessageState = networkId =>
    switch (networkId) {
    | Some(1) => "MAINNET"->React.string
    | Some(5) => "GOERLI"->React.string
    | Some(_) => "Unknown Network"->React.string
    | None => "Connect"->React.string
    };

  <div className=Styles.loginButton>
    <Rimble.Button
      mainColor="#72C7D7"
      disabled={!(networkId == None)}
      onClick={_e => {
        ReasonReactRouter.push("#");
        connectWeb3(RootProviderTypes.NoAction);
      }}>
      {connectionMessageState(networkId)}
    </Rimble.Button>
  </div>;
  //" "->React.string
  // <Rimble.Icon name="ExitToApp" size="16px" /> for when the logout button appears
};