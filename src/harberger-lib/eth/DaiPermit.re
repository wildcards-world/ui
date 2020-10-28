open Globals;

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
