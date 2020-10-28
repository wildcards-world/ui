type provider;

type t;

type abi;

type ethAddress = string;
type ethersBigNumber = {toString: (. unit) => string};

type rawProvider;

type web3Library = {
  getBalance: (. ethAddress) => Promise.promise(option(ethersBigNumber)),
  getSigner: (. ethAddress) => web3Library,
  provider: rawProvider,
};

type rpcDefinition = {
  method: string,
  params: array(string),
  from: ethAddress,
};

type error;
type result;
type rpcResult = {result};

[@bs.send]
external sendAsync:
  (rawProvider, rpcDefinition, (. option(error), rpcResult) => unit) => unit =
  "sendAsync";
