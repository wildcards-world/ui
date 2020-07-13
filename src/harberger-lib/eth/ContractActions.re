open Globals;

// CODE TO BUY:
let getProviderOrSigner =
    (
      library: RootProviderTypes.web3Library,
      account: option(Web3.ethAddress),
    ) => {
  switch (account) {
  | Some(account) => library.getSigner(. account)
  | None => library
  };
};

type txResult = {
  blockHash: string,
  blockNumber: int,
  byzantium: bool,
  confirmations: int,
  // contractAddress: null,
  // cumulativeGasUsed: Object { _hex: "0x26063", … },
  // events: Array(4) [ {…}, {…}, {…}, … ],
  from: Web3.ethAddress,
  // gasUsed: Object { _hex: "0x26063", … },
  // logs: Array(4) [ {…}, {…}, {…}, … ],
  // logsBloom: "0x00200000000000008000000000000000000020000001000000000000400020000000000000002000000000000000000000000002800010000000008000000000000000000000000000000008000000000040000000000000000000000000000000000000020000014000000000000800024000000000000000000010000000000000000000000000000000000000000000008000000000000000000000000200000008000000000000000000000000000000000800000000000000000000000000001002000000000000000000000000000000000000000020000000040020000000000000000080000000000000000000000000000000080000000000200000"
  status: int,
  _to: Web3.ethAddress,
  transactionHash: string,
  transactionIndex: int,
};
type txError = {
  code: int, // -32000 = always failing tx ;  4001 = Rejected by signer.
  message: string,
  stack: option(string),
};
type txHash = string;
type tx = {
  hash: txHash,
  wait: (. unit) => Promise.Js.t(txResult, txError),
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
  buy:
    (. tokenIdString, parsedUnits, parsedUnits, string, txOptions) =>
    Promise.Js.t(tx, txError),
  depositWei: (. txOptions) => Promise.Js.t(tx, txError),
  withdrawDeposit: (. parsedUnits, txOptions) => Promise.Js.t(tx, txError),
  _collectPatronage:
    (. tokenIdString, txOptions) => Promise.Js.t(tx, txError),
  changePrice:
    (. tokenIdString, parsedUnits, txOptions) => Promise.Js.t(tx, txError),
};
type ethersBnFormat;
[@bs.send] external ethersBnToString: ethersBnFormat => string = "toString";
type voteContract = {
  // vote(uint256 proposalIdToVoteFor, uint256 amount, uint256 sqrt)
  vote: (. string, string, string, txOptions) => Promise.Js.t(tx, txError),
  distributeFunds: (. txOptions) => Promise.Js.t(tx, txError),
  proposalIteration: (. unit) => Js.Promise.t(ethersBnFormat),
  currentWinner: (. unit) => Js.Promise.t(ethersBnFormat),
  proposalDeadline: (. unit) => Js.Promise.t(ethersBnFormat),
  proposalVotes: (. string, string) => Js.Promise.t(ethersBnFormat), // iteration -> proposalId -> num votes
  hasUserVotedForProposalIteration:
    (. string, string, string) => Js.Promise.t(bool), // iteration -> proposalId -> num votes
  totalVotes: (. unit) => Js.Promise.t(ethersBnFormat),
};
type loyaltyTokenContract = {
  // approve(address to, uint256 tokenId)
  allowance:
    (. Web3.ethAddress, Web3.ethAddress) => Js.Promise.t(ethersBnFormat),
  balanceOf: (. Web3.ethAddress) => Js.Promise.t(ethersBnFormat),
  approve:
    (. Web3.ethAddress, string, txOptions) => Promise.Js.t(tx, txError),
};

[@bs.new] [@bs.module "ethers"]
external getContract:
  (Web3.ethAddress, Web3.abi, RootProviderTypes.web3Library) => stewardContract =
  "Contract";

[@bs.new] [@bs.module "ethers"]
external getLoyaltyTokenContract:
  (Web3.ethAddress, Web3.abi, RootProviderTypes.web3Library) =>
  loyaltyTokenContract =
  "Contract";

[@bs.new] [@bs.module "ethers"]
external getVotingContract:
  (Web3.ethAddress, Web3.abi, RootProviderTypes.web3Library) => voteContract =
  "Contract";

[@bs.module "./abi/voteContract.json"]
external voteContract: Web3.abi = "voteContract";

[@bs.module "./abi/loyaltyToken.json"]
external loyaltyTokenAbi: Web3.abi = "loyaltyToken";

[@bs.module "ethers"] [@bs.scope "utils"]
external parseUnits: (. string, int) => parsedUnits = "parseUnits";

let getExchangeContract = (stewardAddress, stewardAbi, library, account) => {
  getContract(
    stewardAddress,
    stewardAbi,
    getProviderOrSigner(library, account),
  );
};
let getLoyaltyTokenContract = (stewardAddress, library, account) => {
  getLoyaltyTokenContract(
    stewardAddress,
    loyaltyTokenAbi,
    getProviderOrSigner(library, account),
  );
};
let getVotingContract = (stewardAddress, library, account) => {
  getVotingContract(
    stewardAddress,
    voteContract,
    getProviderOrSigner(library, account),
  );
};

let stewardAddressMainnet = "0x6D47CF86F6A490c6410fC082Fd1Ad29CF61492d0";
let stewardAddressGoerli = "0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B";
let stewardAddressRinkeby = "0xdC43693cDCdB669C0df9E6a4d9c7a0854640DB4D";

let loyaltyTokenAddressMainnet = "0x773c75c2277eD3e402BDEfd28Ec3b51A3AfbD8a4";
let loyaltyTokenAddressGoerli = "0xd7d8c42ab5b83aa3d4114e5297989dc27bdfb715";
// let loyaltyTokenAddressRinkbey = "0xd7d8c42ab5b83aa3d4114e5297989dc27bdfb715";

let voteContractMainnet = "0x03e051b7e42480Cc9D54F1caB525D2Fea2cF4d83";
let voteContractGoerli = "0x316C5f8867B21923db8A0Bd6890A6BFE0Ab6F9d2";
// let voteContractRinkeby = "0x316C5f8867B21923db8A0Bd6890A6BFE0Ab6F9d2";

let useStewardAbi = () => {
  switch (RootProvider.useStewardAbi()) {
  | Some(abi) => abi
  | None =>
    %raw
    {|require("./abi/steward.json").stewardAbi|}
  };
};

let defaultStewardAddressFromChainId =
  fun
  | 1 => Some(stewardAddressMainnet)
  | 5 => Some(stewardAddressGoerli)
  | _ => None;
let useStewardAddress = () => {
  let externallySetAddress = RootProvider.useStewardContractAddress();
  chainId => {
    externallySetAddress->Belt.Option.mapWithDefault(
      defaultStewardAddressFromChainId(chainId), a =>
      Some(a)
    );
  };
};
let loyaltyTokenAddressFromChainId =
  fun
  | 1 => Some(loyaltyTokenAddressMainnet)
  | 5 => Some(loyaltyTokenAddressGoerli)
  | _ => None;
let voteAddressFromChainId =
  fun
  | 1 => Some(voteContractMainnet)
  | 5 => Some(voteContractGoerli)
  | _ => None;

let useStewardContract = () => {
  let context = RootProvider.useWeb3React();
  let stewardContractAddress = useStewardAddress();
  let stewardAbi = useStewardAbi();

  React.useMemo3(
    () => {
      switch (context.library, context.chainId) {
      | (Some(library), Some(chainId)) =>
        stewardContractAddress(chainId)
        ->oMap(getExchangeContract(_, stewardAbi, library, context.account))

      | _ => None
      }
    },
    (context.library, context.account, context.chainId),
  );
};

let useLoyaltyTokenContract = () => {
  let context = RootProvider.useWeb3React();

  React.useMemo3(
    () => {
      switch (context.library, context.chainId) {
      | (Some(library), Some(chainId)) =>
        chainId
        ->loyaltyTokenAddressFromChainId
        ->oMap(getLoyaltyTokenContract(_, library, context.account))

      | _ => None
      }
    },
    (context.library, context.account, context.chainId),
  );
};

let useVoteContract = () => {
  let context = RootProvider.useWeb3React();

  React.useMemo3(
    () => {
      switch (context.library, context.chainId) {
      | (Some(library), Some(chainId)) =>
        chainId
        ->voteAddressFromChainId
        ->oMap(getVotingContract(_, library, context.account))

      | _ => None
      }
    },
    (context.library, context.account, context.chainId),
  );
};

type transactionState =
  | UnInitialised
  | Created
  | SignedAndSubmitted(txHash)
  // TODO: get the error message when it is declined.
  //      4001 - means the transaction was declined by the signer
  //      -32000 - means the transaction is always failing (exceeds gas allowance)
  | Declined(string)
  | Complete(txResult)
  | Failed;

let useBuy = (animal: TokenId.t) => {
  let animalId = animal->TokenId.toString;
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let optSteward = useStewardContract();

  (
    (newPrice, oldPrice, value: string) => {
      let newPriceEncoded = parseUnits(. newPrice, 18);

      let value = parseUnits(. value, 0);
      let oldPriceParsed = parseUnits(. oldPrice, 0);

      setTxState(_ => Created);
      switch (optSteward) {
      | Some(steward) =>
        let buyPromise =
          steward.buy(.
            animalId,
            newPriceEncoded,
            // oldPrice->Obj.magic,
            oldPriceParsed,
            "50000",
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

let useRedeemLoyaltyTokens = (animalId: string) => {
  let (txState, setTxState) = React.useState(() => UnInitialised);
  let optSteward = useStewardContract();
  let buyFunction = () => {
    let value = parseUnits(. "0", 0);

    setTxState(_ => Created);
    switch (optSteward) {
    | Some(steward) =>
      let claimLoyaltyTokenPromise =
        steward._collectPatronage(.
          animalId,
          {
            // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
            value: value,
          },
        )
        ->Promise.Js.toResult;
      claimLoyaltyTokenPromise->Promise.getOk(tx => {
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
      claimLoyaltyTokenPromise->Promise.getError(error => {
        setTxState(_ => Declined(error.message))
      });
      ();
    | None => ()
    };
  };

  (buyFunction, txState);
};

let useApproveLoyaltyTokens = () => {
  let (txState, setTxState) = React.useState(() => UnInitialised);
  let optLoyaltyTokens = useLoyaltyTokenContract();
  let optNetworkId = RootProvider.useWeb3React().chainId;

  let buyFunction = () => {
    let value = parseUnits(. "0", 0);

    setTxState(_ => Created);

    switch (optLoyaltyTokens, optNetworkId) {
    | (Some(steward), Some(networkId)) =>
      let voteContractAddress =
        networkId->voteAddressFromChainId
        |||| "0x0000000000000000000000000000000000000500";

      let claimLoyaltyTokenPromise =
        steward.approve(.
          voteContractAddress,
          "100000000000000000000000",
          {
            // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
            value: value,
          },
        )
        ->Promise.Js.toResult;
      claimLoyaltyTokenPromise->Promise.getOk(tx => {
        setTxState(_ => SignedAndSubmitted(tx.hash));
        let txMinedPromise = tx.wait(.)->Promise.Js.toResult;
        txMinedPromise->Promise.getOk(txOutcome => {
          setTxState(_ => Complete(txOutcome))
        });
        txMinedPromise->Promise.getError(error => {
          setTxState(_ => Failed);
          Js.log(error);
        });
        ();
      });
      claimLoyaltyTokenPromise->Promise.getError(error => {
        setTxState(_ => Declined(error.message))
      });
      ();
    | _ => ()
    };
  };

  (buyFunction, txState);
};
let useVoteForProject = () => {
  let (txState, setTxState) = React.useState(() => UnInitialised);
  let optSteward = useVoteContract();
  let buyFunction = (proposalId: string, squareRoot: BN.bn) => {
    Js.log("ProposalId" ++ proposalId);
    let value = parseUnits(. "0", 0);

    setTxState(_ => Created);
    switch (optSteward) {
    | Some(steward) =>
      Js.log("!!Voting - start!!");
      Js.log(
        "<Proposal ID>, <loyalty tokens to use>, <number of votes (ie the square root)>",
      );
      Js.log3(
        proposalId,
        squareRoot->BN.sqrGet(.)->BN.toStringGet(.),
        squareRoot->BN.toStringGet(.),
      );
      Js.log("!!Voting - end!!");
      let claimLoyaltyTokenPromise =
        steward.vote(.
          proposalId,
          squareRoot->BN.sqrGet(.)->BN.toStringGet(.),
          squareRoot->BN.toStringGet(.),
          {
            // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
            value: value,
          },
        )
        ->Promise.Js.toResult;
      claimLoyaltyTokenPromise->Promise.getOk(tx => {
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
      claimLoyaltyTokenPromise->Promise.getError(error => {
        setTxState(_ => Declined(error.message))
      });
      ();
    | None => ()
    };
  };

  (buyFunction, txState);
};

let useIncreaseVoteIteration = () => {
  let (txState, setTxState) = React.useState(() => UnInitialised);
  let optSteward = useVoteContract();
  let buyFunction = () => {
    let value = parseUnits(. "0", 0);

    setTxState(_ => Created);
    switch (optSteward) {
    | Some(voteContract) =>
      let claimLoyaltyTokenPromise =
        voteContract.distributeFunds(. {
          // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
          value: value,
        })
        ->Promise.Js.toResult;
      claimLoyaltyTokenPromise->Promise.getOk(tx => {
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
      claimLoyaltyTokenPromise->Promise.getError(error => {
        setTxState(_ => Declined(error.message))
      });
      ();
    | None => ()
    };
  };

  (buyFunction, txState);
};

let useUpdateDeposit = () => {
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let optSteward = useStewardContract();

  (
    (value: string) => {
      let value = parseUnits(. value, 0);

      setTxState(_ => Created);
      switch (optSteward) {
      | Some(steward) =>
        let updateDepositPromise =
          steward.depositWei(. {
            // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
            value: value,
          })
          ->Promise.Js.toResult;
        updateDepositPromise->Promise.getOk(tx => {
          setTxState(_ => SignedAndSubmitted(tx.hash));
          let txMinedPromise = tx.wait(.)->Promise.Js.toResult;
          txMinedPromise->Promise.getOk(txOutcome => {
            setTxState(_ => Complete(txOutcome))
          });
          txMinedPromise->Promise.getError(_error => {
            setTxState(_ => Failed)
          });
          ();
        });
        updateDepositPromise->Promise.getError(error => {
          Js.log("error processing transaction: " ++ error.message)
        });
        ();
      | None => ()
      };
    },
    txState,
  );
};

let useWithdrawDeposit = () => {
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let optSteward = useStewardContract();

  (
    amountToWithdraw => {
      let value = parseUnits(. "0", 0);
      Js.log(amountToWithdraw ++ " is the amount I'm trying to withdraw");
      let amountToWithdrawEncoded = parseUnits(. amountToWithdraw, 0);

      setTxState(_ => Created);
      switch (optSteward) {
      | Some(steward) =>
        let updateDepositPromise =
          steward.withdrawDeposit(.
            amountToWithdrawEncoded,
            {
              // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
              value: value,
            },
          )
          ->Promise.Js.toResult;
        updateDepositPromise->Promise.getOk(tx => {
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
        updateDepositPromise->Promise.getError(error => {
          setTxState(_ => Declined(error.message))
        });
        ();
      | None => ()
      };
    },
    txState,
  );
};

let useUserLoyaltyTokenBalance = (address: Web3.ethAddress) => {
  let (result, setResult) = React.useState(() => None);
  let (counter, setCounter) = React.useState(() => 0);

  let optSteward = useLoyaltyTokenContract();

  React.useEffect4(
    () => {
      switch (optSteward) {
      | Some(steward) =>
        let _ = {
          let%Async balance = steward.balanceOf(. address);
          let balanceString = balance->ethersBnToString;
          setResult(_ => Some(BN.new_(balanceString)));
          ()->async;
        };
        ();
      | None => ()
      };
      None;
    },
    (counter, setResult, optSteward, address),
  );

  (result, () => setCounter(_ => counter + 1));
};

let useVoteApprovedTokens = (owner: Web3.ethAddress) => {
  let (result, setResult) = React.useState(() => None);
  let (counter, setCounter) = React.useState(() => 0);

  let optLoyaltyTokens = useLoyaltyTokenContract();
  let optNetworkId = RootProvider.useWeb3React().chainId;

  React.useEffect5(
    () => {
      switch (optLoyaltyTokens, optNetworkId) {
      | (Some(loyaltyToken), Some(networkId)) =>
        let _ = {
          let voteContractAddress =
            networkId->voteAddressFromChainId
            |||| "0x0000000000000000000000000000000000000500";
          // Js.log3("getting the allowance", owner, voteContractAddress);
          let%Async allowance =
            loyaltyToken.allowance(. owner, voteContractAddress);
          let allowanceString = allowance->ethersBnToString;
          setResult(_ => Some(BN.new_(allowanceString)));
          ()->async;
        };
        ();
      | _ => ()
      };
      None;
    },
    (counter, setResult, optLoyaltyTokens, owner, optNetworkId),
  );

  (result, () => setCounter(_ => counter + 1));
};

let useCurrentIteration = () => {
  let (result, setResult) = React.useState(() => None);
  let (counter, setCounter) = React.useState(() => 0);

  let optVoteContract = useVoteContract();

  React.useEffect3(
    () => {
      switch (optVoteContract) {
      | Some(voteContract) =>
        let _ = {
          let%Async currentIteration = voteContract.proposalIteration(.);
          let currentIterationString = currentIteration->ethersBnToString;
          setResult(_ => currentIterationString->Int.fromString);
          ()->async;
        };
        ();
      | None => ()
      };
      None;
    },
    (counter, setResult, optVoteContract),
  );

  (result, () => setCounter(_ => counter + 1));
};

let useCurrentWinner = () => {
  let (result, setResult) = React.useState(() => None);
  let (counter, setCounter) = React.useState(() => 0);

  let optVoteContract = useVoteContract();

  React.useEffect3(
    () => {
      switch (optVoteContract) {
      | Some(voteContract) =>
        let _ = {
          let%Async currentWinnerBn = voteContract.currentWinner(.);
          let currentWinnerString = currentWinnerBn->ethersBnToString;
          setResult(_ => currentWinnerString->Int.fromString);
          ()->async;
        };
        ();
      | None => ()
      };
      None;
    },
    (counter, setResult, optVoteContract),
  );

  (result, () => setCounter(_ => counter + 1));
};

let useProposalVotes = (iteration, projectId) => {
  let (result, setResult) = React.useState(() => None);
  let (counter, setCounter) = React.useState(() => 0);

  let optVoteContract = useVoteContract();

  React.useEffect5(
    () => {
      switch (optVoteContract) {
      | Some(voteContract) =>
        let _ = {
          let%Async proposalVotes =
            voteContract.proposalVotes(. iteration, projectId);
          let propsalVotesString = proposalVotes->ethersBnToString;
          setResult(_ => Some(propsalVotesString->BN.new_));
          ()->async;
        };
        ();
      | None => ()
      };
      None;
    },
    (counter, setResult, optVoteContract, iteration, projectId),
  );

  (result, () => setCounter(_ => counter + 1));
};

let useHasUserVotedForProposalIteration = (iteration, userAddress, projectId) => {
  let (result, setResult) = React.useState(() => None);
  let (counter, setCounter) = React.useState(() => 0);

  let optVoteContract = useVoteContract();

  React.useEffect6(
    () => {
      switch (optVoteContract) {
      | Some(voteContract) =>
        let _ = {
          let%Async hasVotedForProposal =
            voteContract.hasUserVotedForProposalIteration(.
              iteration,
              userAddress,
              projectId,
            );
          setResult(_ => Some(hasVotedForProposal));
          ()->async;
        };
        ();
      | None => ()
      };
      None;
    },
    (counter, setResult, optVoteContract, iteration, projectId, userAddress),
  );

  (result, () => setCounter(_ => counter + 1));
};

// totalVotes: unit => Js.Promise.t(ethersBnFormat),
let useTotalVotes = () => {
  let (result, setResult) = React.useState(() => None);
  let (counter, setCounter) = React.useState(() => 0);

  let optVoteContract = useVoteContract();

  React.useEffect3(
    () => {
      switch (optVoteContract) {
      | Some(voteContract) =>
        let _ = {
          let%Async totalVotes = voteContract.totalVotes(.);
          let totalVotesString = totalVotes->ethersBnToString;
          setResult(_ => Some(totalVotesString->BN.new_));
          ()->async;
        };
        ();
      | None => ()
      };
      None;
    },
    (counter, setResult, optVoteContract),
  );

  (result, () => setCounter(_ => counter + 1));
};

let useProposalDeadline = () => {
  let (result, setResult) = React.useState(() => None);
  let (counter, setCounter) = React.useState(() => 0);

  let optSteward = useVoteContract();

  React.useEffect3(
    () => {
      switch (optSteward) {
      | Some(steward) =>
        let _ = {
          let%Async currentIteration = steward.proposalDeadline(.);
          let currentIterationString = currentIteration->ethersBnToString;
          setResult(_ => currentIterationString->Int.fromString);
          ()->async;
        };
        ();
      | None => ()
      };
      None;
    },
    (counter, setResult, optSteward),
  );

  (result, () => setCounter(_ => counter + 1));
};

let useChangePrice = animal => {
  let animalId = TokenId.toString(animal);
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let optSteward = useStewardContract();

  (
    newPrice => {
      let value = parseUnits(. "0", 0);
      let newPriceEncoded = parseUnits(. newPrice, 0);

      setTxState(_ => Created);
      switch (optSteward) {
      | Some(steward) =>
        let updatePricePromise =
          steward.changePrice(.
            animalId,
            newPriceEncoded,
            {
              // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
              value: value,
            },
          )
          ->Promise.Js.toResult;
        updatePricePromise->Promise.getOk(tx => {
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
        updatePricePromise->Promise.getError(error => {
          setTxState(_ => Declined(error.message))
        });
        ();
      | None => ()
      };
    },
    txState,
  );
};
