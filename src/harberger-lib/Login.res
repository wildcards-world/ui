open Web3Connectors

module ConnectorConfig = {
  let pollingInterval = 8000

  let mainnetRpcUrl = "https://mainnet.infura.io/v3/84842078b09946638c03157f83405213"
  let goerliRpcUrl = "https://goerli.infura.io/v3/84842078b09946638c03157f83405213"

  let portisKey = "1456ad21-b5a7-4364-9eba-e6237a7cc1e9"
  let fortmaticKey = "pk_live_BE64CE1BB4A49C37"
  let squarelinkKey = "3904cdd1b675af615ca9"
}

let connectors = [
  {
    name: "MetaMask",
    connector: injected,
    img: "/img/wallet-icons/metamask.svg",
    connectionPhrase: "Connect to your MetaMask Wallet",
  },
  {
    name: "WalletConnect",
    connector: WalletConnectConnector.make({
      rpc: {"1": ConnectorConfig.mainnetRpcUrl},
      bridge: "https://bridge.walletconnect.org",
      qrcode: true,
      pollingInterval: ConnectorConfig.pollingInterval,
    }),
    connectionPhrase: "Connect via WalletConnect",
    img: "/img/wallet-icons/walletConnect.svg",
  },
  {
    name: "Torus",
    connector: TorusConnector.make({chainId: 1}),
    connectionPhrase: "Connect via Torus",
    img: "/img/wallet-icons/torus.svg",
  },
  {
    name: "Portis",
    connector: PortisConnector.make({
      dAppId: ConnectorConfig.portisKey,
      networks: [1],
    }),
    connectionPhrase: "Connect via Portis",
    img: "/img/wallet-icons/portis.svg",
  },
  {
    name: "Fortmatic",
    connector: FortmaticConnector.make({
      apiKey: ConnectorConfig.fortmaticKey,
      chainId: 1,
    }),
    connectionPhrase: "Connect via Fortmatic",
    img: "/img/wallet-icons/fortmatic.svg",
  },
  // NOTE: Squarelink seems to be broken
  // {
  //   name: "Squarelink",
  //   connector: SquarelinkConnector.make({
  //     clientId: ConnectorConfig.squarelinkKey,
  //     networks: [1],
  //   }),
  //   connectionPhrase: "Connect via Squarelink",
  //   img: "/img/wallet-icons/squarelink.svg",
  // },
]

@react.component
let make = () => {
  let (_connectionStatus, activateConnector) = RootProvider.useActivateConnector()

  <div>
    <p>
      {"Use one of the wallet providers below. "->React.string}
      <small>
        {"(Not sure where to go from here? "->React.string}
        <a
          href="https://blog.wildcards.world/how-to-buy-a-wildcard-web3-ethereum/"
          target="_blank"
          rel="noopener noreferrer">
          <span className=Styles.colorGreen> {"Read this guide"->React.string} </span>
        </a>
        {")"->React.string}
      </small>
    </p>
    <div
      className={
        open CssJs
        style(. [
          display(#grid),
          gridTemplateColumns([#repeat(#autoFit, #minmax(px(176), fr(0.6)))]),
          maxWidth(px(800)),
        ])
      }>
      {connectors
      ->Array.mapWithIndex((index, connector) =>
        <div
          key={index->string_of_int}
          onClick={e => {
            ReactEvent.Mouse.stopPropagation(e)
            activateConnector(connector.connector)
          }}
          className={
            open CssJs
            style(. [zIndex(1), border(px(1), #solid, rgba(195, 195, 195, #num(0.14)))])
          }>
          <div
            className={
              open CssJs
              style(. [
                margin(px(8)),
                display(#flex),
                justifyContent(#center),
                alignItems(#center),
                flexDirection(column),
                cursor(#pointer),
                borderRadius(px(12)),
                backgroundColor(white),
                hover([backgroundColor(rgb(195, 195, 195))]),
                transition(~duration=200, ~delay=0, ~timingFunction=easeInOut, "background-color"),
              ])
            }>
            <div
              className={
                open Css
                style(list{width(px(45)), height(px(45))})
              }>
              <img
                src=connector.img
                alt=connector.name
                className={
                  open Css
                  style(list{width(#percent(100.)), height(#percent(100.))})
                }
              />
            </div>
            <div
              className={
                open Css
                style(list{fontSize(px(24)), fontWeight(#num(700)), marginTop(em(0.5))})
              }>
              {connector.name->React.string}
            </div>
            <div
              className={
                open Css
                style(list{fontSize(px(15)), marginTop(em(0.35)), color(rgb(169, 169, 188))})
              }>
              {connector.connectionPhrase->React.string}
            </div>
          </div>
        </div>
      )
      ->React.array}
    </div>
  </div>
}
