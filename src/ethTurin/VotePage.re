open Globals;

let dragonImg = "/img/animals/Glen.svg";
let refreshImg = "/img/icons/refresh.svg";

let wildTomorrowFundImg = "/img/conservation-partners/wild-tomorrow-fund.png";

let laSendaVerdeImg = "/img/conservation-partners/senda-verde.png";

let greatWhaleConservancyImg = "/img/conservation-partners/great-whale-conservancy-small.png";

let darwinAnimalDoctorsImg = "/img/conservation-partners/darwin-animal-doctors.svg";

type conservationPartnerType = {
  name: string,
  image: string,
  link: string,
  index: int,
};

let conservationPartners: array(conservationPartnerType) = [|
  {
    name: "Wild Tomorrow Fund",
    image: wildTomorrowFundImg,
    link: "https://www.wildtomorrowfund.org/",
    index: 1,
  },
  {
    name: "La Senda Verde",
    image: laSendaVerdeImg,
    link: "http://www.sendaverde.org/",
    index: 2,
  },
  {
    name: "The Great Whale Conservancy",
    image: greatWhaleConservancyImg,
    link: "http://greatwhaleconservancy.org/",
    index: 3,
  },
  {
    name: "Darwin Animal Doctors",
    image: darwinAnimalDoctorsImg,
    link: "http://darwinanimaldoctors.org/",
    index: 4,
  },
|];

type organisationArrayIndex = int;

type voteStep =
  | DefaultView // sub-states can either be loading data, ready, or user is not eligible to vote
  | SelectedOrganisationToVote(organisationArrayIndex, float => unit)
  // | SelectedOrganisationToVote(organisationArrayIndex, currentVote)
  | ProcessTransaction
  | ViewResults;

module HackyComponentThatCallsAFunctionOnce = {
  [@react.component]
  let make = (~reloadFunction) => {
    let (hasCalledFunction, setHasCalledFunction) =
      React.useState(_ => false);
    if (!hasCalledFunction) {
      reloadFunction();
      setHasCalledFunction(_ => true);
    } else {
      ();
    };

    React.null;
  };
};

// TODO: this shouldn't be done using a component, it should be done with a 'useEffect'
module HackyComponentThatReloadsOnTimeout = {
  [@react.component]
  let make = (~reloadFunction: unit => unit, ~timeoutTime) => {
    let (hasCalledFunction, setHasCalledFunction) =
      React.useState(_ => false);

    React.useEffect4(
      () => {
        let timeout =
          Js.Global.setTimeout(
            () =>
              if (!hasCalledFunction) {
                reloadFunction();
                setHasCalledFunction(_ => true);
              } else {
                ();
              },
            timeoutTime,
          );
        Some(() => Js.Global.clearTimeout(timeout));
      },
      (reloadFunction, hasCalledFunction, setHasCalledFunction, timeoutTime),
    );

    React.null;
  };
};

module OrganisationVote = {
  [@react.component]
  let make = (~conservationPartner, ~selectConservation, ~index) => {
    <Rimble.Box width=[|1., 0.25|]>
      <a href={conservationPartner.link} target="_blank">
        <img
          className=Css.(
            style([
              display(`block),
              width(`percent(70.)),
              maxWidth(`px(800)),
              margin(auto),
            ])
          )
          src={conservationPartner.image}
        />
      </a>
      <Rimble.Button
        className=Css.(
          style([display(`block), margin(auto), width(`percent(90.))])
        )
        disabled=true
        onClick={_ => selectConservation(index)}>
        "Voting Disabled"->restr
      </Rimble.Button>
    </Rimble.Box>;
  };
};
module OrganisationVoteResult = {
  [@react.component]
  let make = (~conservationPartner, ~currentIteration, ~totalVotes) => {
    // TODO: add a reload on timeout (incase this value doesn't load first time)
    let (proposal1Votes, _reload) =
      ContractActions.useProposalVotes(
        currentIteration,
        conservationPartner.index->string_of_int,
      );

    let displayText =
      switch (proposal1Votes) {
      | Some(proposalVotes) =>
        (
          (
            (
              (proposalVotes |*| BN.new_("10000") |/| totalVotes)
              ->BN.toStringGet(.)
              ->Float.fromString
              |||| 0.
            )
            /. 100.
          )
          ->Float.toString
          ++ "%"
        )
        ->restr
      | None => <Rimble.Loader />
      };
    let numberOfVotes =
      switch (proposal1Votes) {
      | Some(proposalVotes) =>
        "with "
        ++ (proposalVotes |*| BN.new_("1000000000"))
           ->Web3Utils.fromWeiBNToEthPrecision(~digits=2)
        ++ " votes in total"
      | None => ""
      };

    <Rimble.Box width=[|1., 0.25|]>
      <a href={conservationPartner.link} target="_blank">
        <img
          className=Css.(
            style([
              display(`block),
              width(`percent(70.)),
              maxWidth(`px(800)),
              margin(auto),
            ])
          )
          src={conservationPartner.image}
        />
      </a>
      <p> displayText </p>
      <p> numberOfVotes->restr </p>
    </Rimble.Box>;
  };
};

module VoteResults = {
  [@react.component]
  let make = (~currentIteration) => {
    // TODO: add a reload on timeout (incase this value doesn't load first time)
    let (totalVotes, _reload) = ContractActions.useTotalVotes();
    let currentIteration = currentIteration->string_of_int;
    let (currentWinner, _reload) = ContractActions.useCurrentWinner();

    <>
      {switch (totalVotes) {
       | Some(totalVotes) =>
         <Rimble.Flex flexWrap="wrap" alignItems="center">
           {conservationPartners
            ->Array.map(conservationPartner =>
                <OrganisationVoteResult
                  key={conservationPartner.name}
                  currentIteration
                  totalVotes
                  conservationPartner
                />
              )
            ->React.array}
         </Rimble.Flex>
       | None => <p> <Rimble.Loader /> "loading current standings"->restr </p>
       }}
      {switch (currentWinner) {
       | Some(currentWinner) =>
         <p className=Css.(style([fontSize(em(2.))]))>
           <strong>
             {(
                conservationPartners[currentWinner - 1]
                <$> (partner => partner.name)
                |||| "Unknown"
              )
              ->restr}
           </strong>
           " is currently winning"->restr
         </p>
       //  <h2> {currentWinner->string_of_int->restr} </h2>
       | None => React.null
       }}
    </>;
  };
};

module ApproveLoyaltyTokens = {
  [@react.component]
  let make = (~reloadFunction) => {
    let (approveLoyaltyTokens, transactionStatus) =
      ContractActions.useApproveLoyaltyTokens();
    let etherScanUrl = RootProvider.useEtherscanUrl();

    <div>
      {switch (transactionStatus) {
       | UnInitialised =>
         <>
           <Rimble.Loader />
           <p>
             <a onClick={_ => {approveLoyaltyTokens()}}>
               ">>Click here to enable wildcards vote with your tokens<<"
               ->restr
             </a>
           </p>
         </>
       | Created =>
         <>
           <Rimble.Loader />
           <p>
             "Transaction Created - please check the details and confirm."
             ->restr
           </p>
         </>
       | SignedAndSubmitted(txHash) =>
         <>
           <Rimble.Loader />
           <p>
             "Processing: "->restr
             <a
               target="_blank"
               rel="noopener noreferrer"
               href={"https://" ++ etherScanUrl ++ "/tx/" ++ txHash}>
               "view transaction"->restr
             </a>
           </p>
         </>
       | Declined(message) =>
         <p> {("Submitting transaction failed: " ++ message)->restr} </p>
       | Complete(_txResult) =>
         <p>
           <HackyComponentThatCallsAFunctionOnce reloadFunction />
           "You are now ready to vote :)"->restr
         </p>
       | Failed => <p> "Transaction failed"->restr </p>
       }}
    </div>;
  };
};

[@react.component]
let make = () => {
  let (voteStep, setVoteStep) = React.useState(() => DefaultView);
  // let (voteStep, setVoteStep) = React.useState(() => ViewResults);

  let (voteForProject, _transactionStatus) =
    ContractActions.useVoteForProject();
  let selectConservation = conservationArrayIndex => {
    let submitVoteFunction: float => unit =
      votes => {
        let conservationVotedContractIndex =
          conservationPartners->Array.getUnsafe(conservationArrayIndex).index;

        // NOTE: if they vote with more than about 300000 there will be an overflow here...
        let numberOfVotes =
          (votes *. 100000.)
          ->int_of_float
          ->Int.toString
          ->BN.new_
          ->BN.mulGet(. BN.new_("10000"));

        voteForProject(
          conservationVotedContractIndex->string_of_int,
          numberOfVotes,
        );

        setVoteStep(_ => ProcessTransaction);
      };

    setVoteStep(_ =>
      SelectedOrganisationToVote(conservationArrayIndex, submitVoteFunction)
    );
  };

  let resetVoting = () => {
    setVoteStep(_ => DefaultView);
  };

  let glen = TokenId.makeFromInt(13);
  let optCurrentPrice = PriceDisplay.usePrice(glen);
  let (_, _, ratio, _) = QlHooks.usePledgeRateDetailed(glen);
  // TODO: investigate why the USD price doesn't load here.
  let (optMonthlyPledgeEth, optMonthlyPledgeUsd) =
    switch (optCurrentPrice) {
    | Some((priceEth, optPriceUsd)) => (
        Some(
          toFixedWithPrecisionNoTrailingZeros(
            Float.fromString(priceEth)->Accounting.defaultZeroF *. ratio,
            ~digits=5,
          ),
        ),
        switch (optPriceUsd) {
        | Some(_priceUsd) => None
        | None => None
        },
      )
    | None => (None, None)
    };

  <Rimble.Box className=Styles.topBody>
    <Rimble.Box>
      <Rimble.Flex flexWrap="wrap">
        <Rimble.Box width=[|1., 1., 0.3|]>
          <img
            className=Css.(style([maxWidth(`px(800)), margin(auto)]))
            src=dragonImg
          />
          <a href="/#details/13">
            <h3 className=Css.(style([textAlign(`center)]))>
              "Glen the Dragon from Turin"->restr
            </h3>
          </a>
          <p className=Css.(style([textAlign(`center)]))>
            "Monthly contribution: "->restr
            {switch (optMonthlyPledgeEth) {
             | Some(monthlyPledgeEth) => (monthlyPledgeEth ++ " ETH")->restr
             | None =>
               <Rimble.Loader className=Css.(style([margin(auto)])) />
             }}
            <br />
            <small>
              {switch (optMonthlyPledgeUsd) {
               | Some(monthlyPledgeUsd) =>
                 ("(" ++ monthlyPledgeUsd ++ " USD)")->restr
               | None => React.null
               }}
            </small>
          </p>
          <br />
          <br />
          <br />
          <h4
            className=Css.(
              style([width(`percent(100.)), textAlign(center)])
            )>
            "Voting coming again soon!"->restr
          </h4>
        </Rimble.Box>
        <Rimble.Box width=[|1., 1., 0.7|]>
          <h3 className=Css.(style([textDecoration(`underline)]))>
            "How it works"->restr
          </h3>
          <p>
            "Glen is a special Wildcard, this mystical creature is not tied to a specific conservation but rather each month the owners of Wildcards vote for a conservation they think should receive the funds raised by Glen."
            ->restr
          </p>
          <p>
            "The voting mechanism uses quadratic voting. Wildcards owners vote using Wildcards Loyalty tokens which they earn from holding a Wildcard. Quadratic voting means that the number of loyalty tokens don't represent the exact number of votes but rather the number of loyalty tokens is square rooted to represent the number of votes."
            ->restr
          </p>
          <h3 className=Css.(style([textDecoration(`underline)]))>
            "Quadratic Voting    "->restr
            {voteStep != DefaultView
               ? <img
                   onClick={_ => resetVoting()}
                   src=refreshImg
                   className=Css.(
                     style([maxHeight(`px(16)), paddingLeft(`rem(1.))])
                   )
                 />
               : React.null}
          </h3>
          <small>
            <p>
              "Unfortunately we have decided to stop running our DAO on mainnet ethereum. We are moving all of this code to Matic where voting will be much cheaper and more frictionless"
              ->restr
            </p>
          </small>
          <Rimble.Flex flexWrap="wrap" alignItems="center">
            {conservationPartners
             ->Array.mapWithIndex((index, conservationPartner) =>
                 <OrganisationVote
                   conservationPartner
                   index
                   selectConservation
                   //  currentIteration=1
                   key={conservationPartner.name}
                 />
               )
             ->React.array}
          </Rimble.Flex>
        </Rimble.Box>
      </Rimble.Flex>
    </Rimble.Box>
  </Rimble.Box>;
};
