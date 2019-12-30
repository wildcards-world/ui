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
      <Rimble.Loader
        className=Styles.centerItems /*color="green"*/
        size="80px"
      />
    </React.Fragment>
  | AnimalActions.SignedAndSubmitted =>
    <React.Fragment>
      <Rimble.Heading>
        "Processing Transaction "->React.string
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text> "Tx ."->React.string </Rimble.Text>
      <Rimble.Loader
        className=Styles.centerItems /*color="green"*/
        size="80px"
      />
    </React.Fragment>
  | AnimalActions.Complete(_result) =>
    <React.Fragment>
      <Rimble.Heading>
        "Transaction Complete "->React.string
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text> "Tx ."->React.string </Rimble.Text>
      <Rimble.Loader
        className=Styles.centerItems /*color="green"*/
        size="80px"
      />
    </React.Fragment>
  | _ =>
    <React.Fragment>
      <Rimble.Heading>
        "Processing Transaction "->React.string
        <WildcardsLoader />
      </Rimble.Heading>
      <Rimble.Text> "Unimplemented yet..."->React.string </Rimble.Text>
      <Rimble.Loader
        className=Styles.centerItems /*color="green"*/
        size="80px"
      />
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
