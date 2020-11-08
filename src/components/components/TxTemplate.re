open Globals;

[@react.component]
let make =
    (
      ~children: React.element,
      ~txState: ContractActions.transactionState,
      ~closeButtonText: string,
    ) => {
  let etherscanUrl = RootProvider.useEtherscanUrl();
  let clearNonUrlState = RootProvider.useClearNonUrlState();

  switch (txState) {
  | ContractActions.UnInitialised => children
  | DaiPermit(value) =>
    <React.Fragment>
      <Rimble.Heading>
        {(
           "Please accept the signature in your signer to allow use of "
           ++ value->Web3Utils.fromWeiBNToEthPrecision(~digits=2)
           ++ "."
         )
         ->restr}
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Flex justifyContent="center">
        <Rimble.Loader size="80px" />
      </Rimble.Flex>
    </React.Fragment>
  | SignMetaTx =>
    <React.Fragment>
      <Rimble.Heading>
        "Please accept the signature in your signer to submit this transaction."
        ->restr
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Flex justifyContent="center">
        <Rimble.Loader size="80px" />
      </Rimble.Flex>
    </React.Fragment>
  | ContractActions.Created =>
    <React.Fragment>
      <Rimble.Heading>
        "Processing Transaction "->restr
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text> "Tx Created."->restr </Rimble.Text>
      <Rimble.Flex justifyContent="center">
        <Rimble.Loader size="80px" />
      </Rimble.Flex>
    </React.Fragment>
  // TODO: make it show the link to the tx on etherscan.io!
  | ContractActions.SignedAndSubmitted(txHash) =>
    <React.Fragment>
      <Rimble.Heading>
        "Processing Transaction "->restr
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text>
        <a
          href={j|https://$etherscanUrl/tx/$txHash|j}
          target="_blank"
          rel="noopener noreferrer">
          "View the transaction on etherscan.io"->restr
        </a>
      </Rimble.Text>
      <Rimble.Loader className=Styles.centerItems size="80px" />
    </React.Fragment>
  | ContractActions.Complete(result) =>
    let txHash = result.transactionHash;
    <React.Fragment>
      <Rimble.Heading>
        "Transaction Complete "->restr
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text>
        <a
          href={j|https://$etherscanUrl/tx/$txHash|j}
          target="_blank"
          rel="noopener noreferrer">
          "View the transaction on etherscan.io"->restr
        </a>
      </Rimble.Text>
      <Rimble.Button onClick={_e => clearNonUrlState()}>
        closeButtonText->restr
      </Rimble.Button>
    </React.Fragment>;
  | ContractActions.Declined(message) =>
    <React.Fragment>
      <Rimble.Heading>
        "The transaction was declined by signing device, please try again."
        ->restr
      </Rimble.Heading>
      <p> {("Failure reason: " ++ message)->restr} </p>
      children
    </React.Fragment>
  | ContractActions.Failed =>
    <React.Fragment>
      <Rimble.Heading>
        "The transaction failed."->restr
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text>
        "It is possible that someone else bought the token before you, or the price changed. If you are unsure please feel free to contact our support."
        ->restr
      </Rimble.Text>
      children
    </React.Fragment>
  };
};
