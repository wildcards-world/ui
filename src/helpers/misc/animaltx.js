import { Interface } from "@ethersproject/abi";
import { Contract } from "@ethersproject/contracts";

const STEWARD_ABI = [
  // "function buy(uint256 tokenId, uint256 newPrice) public payable"
  {
    constant: false,
    inputs: [
      {
        name: "tokenId",
        type: "uint256"
      },
      {
        name: "_newPrice",
        type: "uint256"
      }
    ],
    name: "buy",
    outputs: [],
    payable: true,
    stateMutability: "payable",
    type: "function"
  }
];

// mainnet: 0x6D47CF86F6A490c6410fC082Fd1Ad29CF61492d0
// goerli: 0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B

export default (tokenId, price, txObject, signer) => {
  let stewardInterface = new Interface(STEWARD_ABI);
  let stewardContract = new Contract(
    stewardInterface,
    "0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B",
    signer
  );

  // var calldata = stewardInterface.functions["buy(uint256,uint256)"].encode([
  //   "0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B",
  //   tokenId,
  //   price
  // ]);
  console.log(
    "we have the function executing",
    tokenId,
    price,
    // txObject,
    stewardContract
    // stewardInterface.functions,
    // stewardInterface.functions["buy(uint256,uint256)"]
    // calldata
  );
};
