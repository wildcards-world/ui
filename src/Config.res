@val
external network: option<string> = "process.env.REACT_APP_NETWORK"

let isTestnet = network->Option.mapWithDefault(false, network => network == "TEST")

let availableNetworkIds = isTestnet ? [4, 5, 80001] : [1, 137]
