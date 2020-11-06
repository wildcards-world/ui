let getProviderOrSigner =
    (
      library: Web3.web3Library,
      account: option(Web3.ethAddress),
      isGsn: bool,
    ) => {
  switch (account) {
  | Some(account) =>
    if (isGsn) {
      // TODO: wrap this in the gsn stuff
      library.getSigner(. account);
    } else {
      library.getSigner(. account);
    }
  | None => library
  };
};

[@bs.module "./metatxHelpers.js"]
external constructMetaTransactionMessage:
  (string, string, string, string) => string =
  "constructMetaTransactionMessage";

type ethSig = {
  r: string,
  s: string,
  v: int,
};

[@bs.send] external slice: (string, int, int) => string = "slice";
[@bs.val] external parseInt: (string, int) => int = "parseInt";

let getEthSig = sigString => {
  {
    r: sigString->slice(0, 66),
    s: "0x" ++ sigString->slice(66, 130),
    v: parseInt(sigString->slice(130, 132), 16),
  };
};
