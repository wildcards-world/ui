// CODE TO BUY:
let getProviderOrSigner =
    (
      library: RootProviderTypes.web3Library,
      account: option(Web3.ethAddress),
    ) => {
  switch (account) {
  // | Some(_account) => library
  | Some(account) =>
    // Js.log("library");
    // Js.log(library);
    // Js.log(account);
    library.getSigner(. account)
  | None => library
  };
};

// let isAddress = (value) => {
//   try {
//     getAddress(value.toLowerCase());
//   } catch {
//     return false;
//   }
// }

// let getContract = (address, abi, library, account) => {
//   if (!isAddress(address) || address === AddressZero) {
//     throw Error(`Invalid 'address' parameter '${address}'.`);
//   }

//   return new Contract(address, ABI, getProviderOrSigner(library, account));
// }
type abi;
type txResult;
type tx = {wait: (. unit) => Promise.promise(txResult)};
type parsedUnits;
type txOptions = {value: parsedUnits};
type estimateBuy = {
  buy: (. string, parsedUnits, txOptions) => Promise.promise(string),
};
type stewardContract = {
  estimate: estimateBuy,
  buy: (. string, parsedUnits, txOptions) => Promise.promise(tx),
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

let useBuy = animal => {
  let animalId = Animal.getId(animal);
  // let [txResult, ]

  let optSteward =
    useStewardContract("0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B");

  (
    (newPrice, _txObject) => {
      let newPriceEncoded = parseUnits(. newPrice, 18);
      let value = parseUnits(. "0.1", 18);

      // switch (optSteward) {
      // | Some(steward) =>
      //   let _ =
      //     steward.estimate.buy(.
      //       animalId,
      //       newPriceEncoded,
      //       {
      //         // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
      //         value: value,
      //       },
      //     )
      //     ->Promise.get(estimatedGas => {Js.log(estimatedGas)});
      //   ();
      // | None => ()
      // };
      switch (optSteward) {
      | Some(steward) =>
        let _ =
          steward.buy(.
            animalId,
            newPriceEncoded,
            {
              // gasLimit: calculateGasMargin(estimatedGasLimit, GAS_MARGIN)
              value: value,
            },
          )
          ->Promise.get(tx => {tx.wait(.)->Promise.get(Js.log)});
        ();
      | None => ()
      };
    },
    [],
  );
};
