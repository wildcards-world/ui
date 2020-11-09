open Globals;
open Ethers;

type txHash = string;
type tx = {
  hash: txHash,
  wait: (. unit) => Promise.Js.t(txResult, txError),
};
type parsedUnits;
type txOptions = {
  gasLimit: string,
  value: parsedUnits,
};
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
  (Web3.ethAddress, Web3.abi, Web3.web3Library) => stewardContract =
  "Contract";

[@bs.new] [@bs.module "ethers"]
external getLoyaltyTokenContract:
  (Web3.ethAddress, Web3.abi, Web3.web3Library) => loyaltyTokenContract =
  "Contract";

[@bs.new] [@bs.module "ethers"]
external getVotingContract:
  (Web3.ethAddress, Web3.abi, Web3.web3Library) => voteContract =
  "Contract";

[@bs.module "./abi/voteContract.json"]
external voteContract: Web3.abi = "voteContract";

[@bs.module "./abi/loyaltyToken.json"]
external loyaltyTokenAbi: Web3.abi = "loyaltyToken";

[@bs.module "ethers"] [@bs.scope "utils"]
external parseUnits: (. string, int) => parsedUnits = "parseUnits";

let getExchangeContract =
    (stewardAddress, stewardAbi, library, account, isGsn) => {
  getContract(
    stewardAddress,
    stewardAbi,
    ContractUtil.getProviderOrSigner(library, account, isGsn),
  );
};
let getLoyaltyTokenContract = (stewardAddress, library, account, isGsn) => {
  getLoyaltyTokenContract(
    stewardAddress,
    loyaltyTokenAbi,
    ContractUtil.getProviderOrSigner(library, account, isGsn),
  );
};
let getVotingContract = (stewardAddress, library, account, isGsn) => {
  getVotingContract(
    stewardAddress,
    voteContract,
    ContractUtil.getProviderOrSigner(library, account, isGsn),
  );
};
let stewardAddressMainnet = "0x6D47CF86F6A490c6410fC082Fd1Ad29CF61492d0";
let stewardAddressGoerli = "0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B";
let stewardAddressRinkeby = "0x229Cb219F056A9097b2744594Bc37597380854E8";

let loyaltyTokenAddressMainnet = "0x773c75c2277eD3e402BDEfd28Ec3b51A3AfbD8a4";
let loyaltyTokenAddressGoerli = "0xd7d8c42ab5b83aa3d4114e5297989dc27bdfb715";
// let loyaltyTokenAddressRinkbey = "0xd7d8c42ab5b83aa3d4114e5297989dc27bdfb715";

let voteContractMainnet = "0x03e051b7e42480Cc9D54F1caB525D2Fea2cF4d83";
let voteContractGoerli = "0x316C5f8867B21923db8A0Bd6890A6BFE0Ab6F9d2";
// let voteContractRinkeby = "0x316C5f8867B21923db8A0Bd6890A6BFE0Ab6F9d2";

let stewardAddressMaticMain = "0x6D47CF86F6A490c6410fC082Fd1Ad29CF61492d0";
let stewardAddressMumbai = "0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B";

let loyaltyTokenAddressMaticMain = "0x773c75c2277eD3e402BDEfd28Ec3b51A3AfbD8a4";
let loyaltyTokenAddressMumbai = "0xd7d8c42ab5b83aa3d4114e5297989dc27bdfb715";
// let loyaltyTokenAddressRinkbey = "0xd7d8c42ab5b83aa3d4114e5297989dc27bdfb715";

let getDaiContractAddress = (chain: Client.context, chainId) =>
  switch (chain) {
  | Neither
  | MaticQuery =>
    switch (chainId) {
    | 1 => "TODO"
    | _ =>
      // Goerli-Mumbai
      "0xB014216fd1d2B46c4d0291f0Bee46350227a2C60"
    // Mumbai
    // "0x6D725E5472D8c386D5759259b01278AA202130f0"
    }
  | MainnetQuery => "TODO"
  };

let getStewardAddress = (chain: Client.context, chainId) =>
  switch (chain) {
  | Neither
  | MaticQuery =>
    switch (chainId) {
    | 1 => "TODO"
    | _ =>
      // Goerli-Mumbai
      "0x5FDcd5bE3b476f146DbF46d50e0374f17515292f"
    // Mumbai
    // "0x90ED26DA8ceF71ab3ec6853572677D501Dcc670f"
    }
  | MainnetQuery => "TODO"
  };

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
  | 4 => Some(stewardAddressRinkeby)
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

let useStewardContract = isGsn => {
  let context = RootProvider.useWeb3React();
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

let useLoyaltyTokenContract = isGsn => {
  let context = RootProvider.useWeb3React();

  React.useMemo3(
    () => {
      switch (context.library, context.chainId) {
      | (Some(library), Some(chainId)) =>
        chainId
        ->loyaltyTokenAddressFromChainId
        ->oMap(getLoyaltyTokenContract(_, library, context.account, isGsn))

      | _ => None
      }
    },
    (context.library, context.account, context.chainId),
  );
};

let useVoteContract = isGsn => {
  let context = RootProvider.useWeb3React();

  React.useMemo3(
    () => {
      switch (context.library, context.chainId) {
      | (Some(library), Some(chainId)) =>
        chainId
        ->voteAddressFromChainId
        ->oMap(getVotingContract(_, library, context.account, isGsn))

      | _ => None
      }
    },
    (context.library, context.account, context.chainId),
  );
};

type transactionState =
  | UnInitialised
  | DaiPermit(BN.t)
  | SignMetaTx
  | Created
  | SubmittedMetaTx
  | SignedAndSubmitted(txHash)
  // TODO: get the error message when it is declined.
  //      4001 - means the transaction was declined by the signer
  //      -32000 - means the transaction is always failing (exceeds gas allowance)
  | Declined(string)
  // | DaiPermitDclined(string)
  // | SignMetaTxDclined(string)
  | ServerError(string)
  | Complete(txResult)
  | Failed;

let execDaiPermitMetaTx =
    (
      daiNonce,
      stewardNonce,
      setTxState,
      sendMetaTx,
      userAddress,
      spender,
      lib: Web3.web3Library,
      generateFunctionSignature:
        (
          Web3.Contract.MaticSteward.steward,
          DaiPermit.v,
          DaiPermit.r,
          DaiPermit.s
        ) =>
        string,
      chainId,
      verifyingContract,
    ) => {
  DaiPermit.createPermitSig(
    lib.provider,
    verifyingContract,
    daiNonce,
    chainId,
    userAddress,
    spender,
    userAddress,
  )
  ->Js.Promise.then_(
      rsvSig => {
        setTxState(_ => SignMetaTx);
        open ContractUtil;
        let {r, s, v} = rsvSig;

        let web3 = Web3.new_(lib.provider);

        let steward =
          Web3.Contract.MaticSteward.getStewardContract(web3, spender);

        let functionSignature = generateFunctionSignature(steward, v, r, s);

        let messageToSign =
          ContractUtil.constructMetaTransactionMessage(
            stewardNonce,
            chainId->BN.toString,
            functionSignature,
            spender,
          );

        web3
        ->Web3.personalSign(messageToSign, userAddress)
        ->Js.Promise.then_(
            signature => Js.Promise.resolve((functionSignature, signature)),
            _,
          );
      },
      _,
    )
  ->Js.Promise.then_(
      ((functionSignature, signature)) => {
        open ContractUtil;
        let {r, s, v} = getEthSig(signature);
        let resultPromise =
          sendMetaTx(
            ~network="goerli",
            ~r,
            ~s,
            ~v,
            ~functionSignature,
            userAddress,
          );
        resultPromise;
      },
      _,
    )
  ->Js.Promise.then_(
      result => {
        open ReasonApolloHooks;
        let (simple, _) = result;

        setTxState(_ => SubmittedMetaTx);

        (
          switch (simple) {
          | ApolloHooksMutation.Errors(_)
          | ApolloHooksMutation.NoData => setTxState(_ => Failed)
          | ApolloHooksMutation.Data(data) =>
            let success = data##metaTx##success;
            let errorMsg = data##metaTx##errorMsg;
            let txHash = data##metaTx##txHash;
            if (success) {
              setTxState(_ => SignedAndSubmitted(txHash));

              let providerUrl = "https://goerli.infura.io/v3/c401b8ee3a324619a453f2b5b2122d7a";
              let maticProvider = Ethers.makeProvider(providerUrl);

              let waitForTx =
                maticProvider
                ->Ethers.waitForTransaction(txHash)
                ->Promise.Js.toResult;

              waitForTx->Promise.getOk(tx => {setTxState(_ => Complete(tx))});
              waitForTx->Promise.getError(error => {
                setTxState(_ => Failed);
                Js.log("GOT AN ERROR");
                Js.log(error);
              });
            } else {
              setTxState(_ =>
                ServerError(errorMsg->Option.getWithDefault("Unknown error"))
              );
              ();
            };
          }
        )
        ->ignore;
        Js.Promise.resolve();
      },
      _,
    )
  // TODO: This needs to be
  ->Js.Promise.catch(
      err => {
        Js.log2("this error was caught", err);
        Js.Promise.resolve(""->Obj.magic);
      },
      _,
    )
  ->ignore;
};

let useBuy =
    (
      ~chain,
      animal: TokenId.t,
      isGsn,
      library: option(Web3.web3Library),
      account,
    ) => {
  let animalId = animal->TokenId.toString;

  let optSteward = useStewardContract(isGsn);
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let sendMetaTx = QlHooks.useMetaTx();

  let maticState =
    account->Option.flatMap(usersAddress => {
      QlHooks.useMaticState(~forceRefetch=false, usersAddress, "goerli")
    });

  // GOERLI:
  let verifyingContract = getDaiContractAddress(chain, 5);
  let spender = getStewardAddress(chain, 5);
  // BN.newInt_(80001),
  let chainId = BN.newInt_(5);

  switch (chain) {
  | Client.Neither
  | Client.MaticQuery => (
      (
        (newPrice, oldPrice, wildcardsPercentage, value: string) => {
          switch (library, account, maticState) {
          // TODO: This function should not take in options of these values, they should be defined
          | (Some(lib), Some(userAddress), Some(maticState)) =>
            let daiNonce = maticState##daiNonce;
            let stewardNonce = maticState##stewardNonce;

            setTxState(_ => DaiPermit(value->BN.new_));
            execDaiPermitMetaTx(
              daiNonce,
              stewardNonce,
              setTxState,
              sendMetaTx,
              userAddress,
              spender,
              lib,
              (steward, v, r, s) => {
                steward->Web3.Contract.MaticSteward.buyWithPermit(
                  BN.new_(daiNonce),
                  BN.new_("0"),
                  true,
                  v,
                  r,
                  s,
                  animalId,
                  newPrice->Web3Utils.toWeiFromEth,
                  oldPrice,
                  wildcardsPercentage,
                  value,
                ).
                  encodeABI()
              },
              chainId,
              verifyingContract,
            );
          | _ => ()
          };
        }
      ),
      txState,
    )
  | Client.MainnetQuery => (
      (
        (newPrice, oldPrice, wildcardsPercentage, value: string) => {
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
                oldPriceParsed,
                wildcardsPercentage,
                {
                  gasLimit: "500302", //calculateGasMargin(estimatedGasLimit, GAS_MARGIN),
                  value,
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
        }
      ),
      txState,
    )
  };
};

let useBuyAuction =
    (~chain, animal, isGsn, library: option(Web3.web3Library), account) => {
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let animalId = animal->TokenId.toString;

  let optSteward = useStewardContract(isGsn);

  let sendMetaTx = QlHooks.useMetaTx();

  let maticState =
    account->Option.flatMap(usersAddress => {
      Js.log2("the users address", usersAddress);
      QlHooks.useMaticState(~forceRefetch=false, usersAddress, "goerli");
    });

  let verifyingContract = getDaiContractAddress(chain, 5);
  let spender = getStewardAddress(chain, 5);
  let chainId = BN.newInt_(5);

  switch (chain) {
  | Client.Neither
  | Client.MaticQuery => (
      (
        (newPrice, wildcardsPercentage, value: string) => {
          switch (library, account, maticState) {
          | (Some(lib), Some(userAddress), Some(maticState)) =>
            let daiNonce = maticState##daiNonce;
            let stewardNonce = maticState##stewardNonce;
            setTxState(_ => DaiPermit(value->BN.new_));
            execDaiPermitMetaTx(
              daiNonce,
              stewardNonce,
              setTxState,
              sendMetaTx,
              userAddress,
              spender,
              lib,
              (steward, v, r, s) =>
                steward->Web3.Contract.MaticSteward.buyAuctionWithPermit(
                  BN.new_(daiNonce),
                  BN.new_("0"),
                  true,
                  v,
                  r,
                  s,
                  animalId,
                  newPrice,
                  wildcardsPercentage,
                  value,
                ).
                  encodeABI(),
              chainId,
              verifyingContract,
            );

          | _ =>
            Js.log("something important is null");
            Js.log3(library, account, maticState);
            ();
          };
        }
      ),
      txState,
    )
  | Client.MainnetQuery => (
      (
        (newPrice, wildcardsPercentage, value: string) => {
          let newPriceEncoded = parseUnits(. newPrice, 18);

          let value = parseUnits(. value, 0);

          setTxState(_ => Created);
          switch (optSteward) {
          | Some(steward) =>
            // let buyPromise =
            let buyPromise =
              steward.buyAuction(.
                animalId,
                newPriceEncoded,
                wildcardsPercentage,
                {
                  gasLimit: "500302", //calculateGasMargin(estimatedGasLimit, GAS_MARGIN),
                  value,
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
        }
      ),
      txState,
    )
  };
};

let useRedeemLoyaltyTokens = (animalId: string, isGsn) => {
  let (txState, setTxState) = React.useState(() => UnInitialised);
  let optSteward = useStewardContract(isGsn);
  let buyFunction = () => {
    let value = parseUnits(. "0", 0);

    setTxState(_ => Created);
    switch (optSteward) {
    | Some(steward) =>
      let claimLoyaltyTokenPromise =
        steward._collectPatronage(.
          animalId,
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

let useApproveLoyaltyTokens = () => {
  let (txState, setTxState) = React.useState(() => UnInitialised);
  let optLoyaltyTokens = useLoyaltyTokenContract(false);
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
  let optSteward = useVoteContract(false);
  let buyFunction = (proposalId: string, squareRoot: BN.t) => {
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
        squareRoot->BN.sqr->BN.toString,
        squareRoot->BN.toString,
      );
      Js.log("!!Voting - end!!");
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
  let optSteward = useVoteContract(false);
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

let useUpdateDeposit =
    (~chain, isGsn, library: option(Web3.web3Library), account) => {
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let optSteward = useStewardContract(isGsn);

  let sendMetaTx = QlHooks.useMetaTx();

  let maticState =
    account->Option.flatMap(usersAddress => {
      Js.log2("the users address", usersAddress);
      QlHooks.useMaticState(~forceRefetch=false, usersAddress, "goerli");
    });
  Js.log2("Matic state", maticState);

  // GOERLI:
  let verifyingContract = getDaiContractAddress(chain, 5);
  let spender = getStewardAddress(chain, 5);
  // BN.newInt_(80001),
  let chainId = BN.newInt_(5);

  switch (chain) {
  | Client.Neither
  | Client.MaticQuery => (
      (
        amountToAdd => {
          switch (library, account, maticState) {
          | (Some(lib), Some(userAddress), Some(maticState)) =>
            let daiNonce = maticState##daiNonce;
            let stewardNonce = maticState##stewardNonce;
            setTxState(_ => DaiPermit(amountToAdd->BN.new_));
            execDaiPermitMetaTx(
              daiNonce,
              stewardNonce,
              setTxState,
              sendMetaTx,
              userAddress,
              spender,
              lib,
              (steward, v, r, s) =>
                steward->Web3.Contract.MaticSteward.depositWithPermit(
                  BN.new_(daiNonce),
                  BN.new_("0"),
                  true,
                  v,
                  r,
                  s,
                  userAddress,
                  BN.new_(amountToAdd),
                ).
                  encodeABI(),
              chainId,
              verifyingContract,
            );

          | _ =>
            Js.log("something important is null");
            Js.log3(library, account, maticState);
            ();
          };
        }
      ),
      txState,
    )
  | Client.MainnetQuery => (
      (
        (value: string) => {
          let value = parseUnits(. value, 0);

          setTxState(_ => Created);
          switch (optSteward) {
          | Some(steward) =>
            let updateDepositPromise =
              steward.depositWei(. {
                gasLimit: "500302", //calculateGasMargin(estimatedGasLimit, GAS_MARGIN),

                value,
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
        }
      ),
      txState,
    )
  };
};

let useWithdrawDeposit =
    (~chain, isGsn, library: option(Web3.web3Library), account) => {
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let optSteward = useStewardContract(isGsn);

  // let nonce = "";

  // GOERLI:
  // let verifyingContract = getDaiContractAddress(chain, 5);
  let spender = getStewardAddress(chain, 5);
  // BN.newInt_(80001),
  // let chainId = BN.newInt_(5);

  switch (chain) {
  | Client.Neither
  | Client.MaticQuery => (
      (
        amountToWithdraw => {
          switch (library, account) {
          | (Some(lib), Some(account)) =>
            {
              let web3 = Web3.new_(lib.provider);

              let steward =
                Web3.Contract.MaticSteward.getStewardContract(web3, spender);

              steward->Web3.Contract.MaticSteward.withdrawDeposit(
                amountToWithdraw,
              ).
                send({
                from: account,
              });
            }
            ->Js.Promise.catch(
                err => {
                  Js.log2("this error was caught", err);
                  Js.Promise.resolve(""->Obj.magic);
                },
                _,
              )
            ->ignore
          | _ => ()
          };
        }
      ),
      txState,
    )
  | Client.MainnetQuery => (
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
                  gasLimit: "500302", //calculateGasMargin(estimatedGasLimit, GAS_MARGIN),

                  value,
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
        }
      ),
      txState,
    )
  };
};

let useUserLoyaltyTokenBalance = (address: Web3.ethAddress) => {
  let (result, setResult) = React.useState(() => None);
  let (counter, setCounter) = React.useState(() => 0);

  let optSteward = useLoyaltyTokenContract(false);

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

  let optLoyaltyTokens = useLoyaltyTokenContract(false);
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

  let optVoteContract = useVoteContract(false);

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

  let optVoteContract = useVoteContract(false);

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

  let optVoteContract = useVoteContract(false);

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

let useHasUserVotedForProposalIteration =
    (iteration, userAddress, projectId, isGsn) => {
  let (result, setResult) = React.useState(() => None);
  let (counter, setCounter) = React.useState(() => 0);

  let optVoteContract = useVoteContract(isGsn);

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

  let optVoteContract = useVoteContract(false);

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

  let optSteward = useVoteContract(false);

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

let useChangePrice = (animal, isGsn) => {
  let animalId = TokenId.toString(animal);
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let optSteward = useStewardContract(isGsn);

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
              gasLimit: "500302", //calculateGasMargin(estimatedGasLimit, GAS_MARGIN),

              value,
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
