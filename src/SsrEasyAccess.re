// temporary file to put things I will be using for SSR.
let link = ExperimentalSSRClient.webSocketHttpLink(5);
let getClientInstance = ExperimentalSSRClient.instance(~link);
