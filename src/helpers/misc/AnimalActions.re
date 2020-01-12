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
type tx = {wait: (. unit) => Promise.Js.t(txResult, txError)};
type parsedUnits;
type txOptions = {value: parsedUnits};
type estimateBuy = {
  buy: (. string, parsedUnits, txOptions) => Promise.Js.t(string, string),
};
type stewardContract = {
  estimate: estimateBuy,
  buy: (. string, parsedUnits, txOptions) => Promise.Js.t(tx, txError),
  depositWei: (. txOptions) => Promise.Js.t(tx, txError),
  withdrawDeposit: (. parsedUnits, txOptions) => Promise.Js.t(tx, txError),
};

[@bs.new] [@bs.module "ethers"]
external getContract:
  (Web3.ethAddress, abi, RootProviderTypes.web3Library) => stewardContract =
  "Contract";

[@bs.module "../../contracts/abi/steward.json"]
external stewardAbi: abi = "stewardAbi";

[@bs.module "ethers"] [@bs.scope "utils"]
external parseUnits: (. string, int) => parsedUnits = "parseUnits";

let getExchangeContract = (stewardAddress, library, account) => {
  getContract(
    stewardAddress,
    stewardAbi,
    getProviderOrSigner(library, account),
  );
};

let useStewardContract = stewardAddress => {
  let context = RootProvider.useWeb3React();

  React.useMemo3(
    () => {
      switch (context.library) {
      | Some(library) =>
        Some(getExchangeContract(stewardAddress, library, context.account))
      | _ => None
      }
    },
    (stewardAddress, context.library, context.account),
  );
};

type transactionState =
  | UnInitialised
  | Created
  | SignedAndSubmitted
  | Declined
  | Complete(txResult)
  | Failed;

let useBuy = animal => {
  let animalId = Animal.getId(animal);
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let optSteward =
    useStewardContract("0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B");

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
          setTxState(_ => SignedAndSubmitted);
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

let useUpdateDeposit = () => {
  let (txState, setTxState) = React.useState(() => UnInitialised);

  let optSteward =
    useStewardContract("0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B");

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
          setTxState(_ => SignedAndSubmitted);
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

  let optSteward =
    useStewardContract("0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B");

  (
    amountToWithdraw => {
      let value = parseUnits(. "0", 0);
      Js.log(amountToWithdraw ++ " is the amount I'm trying to withdrw");
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
          setTxState(_ => SignedAndSubmitted);
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
