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

type abi;
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
  buy: (. string, parsedUnits, txOptions) => Promise.Js.t(string, string),
};
type stewardContract = {
  estimate: estimateBuy,
  buy: (. tokenIdString, parsedUnits, txOptions) => Promise.Js.t(tx, txError),
  depositWei: (. txOptions) => Promise.Js.t(tx, txError),
  withdrawDeposit: (. parsedUnits, txOptions) => Promise.Js.t(tx, txError),
  _collectPatronage:
    (. tokenIdString, txOptions) => Promise.Js.t(tx, txError),
  changePrice:
    (. tokenIdString, parsedUnits, txOptions) => Promise.Js.t(tx, txError),
};
type voteContract = {
  // vote(uint256 proposalIdToVoteFor, uint256 amount, uint256 sqrt)
  vote: (. string, string, string) => Promise.Js.t(tx, txError),
};
type loyaltyTokenContract = {
  // approve(address to, uint256 tokenId)
  approve:
    (. Web3.ethAddress, string, txOptions) => Promise.Js.t(tx, txError),
};

[@bs.new] [@bs.module "ethers"]
external getContract:
  (Web3.ethAddress, abi, RootProviderTypes.web3Library) => stewardContract =
  "Contract";

[@bs.new] [@bs.module "ethers"]
external getLoyaltyTokenContract:
  (Web3.ethAddress, abi, RootProviderTypes.web3Library) => loyaltyTokenContract =
  "Contract";

[@bs.new] [@bs.module "ethers"]
external getVotingContract:
  (Web3.ethAddress, abi, RootProviderTypes.web3Library) => voteContract =
  "Contract";

[@bs.module "../../contracts/abi/steward.json"]
external stewardAbi: abi = "stewardAbi";

[@bs.module "../../contracts/abi/voteContract.json"]
external voteContract: abi = "voteContract";

[@bs.module "../../contracts/abi/loyaltyToken.json"]
external loyaltyTokenAbi: abi = "loyaltyToken";

[@bs.module "ethers"] [@bs.scope "utils"]
external parseUnits: (. string, int) => parsedUnits = "parseUnits";

let getExchangeContract = (stewardAddress, library, account) => {
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

let loyaltyTokenAddressGoerli = "0xd7d8c42ab5b83aa3d4114e5297989dc27bdfb715";
let voteContractGoerli = "0xE14EBa08D16dEB1639D3ae23a5E8e779d5BA83DE";

let useStewardContract = () => {
  let context = RootProvider.useWeb3React();

  let optNetworkId = context.chainId;

  React.useMemo3(
    () => {
      switch (context.library) {
      | Some(library) =>
        switch (optNetworkId) {
        | Some(networkId) =>
          switch (networkId) {
          | 1 =>
            Some(
              getExchangeContract(
                "0x6D47CF86F6A490c6410fC082Fd1Ad29CF61492d0",
                library,
                context.account,
              ),
            )
          | 5 =>
            Some(
              getExchangeContract(
                "0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B",
                library,
                context.account,
              ),
            )
          | _ => None
          }
        | None => None
        }
      | None => None
      }
    },
    (context.library, context.account, optNetworkId),
  );
};

let useLoyaltyTokenContract = () => {
  let context = RootProvider.useWeb3React();

  let optNetworkId = context.chainId;

  React.useMemo3(
    () => {
      switch (context.library) {
      | Some(library) =>
        switch (optNetworkId) {
        | Some(networkId) =>
          switch (networkId) {
          | 5 =>
            Some(
              getLoyaltyTokenContract(
                loyaltyTokenAddressGoerli,
                library,
                context.account,
              ),
            )
          | _ => None
          }
        | None => None
        }
      | None => None
      }
    },
    (context.library, context.account, optNetworkId),
  );
};

let useVoteContract = () => {
  let context = RootProvider.useWeb3React();

  let optNetworkId = context.chainId;

  React.useMemo3(
    () => {
      switch (context.library) {
      | Some(library) =>
        switch (optNetworkId) {
        | Some(networkId) =>
          switch (networkId) {
          | 5 =>
            Some(
              getVotingContract(voteContractGoerli, library, context.account),
            )
          | _ => None
          }
        | None => None
        }
      | None => None
      }
    },
    (context.library, context.account, optNetworkId),
  );
};

type transactionState =
  | UnInitialised
  | Created
  | SignedAndSubmitted(txHash)
  | Declined
  | Complete(txResult)
  | Failed;

let useBuy = animal => {
  let animalId = Animal.getId(animal);
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let optSteward = useStewardContract();

  (
    (newPrice, value: string) => {
      let newPriceEncoded = parseUnits(. newPrice, 18);
      let value = parseUnits(. value, 0);

      setTxState(_ => Created);
      switch (optSteward) {
      | Some(steward) =>
        let buyPromise =
          steward.buy(.
            animalId,
            newPriceEncoded,
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
        buyPromise->Promise.getError(error => {Js.log(error.message)});
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
        Js.log(error.message)
      });
      ();
    | None => ()
    };
  };

  (buyFunction, txState);
};

let useApproveLoyaltyTokens = (address: Web3.ethAddress) => {
  let (txState, setTxState) = React.useState(() => UnInitialised);
  let optSteward = useLoyaltyTokenContract();
  let buyFunction = () => {
    Js.log(address);
    let value = parseUnits(. "0", 0);

    setTxState(_ => Created);
    switch (optSteward) {
    | Some(steward) =>
      Js.log(steward);
      let claimLoyaltyTokenPromise =
        steward.approve(.
          voteContractGoerli,
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
        Js.log(error.message)
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
          Js.log(error.message)
        });
        ();
      | None => ()
      };
    },
    txState,
  );
};
let useChangePrice = animal => {
  let animalId = Animal.getId(animal);
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
          Js.log(error.message)
        });
        ();
      | None => ()
      };
    },
    txState,
  );
};
