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

type ethSig = {
  r: string,
  s: string,
  v: int,
};
[@bs.send] external slice: (string, int, int) => string = "slice";
[@bs.val] external parseInt: (string, int) => int = "parseInt";

let getEthSig = sigString => {
  {
    r: sigString->slice(0, 66),
    s: "0x" ++ sigString->slice(66, 130),
    v: parseInt(sigString->slice(130, 132), 16),
  };
};

[@bs.send] external padStart: (string, int, string) => string = "padStart";
let createPermitSig =
    (provider, verifyingContract, nonce, chainId, holder, spender, from) => {
  open Web3;
  open Erc712;

  Js.log2("Chain Id being used:", chainId);
  Js.log2(
    "Chain Id being used:",
    "0x" ++ chainId->BN.toStringRad(16)->padStart(16, "0"),
  );

  let domain = {
    name: "(PoS) Dai Stablecoin",
    // name: "Dai Stablecoin",
    version: "1",
    verifyingContract,
    salt: "0x" ++ chainId->BN.toStringRad(16)->padStart(64, "0"),
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
  Js.log2("data1", dataString);

  let getData: unit => Js.t(string) = [%raw
    {|() =>({
    types: {
      EIP712Domain: [
        {
          name: "name",
          type: "string",
        },
        {
          name: "version",
          type: "string",
        },
        {
          name: "verifyingContract",
          type: "address",
        },
        {
          name: "salt",
          type: "bytes32",
        },
      ],
      Permit: [
        {
          name: "holder",
          type: "address",
        },
        {
          name: "spender",
          type: "address",
        },
        {
          name: "nonce",
          type: "uint256",
        },
        {
          name: "expiry",
          type: "uint256",
        },
        {
          name: "allowed",
          type: "bool",
        },
      ],
    },
    domain: {
      name: "(PoS) Dai Stablecoin",
      version: "1",
      verifyingContract: "0xea9d8a947dD7eBa9cF883c4aa71f18aD5A9c06bB",
      salt:
        "0x0000000000000000000000000000000000000000000000000000000000000005",
    },
    primaryType: "Permit",
    message: {
      holder: "0xd3Cbce59318B2E570883719c8165F9390A12BdD6",
      spender: "0xf02Bb5b595Af96597b82f39F5de265E77Dc75CbC",
      nonce: "0",
      expiry: "0",
      allowed: true,
    },
  })|}
  ];

  let data =
    getData()->Obj.magic->Js.Json.stringifyAny->Option.getWithDefault("");
  Js.log2("data2", data);
  let exampleRpcDefinition = {
    // method: "eth_signTypedData",
    method: "eth_signTypedData_v3",
    // params: [|from, data|],
    params: [|from, dataString|],
    from,
  };

  Js.log2("The query", exampleRpcDefinition);

  Js.Promise.make((~resolve, ~reject) =>
    provider
    ->Web3.sendAsync(exampleRpcDefinition, (. err, result) => {
        switch (err->Js.Nullable.toOption) {
        | Some(err) =>
          Js.log2("There was an error", err);
          reject(. err->Obj.magic);
        | None =>
          Js.log(result);
          let sigString = result.result->Obj.magic;
          Js.log(getEthSig(sigString));

          resolve(. getEthSig(sigString));
        }
      })
    ->ignore
  );
};

[@bs.module "./biconomy-exec"]
//   library,
external execTestTx:
  (
    . option(Web3.web3Library),
    //   account,
    option(Web3.ethAddress),
    //   stewardContractAddress
    Web3.ethAddress,
    //   nonce,
    string,
    //   expiry,
    string,
    //   allowed,
    bool,
    //   v,
    int,
    //   r,
    string,
    //   s,
    string,
    //   tokenId,
    string,
    //   _newPrice,
    string,
    //   previousPrice,
    string,
    //   serviceProviderPercentage,
    string,
    //   depositAmount,
    string
  ) =>
  Js.Promise.t(unit) =
  "execTestTx";

// (
// );
