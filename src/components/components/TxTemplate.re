[@react.component]
let make =
    (~children: React.element, ~txState: AnimalActions.transactionState) => {
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
  | AnimalActions.SignedAndSubmitted =>
    <React.Fragment>
      <Rimble.Heading>
        "Processing Transaction "->React.string
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text> "Tx ."->React.string </Rimble.Text>
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
        <a href={j|https://etherscan.io/tx/$txHash|j}>
          "View the transaction on Etherscan"->React.string
        </a>
      </Rimble.Text>
    </React.Fragment>;
  | AnimalActions.Declined =>
    <React.Fragment>
      <Rimble.Heading>
        "The transaction was desclined by signing device, please try again."
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
  // <Fragment>
  //   {(() => {
  //     switch (txObjects[0].status) {
  //       case 'pending':
  //         return <Fragment>
  //           {/* <Text>The transaction was is being processed.</Text> */}
  //           <Loader className={centerItems} color='green' size='80px' />
  //         </Fragment>
  //       case 'error':
  //         return <Text>The transaction was failed, please try again.</Text>
  //       case 'success':
  //         return <Fragment>
  //           <Text>The transaction was successful.</Text>
  //           <Text>It has been confirmed in the blockchain{/*txObjects[0].confirmations.length*/}.</Text>
  //           <Text>
  //             <a href={'https://etherscan.io/tx/' + txObjects[0].receipt.transactionHash} target='_blank'>View transaction on Ethersan</a>
  //           </Text>
  //         </Fragment>
  //     }
  //   })()}
  // </Fragment>
  // :
  // <Fragment>
  //   <Text>Sending transaction to signer.
  //   </Text>
  //   <Loader className={centerItems} color='green' size='80px' />
  // </Fragment>
  };
};
