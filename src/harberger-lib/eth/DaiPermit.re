open Globals;

type daiContract;
[@bs.send]
external getNonce: (daiContract, Web3.ethAddress) => Js.Promise.t(BN.t) =
  "getNonce";

[@bs.new] [@bs.module "ethers"]
external getContract:
  (Web3.ethAddress, Web3.abi, Web3.web3Library) => daiContract =
  "Contract";

let getDaiContract = (daiAddress, stewardAbi, library, account) => {
  getContract(
    daiAddress,
    stewardAbi,
    ContractActions.getProviderOrSigner(library, account, false),
  );
};

[@bs.module "./abi/dai.json"] external daiAbi: Web3.abi = "dai";

let getNonce = (daiContractAddress, library, account) => {
  let daiContract =
    getDaiContract(daiContractAddress, daiAbi, library, account);

  let callDai: (. daiContract) => unit = [%raw
    {|
    async (daiContract) => {
      try {
        let result = await daiContract.getNonce("0xd3Cbce59318B2E570883719c8165F9390A12BdD6");
        console.log("the result", result)
      } catch (e) {
        console.log("the error:", e);
      }
    }
  |}
  ];
  callDai(. daiContract)->ignore;

  daiContract->getNonce(account->Option.getWithDefault("0x0"));
};

[@bs.send] external padStart: (string, int, string) => string = "padStart";
let createPermitSig =
    (provider, verifyingContract, nonce, chainId, holder, spender, from) => {
  open Web3;
  open Erc712;

  let domain = {
    name: "Dai Stablecoin",
    version: "1",
    verifyingContract,
    salt: "0x" ++ chainId->BN.toStringRad(16)->padStart(16, "0"),
  };

  let message = {
    "holder": holder,
    "spender": spender,
    "nonce": nonce,
    "expiry": 0,
    // "expiry": deadline,
    "allowed": true,
  };

  let data = {
    "types": {
      "EIP712Domain": eip712Domain,
      "Permit": permit,
    },
    "domain": domain,
    "primaryType": "Permit",
    "message": message,
  };
  let dataString =
    data->Obj.magic->Js.Json.stringifyAny->Option.getWithDefault("");

  let exampleRpcDefinition = {
    method: "eth_signTypedData_v3",
    params: [|from, dataString|],
    from,
  };

  Js.log2("The query", exampleRpcDefinition);

  provider
  ->Web3.sendAsync(exampleRpcDefinition, (. _err, result) => {
      Js.log(result)
    })
  ->ignore;
};
