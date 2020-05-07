open Globals;

let dragonImg = [%bs.raw {|require('../img/animals/Glen.svg')|}];
let refreshImg = [%bs.raw {|require('../img/icons/refresh.svg')|}];

let wildTomorrowFundImg = [%bs.raw
  {|require('../img/conservation-partners/wild-tomorrow-fund.png')|}
];

let laSendaVerdeImg = [%bs.raw
  {|require('../img/conservation-partners/senda-verde.png')|}
];

let greatWhaleConservancyImg = [%bs.raw
  {|require('../img/conservation-partners/great-whale-conservancy-small.png')|}
];

let darwinAnimalDoctorsImg = [%bs.raw
  {|require('../img/conservation-partners/darwin-animal-doctors.svg')|}
];

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

// type reasonUneligeableToVote =
//   | DontOwnAWildcard
//   | EligeableToVote;
type organisationArrayIndex = int;
// type currentVote = {
//   vote: BN.bn,
//   maxPossibleVote: BN.bn // This should be calculated at the beginning
// };
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

module OrganisationVote = {
  [@react.component]
  let make =
      (
        ~currentIteration,
        ~conservationPartner,
        ~cannotVote,
        ~selectConservation,
        ~index,
        ~currentUser,
      ) => {
    let (hasVotedForProposal1Votes, _reload) =
      AnimalActions.useHasUserVotedForProposalIteration(
        currentIteration->string_of_int,
        currentUser,
        conservationPartner.index->string_of_int,
      );

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
        disabled={hasVotedForProposal1Votes |||| true || cannotVote}
        onClick={_ => selectConservation(index)}>
        {(
           hasVotedForProposal1Votes
           >>= (hasVoted => hasVoted ? Some("Cannot Vote Twice") : None)
           |||| "Vote"
         )
         ->restr}
      </Rimble.Button>
    </Rimble.Box>;
  };
};
module OrganisationVoteResult = {
  [@react.component]
  let make = (~conservationPartner, ~currentIteration, ~totalVotes) => {
    // ~cannotVote,
    // ~selectConservation,
    // ~currentUser,
    // let (hasVotedForProposal1Votes, _reload) =
    //   AnimalActions.useHasUserVotedForProposalIteration(
    //     currentIteration->string_of_int,
    //     currentUser,
    //     conservationPartner.index->string_of_int,
    //   );
    let (proposal1Votes, _reload) =
      AnimalActions.useProposalVotes(
        currentIteration,
        conservationPartner.index->string_of_int,
      );
    let displayText =
      switch (proposal1Votes) {
      | Some(proposalVotes) =>
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
      | None => "loading"
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
      <p> displayText->restr </p>
      <p> numberOfVotes->restr </p>
    </Rimble.Box>;
  };
};

module VoteResults = {
  [@react.component]
  let make = (~currentIteration) => {
    let (totalVotes, _reload) = AnimalActions.useTotalVotes();
    let currentIteration = currentIteration->string_of_int;

    switch (totalVotes) {
    | Some(totalVotes) =>
      <Rimble.Flex flexWrap="wrap" alignItems="center">
        {conservationPartners
         ->Array.map(conservationPartner =>
             <OrganisationVoteResult
               currentIteration
               totalVotes
               conservationPartner
             />
           )
         ->React.array}
      </Rimble.Flex>
    | None => <p> "loading total"->restr </p>
    };
  };
};

module ApproveLoyaltyTokens = {
  [@react.component]
  let make = (~reloadFunction) => {
    let (approveLoyaltyTokens, transactionStatus) =
      AnimalActions.useApproveLoyaltyTokens();
    let etherScanUrl = RootProvider.useEtherscanUrl();

    <div>
      {switch (transactionStatus) {
       | UnInitialised =>
         <p>
           <a onClick={_ => {approveLoyaltyTokens()}}>
             ">>Click here to enable wildcards vote with your tokens<<"->restr
           </a>
         </p>
       | Created =>
         <p>
           "Transaction Created - please check the details and confirm."->restr
         </p>
       | SignedAndSubmitted(txHash) =>
         <p>
           "Processing: "->restr
           <a
             target="_blank"
             rel="noopener noreferrer"
             href={"https://" ++ etherScanUrl ++ "/tx/" ++ txHash}>
             "view transaction"->restr
           </a>
         </p>
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

  let (voteForProject, transactionStatus) = AnimalActions.useVoteForProject();
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

  let networkIdOpt = RootProvider.useNetworkId();
  let notGoerliNetworkWarning = networkId =>
    switch (networkId) {
    | Some(5) => React.null
    // | Some(1) => React.null
    | _ =>
      <>
        <h4 className=Css.(style([color(red)]))>
          "Voting is currently only available on the Goerli testnet right."
          ->restr
        </h4>
        <h2 className=Css.(style([color(red)]))>
          "Launching in: "->restr
          <CountDown
            endDateMoment={MomentRe.momentUtcDefaultFormat(
              "2020-05-08T08:00:00",
            )}
          />
        </h2>
      </>
    };

  let notGoerliNetwork = networkId => networkId != 5;

  let optCurrentPrice = PriceDisplay.uesPrice(Animal.Glen);
  let (_, _, ratio, _) = Animal.pledgeRate(Animal.Glen);
  let (optMonthlyPledgeEth, optMonthlyPledgeUsd) =
    switch (optCurrentPrice) {
    | Some((priceEth, optPriceUsd)) => (
        Some(
          Js.Float.toString(
            Belt.Float.fromString(priceEth)->Accounting.defaultZeroF *. ratio,
          ),
        ),
        switch (optPriceUsd) {
        | Some(_priceUsd) => None
        | None => None
        },
      )
    | None => (None, None)
    };

  let currentUser = RootProvider.useCurrentUser();

  let userAddressLowerCase =
    switch (currentUser) {
    | Some(currentUser) => currentUser->Js.String.toLowerCase
    | _ => "0x0000000000000000000000000000000000000000"
    };

  let patronQueryOpt = QlHooks.usePatronQuery(userAddressLowerCase);

  let currentlyOwnedTokens =
    switch (patronQueryOpt) {
    | Some(patronQueryResult) =>
      patronQueryResult##patron
      ->oMap(patron => patron##tokens->Array.map(token => token##id))
      ->setDefault([||])
    | None => [||]
    };

  let etherScanUrl = RootProvider.useEtherscanUrl();

  let txStateDisplay =
    <div>
      {switch (transactionStatus) {
       | UnInitialised => <p> "startingTransaction"->restr </p>
       | Created => <p> "Transaction Created"->restr </p>
       | SignedAndSubmitted(txHash) =>
         <p>
           "Processing: "->restr
           <a
             target="_blank"
             rel="noopener noreferrer"
             href={"https://" ++ etherScanUrl ++ "/tx/" ++ txHash}>
             "view transaction"->restr
           </a>
         </p>
       | Declined(message) =>
         <p> {("Submitting transaction failed: " ++ message)->restr} </p>
       | Complete(_txResult) =>
         <>
           <p> "You have voted for a project"->restr </p>
           <Rimble.Button
             className=Css.(
               style([
                 display(`block),
                 margin(auto),
                 width(`percent(90.)),
               ])
             )
             onClick={_ => setVoteStep(_ => ViewResults)}>
             "View Results"->restr
           </Rimble.Button>
         </>
       | Failed => <p> "Transaction failed"->restr </p>
       }}
    </div>;

  let (redeemedLoyaltyTokenBalanceBn, _resetLoyaltyTokenBalance) =
    AnimalActions.useUserLoyaltyTokenBalance(userAddressLowerCase);
  let (optCurrentIteration, _resetLoyaltyTokenBalance) =
    AnimalActions.useCurrentIteration();

  let redeemedLoyaltyTokenBalance =
    redeemedLoyaltyTokenBalanceBn->oFlatMap(balance =>
      balance->Web3Utils.fromWeiBNToEthPrecision(~digits=3)->Float.fromString
    )
    |||| 0.;

  let (amountApproved, resetAmountApproved) =
    AnimalActions.useVoteApprovedTokens(userAddressLowerCase);
  let hasApprovedFullBalance =
    amountApproved
    |||| BN.new_("0")
    |>| (
      redeemedLoyaltyTokenBalanceBn |||| BN.new_("10000000000000000000000")
    );

  // TODO: This gets the value from the graph rather - use this in the future rather than querying the chain.
  // let totalLoyaltyTokensOpt =
  //   QlHooks.useTotalLoyaltyToken(userAddressLowerCase);
  // let redeemedLoyaltyTokenBalance: float =
  //   totalLoyaltyTokensOpt->oFlatMap(((_, claimedLoyaltyTokens)) =>
  //     claimedLoyaltyTokens
  //     ->Web3Utils.fromWeiBNToEthPrecision(~digits=3)
  //     ->Float.fromString
  //   )
  //   |||| 0.;

  let cannotVote: bool =
    currentlyOwnedTokens->Array.length <= 0
    || notGoerliNetwork(networkIdOpt |||| (-1));

  let maxVote = Js.Math.sqrt(redeemedLoyaltyTokenBalance);

  <Rimble.Box className=Styles.topBody>
    <Rimble.Box>
      <Rimble.Flex flexWrap="wrap">
        <Rimble.Box width=[|1., 1., 0.3|]>
          <img
            className=Css.(style([maxWidth(`px(800)), margin(auto)]))
            src=dragonImg
          />
          <a href="/#details/Glen">
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
            {currentlyOwnedTokens->Array.length <= 0
               ? <p className=Css.(style([color(red)]))>
                   "You can only vote if you are the owner of a wildcard"
                   ->restr
                 </p>
               : <p>
                   {(
                      "Redeemed loyalty token balance: "
                      ++ {
                        redeemedLoyaltyTokenBalance->Float.toString;
                      }
                      ++ " WLT"
                    )
                    ->restr}
                 </p>}
            {notGoerliNetworkWarning(networkIdOpt)}
          </small>
          <Rimble.Flex flexWrap="wrap" alignItems="center">
            {switch (voteStep) {
             | DefaultView =>
               switch (optCurrentIteration) {
               | Some(currentIteration) =>
                 conservationPartners
                 ->Array.mapWithIndex((index, conservationPartner) =>
                     <OrganisationVote
                       currentUser=userAddressLowerCase
                       currentIteration
                       conservationPartner
                       cannotVote
                       index
                       selectConservation
                     />
                   )
                 ->React.array
               | None => <p> "loading vote data"->restr </p>
               }
             | SelectedOrganisationToVote(
                 conservationVotedArrayIndex,
                 submitVoteFunction,
               ) =>
               let selectedConservationPartner =
                 conservationPartners->Array.getUnsafe(
                   conservationVotedArrayIndex,
                 );
               <>
                 <Rimble.Box width=[|1., 0.25|]>
                   <a href={selectedConservationPartner.link} target="_blank">
                     <img
                       className=Css.(
                         style([
                           display(`block),
                           width(`percent(70.)),
                           maxWidth(`px(800)),
                           margin(auto),
                         ])
                       )
                       src={selectedConservationPartner.image}
                     />
                   </a>
                 </Rimble.Box>
                 <Rimble.Box width=[|1., 0.75|]>
                   {hasApprovedFullBalance
                      ? <QVSelect submitVoteFunction maxVote />
                      : <ApproveLoyaltyTokens
                          reloadFunction=resetAmountApproved
                        />}
                 </Rimble.Box>
               </>;
             | ProcessTransaction => txStateDisplay
             | ViewResults =>
               switch (optCurrentIteration) {
               | Some(currentIteration) => <VoteResults currentIteration />
               | None => <p> "loading vote data"->restr </p>
               }
             }}
          </Rimble.Flex>
        </Rimble.Box>
      </Rimble.Flex>
    </Rimble.Box>
  </Rimble.Box>;
};
