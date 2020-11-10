import abi from "ethereumjs-abi";
import { toBuffer } from "ethereumjs-util";

export const constructMetaTransactionMessage = (
  nonce,
  chainId,
  functionSignature,
  contractAddress
) => {
  return (
    "0x" +
    abi
      .soliditySHA3(
        ["uint256", "address", "uint256", "bytes"],
        [nonce, contractAddress, chainId, toBuffer(functionSignature)]
      )
      .toString("hex")
  );
};
