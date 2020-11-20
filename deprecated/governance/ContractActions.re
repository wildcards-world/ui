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

[@bs.new] [@bs.module "ethers"]
external getVotingContract:
  (Web3.ethAddress, Web3.abi, Web3.web3Library) => voteContract =
  "Contract";

let getVotingContract = (stewardAddress, library, account) => {
  getVotingContract(
    stewardAddress,
    voteContract,
    ContractUtil.getProviderOrSigner(library, account),
  );
};

let voteContractMainnet = "0x03e051b7e42480Cc9D54F1caB525D2Fea2cF4d83";
let voteContractGoerli = "0x316C5f8867B21923db8A0Bd6890A6BFE0Ab6F9d2";
// let voteContractRinkeby = "0x316C5f8867B21923db8A0Bd6890A6BFE0Ab6F9d2";

let voteAddressFromChainId =
  fun
  | 1 => Some(voteContractMainnet)
  | 5 => Some(voteContractGoerli)
  | _ => None;

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

[@bs.module "./abi/voteContract.json"]
external voteContract: Web3.abi = "voteContract";

let useApproveLoyaltyTokens = () => {
  let (txState, setTxState) = React.useState(() => UnInitialised);
  let optLoyaltyTokens = useLoyaltyTokenContract();
  let optNetworkId = RootProvider.useWeb3React().chainId;

  let buyFunction = () => {
    let value = parseUnits(. "0", 0);

    setTxState(_ => Created);

    switch (optLoyaltyTokens, optNetworkId) {
    | (Some(loyaltyTokenCotract), Some(networkId)) =>
      let voteContractAddress =
        networkId->voteAddressFromChainId |||| CONSTANTS.nullEthAddress;

      let claimLoyaltyTokenPromise =
        loyaltyTokenCotract.approve(.
          voteContractAddress,
          "100000000000000000000000",
          {
            gasLimit: "500302", //calculateGasMargin(estimatedGasLimit, GAS_MARGIN),

            value,
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
  let buyFunction = (proposalId: string, squareRoot: BN.t) => {
    Js.log("ProposalId" ++ proposalId);
    let value = parseUnits(. "0", 0);

    setTxState(_ => Created);
    switch (optSteward) {
    | Some(steward) =>
      let claimLoyaltyTokenPromise =
        steward.vote(.
          proposalId,
          squareRoot->BN.sqr->BN.toString,
          squareRoot->BN.toString,
          {
            gasLimit: "500302", //calculateGasMargin(estimatedGasLimit, GAS_MARGIN),
            value,
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
          gasLimit: "500302", //calculateGasMargin(estimatedGasLimit, GAS_MARGIN),

          value,
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
            networkId->voteAddressFromChainId |||| CONSTANTS.nullEthAddress;
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
