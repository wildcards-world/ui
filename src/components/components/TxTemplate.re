[@react.component]
let make =
    (~children: React.element, ~txState: AnimalActions.transactionState) => {
  let networkId = RootProvider.useNetworkId();

  let etherscanUrl = {
    switch (networkId) {
    | Some(5) => "goerli.etherscan.io"
    | _ => "etherscan.io"
    };
  };
  let clearNonUrlState = RootProvider.useClearNonUrlState();

  switch (txState) {
  | AnimalActions.UnInitialised => children
  | AnimalActions.Created =>
    <React.Fragment>
      <Rimble.Heading>
        "Processing Transaction "->React.string
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text> "Tx Created."->React.string </Rimble.Text>
      <Rimble.Loader className=Styles.centerItems size="80px" />
    </React.Fragment>
  // TODO: make it show the link to the tx on etherscan.io!
  | AnimalActions.SignedAndSubmitted(txHash) =>
    <React.Fragment>
      <Rimble.Heading>
        "Processing Transaction "->React.string
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text>
        <a href={j|https://$etherscanUrl/tx/$txHash|j} target="_blank">
          "View the transaction on etherscan.io"->React.string
        </a>
      </Rimble.Text>
      <Rimble.Loader className=Styles.centerItems size="80px" />
    </React.Fragment>
  | AnimalActions.Complete(result) =>
    let txHash = result.transactionHash;
    <React.Fragment>
      <Rimble.Heading>
        "Transaction Complete "->React.string
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text>
        <a href={j|https://$etherscanUrl/tx/$txHash|j} target="_blank">
          "View the transaction on etherscan.io"->React.string
        </a>
      </Rimble.Text>
      <Rimble.Button onClick={_e => clearNonUrlState()}>
        "Back to view animal"->React.string
      </Rimble.Button>
    </React.Fragment>;
  | AnimalActions.Declined =>
    <React.Fragment>
      <Rimble.Heading>
        "The transaction was declined by signing device, please try again."
        ->React.string
        <WildcardsLoader />
      </Rimble.Heading>
      children
    </React.Fragment>
  | AnimalActions.Failed =>
    <React.Fragment>
      <Rimble.Heading>
        "The transaction failed."->React.string
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text>
        "It is possible that someone else bought the token before you, or the price changed. If you are unsure please feel free to contact our support."
        ->React.string
      </Rimble.Text>
      children
    </React.Fragment>
  };
};
