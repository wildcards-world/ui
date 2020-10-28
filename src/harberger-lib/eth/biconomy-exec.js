import React from "react";
import {
  Web3ReactProvider,
  useWeb3React,
  UnsupportedChainIdError,
} from "@web3-react/core";
import jsonInterface from "./WildcardSteward_v3_matic.json";
import Web3 from "web3";
import { toBuffer } from "ethereumjs-util";
import abi from "ethereumjs-abi";
import events from "events";
const { signDaiPermit } = require("eth-permit");

// import Biconomy from "@biconomy/mexa";

export const useSetupBuyFunction = (tokenId, stewardContractAddress, daiContractAddress, networkId) => {
  console.log("1")
  const context = useWeb3React();
  console.log("2")
  const { library, account } = context;
  
  console.log("3");
  if (!library) {
    // TODO: FIX THIS CODE IS BAD AND ERROR PRONE!
    console.log("Library not defined");
    return;
  }
  
  const web3 = new Web3(library.provider);
  console.log("4");
  // const contractAddress = "0x59b3c176c39bd8734717492f4da8fe26ff6a454d";
  
  const contract = new web3.eth.Contract(jsonInterface.abi, stewardContractAddress);
  console.log("5");
  
  return async (
    newPrice,
    currentPriceWei,
    wildcardsPercentage,
    amountToSend
    ) => {
      console.log("6");
      console.log(library.provider,// Mainnet - for signing
        daiContractAddress,
        account,
        stewardContractAddress,
        Number.MAX_SAFE_INTEGER);
      const { nonce, expiry, v, r, s } = await signDaiPermit(
        // library.provider,
        window.ethereum,
        "0x0099f841a6ab9a082828fac66134fd25c9d8a195",
        account,
        "0x89e2d4628435368a7CD72611E769dDe27802b95e",
        );
      // const { nonce, expiry, v, r, s } = await signDaiPermit(
      //   // library.provider,// Mainnet - for signing
      //   window.ethereum,
      //   // library.provider,// matic - for getting dai info
      //   daiContractAddress.toLowerCase(),
      //   account.toLowerCase(),
      //   stewardContractAddress,
      //   Number.MAX_SAFE_INTEGER// TODO: put a reasonable number here!!
      //   );
        
        // console.log("7 ---!");
        // const functionSignature = contract.methods
        // .buyAuctionWithPermit(
        //   // uint256 nonce,
        //   nonce,
        //   // uint256 expiry,
        //   expiry,
        //   // bool allowed,
        //   true,
        //   // uint8 v,
        //   v,
        //   // bytes32 r,
        //   r,
        //   // bytes32 s,
        //   s,
        //   // uint256 tokenId,
        //   tokenId,
        //   // uint256 _newPrice,
        //   newPrice,
        //   // uint256 previousPrice,
        //   currentPriceWei,
        //   // uint256 serviceProviderPercentage,
        //   wildcardsPercentage,
        //   // uint256 depositAmount
        //   amountToSend
        //   )
        //   .encodeABI();
        //   console.log("8");
          
    //       const result = await executeMetaTransaciton(
    //         account,
    //         functionSignature,
    //         contract,
    //         stewardContractAddress,
    //         networkId,
    //         web3
    //         );
    //         console.log("9");

    // result
    //   .on("transactionHash", (hash) => {
    //     // On transacion Hash
    //     console.log("hash", { hash });
    //   })
    //   .once("confirmation", (confirmation, recipet) => {
    //     console.log("confirmation", { confirmation, recipet });
    //     // On Confirmation
    //   })
    //   .on("error", (error) => {
    //     // On Error
    //   });
  };
};

// function buyAuctionFunction(newPrice, wildcardsPercentage, amountToSend) {
//   console.log(newPrice, wildcardsPercentage, amountToSend);
//   return Promise.resolve("Some Auction tx hash");
// }

// var Contract = require("web3-eth-contract");
/******
 * HELPER CODE
 */

const constructMetaTransactionMessage = (
  nonce,
  chainId,
  functionSignature,
  contractAddress
) => {
  return abi.soliditySHA3(
    ["uint256", "address", "uint256", "bytes"],
    [nonce, contractAddress, chainId, toBuffer(functionSignature)]
  );
};

const getSignatureParameters = (signature, web3) => {
  if (!web3.utils.isHexStrict(signature)) {
    throw new Error(
      'Given value "'.concat(signature, '" is not a valid hex string.')
    );
  }
  var r = signature.slice(0, 66);
  var s = "0x".concat(signature.slice(66, 130));
  var v = "0x".concat(signature.slice(130, 132));
  v = web3.utils.hexToNumber(v);
  if (![27, 28].includes(v)) v += 27;
  return {
    r: r,
    s: s,
    v: v,
  };
};

const executeMetaTransaciton = async (
  userAddress,
  functionSignature,
  contract,
  contractAddress,
  chainId,
  web3
) => {
  var eventEmitter = new events.EventEmitter();
  if (
    (contract && userAddress && functionSignature, chainId, contractAddress)
  ) {
    // let nonce = 0;
    let nonce = await contract.methods.getNonce(userAddress).call();
    // console.log("The nonce is:", nonce);
    let messageToSign = constructMetaTransactionMessage(
      nonce,
      chainId,
      functionSignature,
      contractAddress
    );

    // console.log("web3", web3);
    // console.log("web3.eth", web3.eth);
    // console.log("web3.eth.personal", web3.eth.personal);

    // const signature = await web3.eth.sign(
    const signature = await web3.eth.personal.sign(
      "0x" + messageToSign.toString("hex"),
      userAddress
    );

    // console.info(`User signature is ${signature}`);
    let { r, s, v } = getSignatureParameters(signature, web3);

    // try {
    //   fetch(`https://api.biconomy.io/api/v2/meta-tx/native`, {
    //     method: "POST",
    //     headers: {
    //       "x-api-key": "IUNMuYhZ7.9c178f07-e191-4877-b995-ef4b61ed956f",
    //       "Content-Type": "application/json;charset=utf-8",
    //     },
    //     body: JSON.stringify({
    //       to: contractAddress,
    //       apiId: "e0dd72a6-78e0-44f8-b56e-902c1b519ffa",
    //       params: [userAddress, functionSignature, r, s, v],
    //       // params: [userAddress, functionData, r, s, v],
    //       from: userAddress,
    //     }),
    //   })
    //     .then((response) => response.json())
    //     .then(function (result) {
    //       console.log(result);
    //       console.log(`Transaction sent by relayer with hash ${result.txHash}`);
    //     })
    //     .catch(function (error) {
    //       console.log(error);
    //     });
    // } catch (error) {
    //   console.log(error);
    // }

    // No need to calculate gas limit or gas price here
    let transactionListener = contract.methods
      .executeMetaTransaction(userAddress, functionSignature, r, s, v)
      .send({
        from: userAddress,
      });

    transactionListener
      .on("transactionHash", (hash) => {
        eventEmitter.emit("transactionHash", hash);
      })
      .once("confirmation", (confirmation, recipet) => {
        eventEmitter.emit("confirmation", confirmation, recipet);
      })
      .on("error", (error) => {
        eventEmitter.emit("error", error);
      });

    return eventEmitter;
  } else {
    console.log(
      "All params userAddress, functionSignature, chainId, contract address and contract object are mandatory"
    );
  }
};
/******
 * HELPER CODE -end
 */

export const execTestTx = async (library, account) => {
  // const context = useWeb3React();
  // const {
  //   // connector,
  //   library,
  //   chainId,
  //   account,
  //   activate,
  //   deactivate,
  //   active,
  //   error,
  // } = context;

  if (!library) return;

  const web3 = new Web3(library.provider);
  const contractAddress = "0x59b3c176c39bd8734717492f4da8fe26ff6a454d";

  // const biconomy = new Biconomy(library.provider, {
  //   apiKey: "IUNMuYhZ7.9c178f07-e191-4877-b995-ef4b61ed956f",
  //   debug: true,
  // });

  // const web3Biconomy = new Web3(biconomy);
  var contract = new web3.eth.Contract(jsonInterface.abi, contractAddress);

  let functionSignature = contract.methods
    .testFunctionThatDoesNothing(account)
    .encodeABI();

  // let result = contract.methods.testFunctionThatDoesNothing(account).send({
  //   from: account,
  // });
  let result = await executeMetaTransaciton(
    account,
    functionSignature,
    contract,
    contractAddress,
    "4",
    web3
  );

  // result
  //   .on("transactionHash", (hash) => {
  //     // On transacion Hash
  //     console.log("hash", { hash });
  //   })
  //   .once("confirmation", (confirmation, recipet) => {
  //     console.log("confirmation", { confirmation, recipet });
  //     // On Confirmation
  //   })
  //   .on("error", (error) => {
  //     // On Error
  //   });
};
