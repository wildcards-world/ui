open Globals;

[@react.component]
let make =
    (
      ~children: React.element,
      ~txHash: option(string),
      ~closeButtonText: string,
    ) => {
  let etherscanUrl = RootProvider.useEtherscanUrl();
  // let clearNonUrlState = RootProvider.useClearNonUrlState();

  switch (txHash) {
  | None => children
  | Some(txHash) =>
    <React.Fragment>
      <Rimble.Heading>
        closeButtonText->restr
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
  };
};
