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
type organisationIdentifier = int;
type currentVote = {
  vote: BN.bn,
  maxPossibleVote: BN.bn // This should be calculated at the beginning
};
type voteStep =
  | DefaultView // sub-states can either be loading data, ready, or user is not eligible to vote
  | SelectedOrganisationToVote(organisationIdentifier, currentVote)
  | ProcessTransaction
  | ViewResults;

module ApproveLoyaltyTokens = {
  [@react.component]
  let make = () => {
    let (approveLoyaltyTokens, transactionStatus) =
      AnimalActions.useApproveLoyaltyTokens();
    let etherScanUrl = RootProvider.useEtherscanUrl();

    <div>
      {switch (transactionStatus) {
       | UnInitialised =>
         <p>
           <a onClick={_ => {approveLoyaltyTokens()}}>
             "Approve wildcards to spend your tokens"->restr
           </a>
         </p>
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
       | Complete(_txResult) => <p> "You are now ready to vote :)"->restr </p>
       | Failed => <p> "Transaction failed"->restr </p>
       }}
    </div>;
  };
};

[@react.component]
let make = () => {
  let (voteForProject, transactionStatus) = AnimalActions.useVoteForProject();

  let networkIdOpt = RootProvider.useNetworkId();

  let (voteStep, setVoteStep) = React.useState(() => 0);
  let (conservationVotedArrayIndex, setConservationVotedArrayIndex) =
    React.useState(() => 0);

  let nextVoteStep = () => setVoteStep(voteStep => voteStep + 1);

  let selectConservation = conservationArrayIndex => {
    setConservationVotedArrayIndex(conservationArrayIndex);
    nextVoteStep();
  };

  let selectVote: int => unit =
    votes => {
      let conservationVotedContractIndex =
        conservationPartners->Array.getUnsafe(conservationVotedArrayIndex).
          index;
      let votesBn =
        votes->Int.toString->BN.new_->BN.mulGet(. BN.new_("1000000000"));
      voteForProject(conservationVotedContractIndex->string_of_int, votesBn);

      nextVoteStep();
    };
  let selectVoteFloat: float => unit =
    votes => {
      let conservationVotedContractIndex =
        conservationPartners->Array.getUnsafe(conservationVotedArrayIndex).
          index;

      let votesBn =
        (votes *. 1000000000.)->int_of_float->Int.toString->BN.new_;
      voteForProject(conservationVotedContractIndex->string_of_int, votesBn);
      nextVoteStep();
    };

  let resetVoting = () => {
    setVoteStep(_ => 0);
    setConservationVotedArrayIndex(_ => 0);
  };
  let notGoerliNetworkWarning = networkId =>
    switch (networkId) {
    // TODO: this should only show if the user hasn't approved yet.
    | 5 => <ApproveLoyaltyTokens />
    | _ =>
      <p className=Css.(style([color(red)]))>
        "Voting is only available on the Goerli test net right now but will be live once sufficient testing and coverage is accounted for"
        ->restr
      </p>
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
    | Some(currentUser) => currentUser->Js.String.toLowerCase //TODO - check with zuck this cant be a 3box name name
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
       | Complete(_txResult) => <p> "You have voted for a project"->restr </p>
       | Failed => <p> "Transaction failed"->restr </p>
       }}
    </div>;

  let (redeemedLoyaltyTokenBalanceBn, _resetLoyaltyTokenBalance) =
    AnimalActions.useUserLoyaltyTokenBalance(
      currentUser |||| "0x0000000000000000000000000000000000000500",
    );
  let redeemedLoyaltyTokenBalance =
    redeemedLoyaltyTokenBalanceBn->oFlatMap(balance =>
      balance->Web3Utils.fromWeiBNToEthPrecision(~digits=3)->Float.fromString
    )
    |||| 0.;

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
            {voteStep != 0
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
                   "Redeemed loyalty token balance: "->restr
                   {redeemedLoyaltyTokenBalance->Float.toString->restr}
                 </p>}
            {switch (networkIdOpt) {
             | Some(networkId) => notGoerliNetworkWarning(networkId)
             | _ =>
               <p className=Css.(style([color(red)]))>
                 "Voting is only available on the Goerli testnet right now but will be live once sufficient testing and coverage is accounted for"
                 ->restr
               </p>
             }}
          </small>
          <Rimble.Flex flexWrap="wrap" alignItems="center">
            {switch (voteStep) {
             | 0 =>
               conservationPartners
               ->Array.mapWithIndex((index, conservationPartner) =>
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
                         style([
                           display(`block),
                           margin(auto),
                           width(`percent(90.)),
                         ])
                       )
                       disabled=cannotVote
                       onClick={_ => selectConservation(_ => index)}>
                       "Vote"->restr
                     </Rimble.Button>
                   </Rimble.Box>
                 )
               ->React.array
             | 1 =>
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
                   <QVSelect
                     selectVote
                     makeCustomVote=selectVoteFloat
                     maxVote
                   />
                 </Rimble.Box>
               </>;
             | 2 => txStateDisplay
             | _ => React.null
             }}
          </Rimble.Flex>
        </Rimble.Box>
      </Rimble.Flex>
    </Rimble.Box>
  </Rimble.Box>;
};
