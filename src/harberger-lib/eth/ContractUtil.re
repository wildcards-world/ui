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
