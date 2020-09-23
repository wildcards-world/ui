open Globals;

[@bs.module "./wrapSignerInGsnProvider"]
external wrapSignerInGsnProvider:
  (. RootProviderTypes.rawProvider, RootProviderTypes.web3Library) =>
  RootProviderTypes.web3Library =
  "default";
// (. RootProviderTypes.web3Library) => RootProviderTypes.web3Library =

// CODE TO BUY:
let getProviderOrSigner =
    (
      library: RootProviderTypes.web3Library,
      account: option(Web3.ethAddress),
      isGsn: bool,
    ) => {
  switch (account) {
  | Some(account) =>
    if (isGsn) {
      wrapSignerInGsnProvider(.
        library.provider,
        library.getSigner(. account),
        // TODO: wrap this in the gsn stuff
        // library.getSigner(.
        //   account,
        // );
      );
    } else {
      library.getSigner(. account);
    }
  | None => library
  };
};

// type txResult = {
//   blockHash: string,
//   blockNumber: int,
//   byzantium: bool,
//   confirmations: int,
//   // contractAddress: null,
//   // cumulativeGasUsed: Object { _hex: "0x26063", … },
//   // events: Array(4) [ {…}, {…}, {…}, … ],
//   from: Web3.ethAddress,
//   // gasUsed: Object { _hex: "0x26063", … },
//   // logs: Array(4) [ {…}, {…}, {…}, … ],
//   // logsBloom: "0x00200000000000008000000000000000000020000001000000000000400020000000000000002000000000000000000000000002800010000000008000000000000000000000000000000008000000000040000000000000000000000000000000000000020000014000000000000800024000000000000000000010000000000000000000000000000000000000000000008000000000000000000000000200000008000000000000000000000000000000000800000000000000000000000000001002000000000000000000000000000000000000000020000000040020000000000000000080000000000000000000000000000000080000000000200000"
//   status: int,
//   _to: Web3.ethAddress,
//   transactionHash: string,
//   transactionIndex: int,
// };
type txError = {
  code: int, // -32000 = always failing tx ;  4001 = Rejected by signer.
  message: string,
  stack: option(string),
};
type txHash = string;
type tx = {
  hash: txHash,
  wait:
    (. unit) =>
    Promise.Js.t(ContractActions.txResult, ContractActions.txError),
};
type parsedUnits;
type txOptions = {value: parsedUnits};
type tokenIdString = string;
type estimateBuy = {
  buy:
    // (. string, parsedUnits, txOptions) =>
    (. string, parsedUnits, parsedUnits, txOptions) =>
    Promise.Js.t(string, string),
};
type stewardContract = {
  estimate: estimateBuy,
  // mint(address to, uint256 amount):
  mint: (. string, parsedUnits, txOptions) => Promise.Js.t(tx, txError),
  buy:
    (. tokenIdString, parsedUnits, parsedUnits, string, txOptions) =>
    Promise.Js.t(tx, txError),
  buyAuction:
    (. tokenIdString, parsedUnits, string, txOptions) =>
    Promise.Js.t(tx, txError),
  depositWei: (. txOptions) => Promise.Js.t(tx, txError),
  withdrawDeposit: (. parsedUnits, txOptions) => Promise.Js.t(tx, txError),
  _collectPatronage:
    (. tokenIdString, txOptions) => Promise.Js.t(tx, txError),
  changePrice:
    (. tokenIdString, parsedUnits, txOptions) => Promise.Js.t(tx, txError),
};
[@bs.send]
external buyOld:
  (stewardContract, tokenIdString, parsedUnits, txOptions) =>
  Promise.Js.t(tx, txError) =
  "buy";

type ethersBnFormat;
[@bs.send] external ethersBnToString: ethersBnFormat => string = "toString";

[@bs.new] [@bs.module "ethers"]
external getContract:
  (Web3.ethAddress, Web3.abi, RootProviderTypes.web3Library) => stewardContract =
  "Contract";

[@bs.module "./abi/voteContract.json"]
external voteContract: Web3.abi = "voteContract";

[@bs.module "ethers"] [@bs.scope "utils"]
external parseUnits: (. string, int) => parsedUnits = "parseUnits";

let getExchangeContract =
    (stewardAddress, stewardAbi, library, account, isGsn) => {
  getContract(
    stewardAddress,
    stewardAbi,
    getProviderOrSigner(library, account, isGsn),
  );
};

let erc20 = "0xc10B962351DCebEee99BFb6219D75aEDe2872839";

let useStewardAbi = () => {
  // switch (RootProvider.useStewardAbi()) {
  // // | Some(abi) => abi
  // | None =>

  %raw
  {|require("./gsn-test-erc20.json")|};
  // };
};

let defaultStewardAddressFromChainId =
  fun
  | _ => Some(erc20);
// | _ => None;
let useStewardAddress = () => {
  let externallySetAddress = RootProvider.useStewardContractAddress();
  chainId => {
    externallySetAddress->Belt.Option.mapWithDefault(
      defaultStewardAddressFromChainId(chainId), a =>
      Some(a)
    );
  };
};

let useStewardContract = isGsn => {
  let context = RootProvider.useWeb3React();
  // let context = RootProvider.useWeb3ReactId("matic");
  let stewardContractAddress = useStewardAddress();
  let stewardAbi = useStewardAbi();

  React.useMemo3(
    () => {
      switch (context.library, context.chainId) {
      | (Some(library), Some(chainId)) =>
        stewardContractAddress(chainId)
        ->oMap(
            getExchangeContract(
              _,
              stewardAbi,
              library,
              context.account,
              isGsn,
            ),
          )

      | _ => None
      }
    },
    (context.library, context.account, context.chainId),
  );
};

// type transactionState =
//   | UnInitialised
//   | Created
//   | SignedAndSubmitted(txHash)
//   // TODO: get the error message when it is declined.
//   //      4001 - means the transaction was declined by the signer
//   //      -32000 - means the transaction is always failing (exceeds gas allowance)
//   | Declined(string)
//   | Complete(txResult)
//   | Failed;

let useMint = isGsn => {
  let (txState, setTxState) =
    React.useState(() => ContractActions.UnInitialised);

  let optSteward = useStewardContract(isGsn);

  (
    () => {
      let amount = parseUnits(. "1", 18);
      let value = parseUnits(. "0", 18);
      Js.log("This is something");

      setTxState(_ => Created);
      switch (optSteward) {
      | Some(steward) =>
        let buyPromise =
          steward.mint(.
            "0x8c7A88756EbbF46Ede65E4D678359cAC5f08f7b2",
            amount,
            {
              // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
              value: value,
            },
          )
          ->Promise.Js.toResult;
        buyPromise->Promise.getOk(tx => {
          setTxState(_ => SignedAndSubmitted(tx.hash));
          let txMinedPromise = tx.wait(.)->Promise.Js.toResult;
          txMinedPromise->Promise.getOk(txOutcome => {
            Js.log(txOutcome);
            setTxState(_ => Complete(txOutcome));
          });
          txMinedPromise->Promise.getError(error => {
            setTxState(_ => Failed);
            Js.log(error);
          });
          ();
        });
        buyPromise->Promise.getError(error => {
          setTxState(_ => Declined(error.message))
        });
        ();
      | None => ()
      };
    },
    txState,
  );
};

// let useRedeemLoyaltyTokens = (animalId: string, isGsn) => {
//   let (txState, setTxState) = React.useState(() => UnInitialised);
//   let optSteward = useStewardContract(isGsn);
//   let buyFunction = () => {
//     let value = parseUnits(. "0", 0);

//     setTxState(_ => Created);
//     switch (optSteward) {
//     | Some(steward) =>
//       let claimLoyaltyTokenPromise =
//         steward._collectPatronage(.
//           animalId,
//           {
//             // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
//             value: value,
//           },
//         )
//         ->Promise.Js.toResult;
//       claimLoyaltyTokenPromise->Promise.getOk(tx => {
//         setTxState(_ => SignedAndSubmitted(tx.hash));
//         let txMinedPromise = tx.wait(.)->Promise.Js.toResult;
//         txMinedPromise->Promise.getOk(txOutcome => {
//           Js.log(txOutcome);
//           setTxState(_ => Complete(txOutcome));
//         });
//         txMinedPromise->Promise.getError(error => {
//           setTxState(_ => Failed);
//           Js.log(error);
//         });
//         ();
//       });
//       claimLoyaltyTokenPromise->Promise.getError(error => {
//         setTxState(_ => Declined(error.message))
//       });
//       ();
//     | None => ()
//     };
//   };

//   (buyFunction, txState);
// };
