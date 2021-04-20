let infoModalStyle = {
  open Css
  style(list{padding(rem(3.)), borderRadius(px(5)), width(px(640)), maxWidth(#vw(100.))})
}

@react.component
let make = (~clickAction=() => ()) => {
  let connectWeb3 = RootProvider.useConnectWeb3()
  let deactivateWeb3 = RootProvider.useDeactivateWeb3()
  let networkIdOpt = RootProvider.useNetworkId()

  let connectedNetworkName = networkId =>
    switch networkId {
    | 1 => j`MAINNET\\xa0`->React.string
    | 3 => j`ROPSTEN\\xa0`->React.string
    | 4 => j`RINKEBY\\xa0`->React.string
    | 5 => j`GOERLI\\xa0`->React.string
    | 42 => j`KOVAN\\xa0`->React.string
    | 137 => j`MATIC\\xa0`->React.string
    | 80001 => j`MATIC Test\\xa0`->React.string
    | _ => j`Unknown\\xa0`->React.string
    }

  let web3Button = switch networkIdOpt {
  | Some(networkId) =>
    <Rimble.Button
      mainColor="#72C7D7"
      onClick={_e => {
        clickAction()
        deactivateWeb3()
      }}>
      {connectedNetworkName(networkId)} <Rimble.Icon name="ExitToApp" size="16px" />
    </Rimble.Button>
  | None =>
    <Rimble.Button
      mainColor="#72C7D7"
      onClick={_e => {
        clickAction()
        ReasonReactRouter.push("#")
        connectWeb3(RootProviderTypes.NoAction)
      }}>
      {"Log In"->React.string}
    </Rimble.Button>
  }

  <div className=Styles.loginButton> web3Button </div>
}
