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
  (rawProvider, rpcDefinition, (. Js.Nullable.t(error), rpcResult) => unit) =>
  unit =
  "sendAsync";

/*
   const web3 = new Web3(library.provider);

 var contract = new web3.eth.Contract(jsonInterface.abi, contractAddress);

 let functionSignature = contract.methods
   .depositWithPermit(nonce, expiry, allowed, v, r, s, account, depositAmount)
   // .encodeABI();
   .send({
     from: account,
   });
 */

[@bs.new] [@bs.module "web3"] external new_: rawProvider => t = "default";

module Contract = {
  type sendParams = {from: ethAddress};
  type txSendResult;
  type contractMethod = {
    abiEncode: unit => string,
    send: sendParams => Js.Promise.t(txSendResult),
  };

  type contract;

  type dai;

  // [@bs.send] [@bs.new] [@bs.scope "eth"]
  // external getContract: (t, abi, ethAddress) => contract = "Contract";

  // Temporary code until I work out how to make the above binding work...
  let getContract: (. t, abi, ethAddress) => contract = [%raw
    {|
      (web3, abi, contractAddress) => new web3.eth.Contract(abi, contractAddress)
    |}
  ];

  module MaticSteward = {
    type steward;

    [@bs.send] [@bs.scope "methods"]
    external depositWithPermit:
      (steward, BN.t, BN.t, bool, int, string, string, ethAddress, BN.t) =>
      contractMethod =
      "depositWithPermit";

    [@bs.send] [@bs.scope "methods"]
    external withdrawDeposit: (steward, ethAddress) => contractMethod =
      "withdrawDeposit";

    [@bs.module "./abis/abis.json"]
    external stewardMaticAbi: abi = "stewardMatic";

    let getStewardContract = (web3, address): steward => {
      getContract(. web3, stewardMaticAbi, address)->Obj.magic;
    };
  };
};
