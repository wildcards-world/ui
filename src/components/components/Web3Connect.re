[@react.component]
let make = () => {
  let connectWeb3 = RootProvider.useConnectWeb3();
  let deactivateWeb3 = RootProvider.useDeactivateWeb3();
  let networkIdOpt = RootProvider.useNetworkId();

  let connectedNetworkName = networkId =>
    switch (networkId) {
    | 1 => {j|MAINNET\xa0|j}->React.string
    | 5 => {j|GOERLI\xa0|j}->React.string
    | _ => {j|Unknown\xa0|j}->React.string
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

  <div className=Styles.loginButton> web3Button </div>;
};
