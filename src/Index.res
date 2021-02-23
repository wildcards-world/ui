%bs.raw(`require("react-tabs/style/react-tabs.css")`)

module Router = {
  @react.component
  let make = () => {
    let url = ReasonReactRouter.useUrl()
    switch url.path {
    | list{_} => <p> {React.string("Unknown page")} </p>
    | _ => <ReactTranslate> <Layout /> </ReactTranslate>
    }
  }
}

@val
external mainnetApi: option<string> = "process.env.REACT_APP_MAINNET_BE"
@val external goerliApi: option<string> = "process.env.REACT_APP_GOERLI_BE"
@val
external maticTestnetApi: option<string> = "process.env.REACT_APP_MATIC_TESTNET"
@val
external maticAltTestnetApi: option<string> = "process.env.REACT_APP_MATIC_TESTNET_ALT"
@val external maticApi: option<string> = "process.env.REACT_APP_MATIC_BE"
@val
external rinkebyApi: option<string> = "process.env.REACT_APP_RINKEBY_BE"

ReactDOMRe.renderToElementWithId(
  <WildcardsProvider
    getGraphEndpoints={(networkId, ()) => {
      open Client

      let endpoints = switch networkId {
      | 5 => {
          mainnet: Option.getWithDefault(goerliApi, "https://goerli.api.wildcards.world/v1/graphq"),
          matic: Option.getWithDefault(
            maticTestnetApi,
            "https://api.thegraph.com/subgraphs/name/wildcards-world/wildcards-polygon-testnet",
          ),
          ws: "wss://api.thegraph.com/subgraphs/name/wildcards-world/wildcards-goerli",
        }
      | 4 => {
          mainnet: Option.getWithDefault(
            rinkebyApi,
            "https://rinkeby.api.wildcards.world/v1/graphq",
          ),
          matic: Option.getWithDefault(
            maticAltTestnetApi,
            "https://api.thegraph.com/subgraphs/name/wildcards-world/wildcards-testnet",
          ),
          ws: "wss://api.thegraph.com/subgraphs/name/wildcards-world/wildcards-goerli",
        }
      | _ => {
          mainnet: Option.getWithDefault(mainnetApi, "https://api.wildcards.world/v1/graphq"),
          matic: Option.getWithDefault(
            maticApi,
            "https://api.thegraph.com/subgraphs/name/wildcards-world/wildcards-polygon",
          ),
          ws: "wss://api.thegraph.com/subgraphs/name/wildcards-world/wildcards",
        }
      }
      endpoints
    }}>
    <UsdPriceProvider> <Router /> </UsdPriceProvider> <DiscordChat />
  </WildcardsProvider>,
  "root",
)
