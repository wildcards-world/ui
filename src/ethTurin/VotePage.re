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

// Use TODO
// type conservationPartnerType = {
//   name: string,
//   image: string,
//   link: string,
// };

// let conservationPartners: array(conservationPartnerType) = [|
//   {
//     name: "Wild Tomorrow Fund",
//     image: wildTomorrowFundImg,
//     link: "https://www.wildtomorrowfund.org/",
//   },
//   {
//     name: "La Senda Verde",
//     image: laSendaVerdeImg,
//     link: "http://www.sendaverde.org/",
//   },
//   {
//     name: "The Great Whale Conservancy",
//     image: greatWhaleConservancyImg,
//     link: "http://greatwhaleconservancy.org/",
//   },
//   {
//     name: "Darwin Animal Doctors",
//     image: darwinAnimalDoctorsImg,
//     link: "http://darwinanimaldoctors.org/",
//   },
// |];

[@react.component]
let make = () => {
  let networkIdOpt = RootProvider.useNetworkId();

  let (voteStep, setVoteStep) = React.useState(() => 4);
  let (voteValue, setVoteValue) = React.useState(() => 1);
  let (wildTomorrowLead, _setWildTomorrowLead) = React.useState(() => 25);
  let (laSendaVerdeLead, _setLaSendaVerde) = React.useState(() => 25);
  let (whaleConservancyLead, _setWhaleConservancy) = React.useState(() => 25);
  let (darwinAnimalLead, _setDarwinAnimalLead) = React.useState(() => 25);
  let (conservationVoted, setConservationVoted) = React.useState(() => "");

  let nextVoteStep = () => setVoteStep(voteStep => voteStep + 1);

  Js.log2("current vote step", voteStep);
  Js.log2("current vote value", voteValue);

  let selectConservation = conservationName => {
    setConservationVoted(conservationName);
    nextVoteStep();
  };

  let selectVote: int => unit =
    votes =>
      if (votes == (-1)) {
        nextVoteStep();
      } else {
        setVoteValue(_ => votes);
        nextVoteStep();
        nextVoteStep();
      };

  let resetVoting = () => {
    setVoteStep(_ => 0);
    setVoteValue(_ => 1);
    setConservationVoted(_ => "");
  };

  let makeVote: _ => unit =
    () => {
      Js.log("vote made");
      nextVoteStep();
    };

  let notGoerliNetworkWarning = networkId =>
    switch (networkId) {
    | 5 => React.null
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

  let totalLoyaltyTokensOpt =
    QlHooks.useTotalLoyaltyToken(userAddressLowerCase);

  let redeemedLoyaltyTokenBalance: float =
    totalLoyaltyTokensOpt->oFlatMap(((_, claimedLoyaltyTokens)) =>
      claimedLoyaltyTokens
      ->Web3Utils.fromWeiBNToEthPrecision(~digits=3)
      ->Float.fromString
    )
    |||| 0.;

  let customVote: int => unit =
    votes =>
      if (votes < 0) {
        setVoteValue(_ => 0);
      } else if (votes * votes >= redeemedLoyaltyTokenBalance->Float.toInt) {
        setVoteValue(_ =>
          redeemedLoyaltyTokenBalance->Js.Math.sqrt->Float.toInt
        );
      } else {
        setVoteValue(_ => votes);
      };

  let cannotVote: bool =
    currentlyOwnedTokens->Array.length <= 0
    || notGoerliNetwork(networkIdOpt |||| (-1));

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
            {conservationVoted == ""
             || conservationVoted == "The Wild Tomorrow Fund"
               ? <Rimble.Box width=[|1., 0.25|]>
                   <a href="https://www.wildtomorrowfund.org/" target="_blank">
                     <img
                       className=Css.(
                         style([
                           display(`block),
                           width(`percent(70.)),
                           maxWidth(`px(800)),
                           margin(auto),
                         ])
                       )
                       src=wildTomorrowFundImg
                     />
                   </a>
                   {switch (voteStep) {
                    | 0 =>
                      <Rimble.Button
                        className=Css.(
                          style([
                            display(`block),
                            margin(auto),
                            width(`percent(90.)),
                          ])
                        )
                        disabled=cannotVote
                        onClick={_ =>
                          selectConservation(_ => "The Wild Tomorrow Fund")
                        }>
                        "Vote"->restr
                      </Rimble.Button>
                    | 1 => <QVSelect selectVote redeemedLoyaltyTokenBalance />
                    | 2 =>
                      <CustomVote
                        redeemedLoyaltyTokenBalance
                        voteValue
                        customVote
                        makeVote
                      />
                    | 3 => <Rimble.Loader />
                    | 4 =>
                      <p
                        className=Css.(
                          style([textAlign(center), color(teal)])
                        )>
                        {wildTomorrowLead->Int.toString->restr}
                        "% of votes"->restr
                      </p>
                    | _ => React.null
                    }}
                 </Rimble.Box>
               : React.null}
            {conservationVoted == ""
             || conservationVoted == "The Great Whale Conservancy"
               ? <Rimble.Box width=[|1., 0.25|]>
                   <a href="http://greatwhaleconservancy.org/" target="_blank">
                     <img
                       className=Css.(
                         style([
                           display(`block),
                           width(`percent(70.)),
                           maxWidth(`px(800)),
                           margin(auto),
                         ])
                       )
                       src=greatWhaleConservancyImg
                     />
                   </a>
                   {switch (voteStep) {
                    | 0 =>
                      <Rimble.Button
                        className=Css.(
                          style([
                            display(`block),
                            margin(auto),
                            width(`percent(90.)),
                          ])
                        )
                        disabled=cannotVote
                        onClick={_ =>
                          selectConservation(_ =>
                            "The Great Whale Conservancy"
                          )
                        }>
                        "Vote"->restr
                      </Rimble.Button>
                    | 1 => <QVSelect selectVote redeemedLoyaltyTokenBalance />
                    | 2 =>
                      <CustomVote
                        redeemedLoyaltyTokenBalance
                        voteValue
                        customVote
                        makeVote
                      />
                    | 3 => <Rimble.Loader />
                    | 4 =>
                      <p
                        className=Css.(
                          style([textAlign(center), color(teal)])
                        )>
                        {whaleConservancyLead->Int.toString->restr}
                        "% of votes"->restr
                      </p>
                    | _ => React.null
                    }}
                 </Rimble.Box>
               : React.null}
            {conservationVoted == "" || conservationVoted == "La Senda Verde"
               ? <Rimble.Box width=[|1., 0.25|]>
                   <a href="http://www.sendaverde.org/" target="_blank">
                     <img
                       className=Css.(
                         style([
                           display(`block),
                           width(`percent(70.)),
                           maxWidth(`px(800)),
                           margin(auto),
                         ])
                       )
                       src=laSendaVerdeImg
                     />
                   </a>
                   {switch (voteStep) {
                    | 0 =>
                      <Rimble.Button
                        className=Css.(
                          style([
                            display(`block),
                            margin(auto),
                            width(`percent(90.)),
                          ])
                        )
                        disabled=cannotVote
                        onClick={_ =>
                          selectConservation(_ => "La Senda Verde")
                        }>
                        "Vote"->restr
                      </Rimble.Button>
                    | 1 => <QVSelect selectVote redeemedLoyaltyTokenBalance />
                    | 2 =>
                      <CustomVote
                        redeemedLoyaltyTokenBalance
                        voteValue
                        customVote
                        makeVote
                      />
                    | 3 => <Rimble.Loader />
                    | 4 =>
                      <p
                        className=Css.(
                          style([textAlign(center), color(teal)])
                        )>
                        {laSendaVerdeLead->Int.toString->restr}
                        "% of votes"->restr
                      </p>
                    | _ => React.null
                    }}
                 </Rimble.Box>
               : React.null}
            {conservationVoted == ""
             || conservationVoted == "Darwin Animal Doctors"
               ? <Rimble.Box width=[|1., 0.25|]>
                   <a href="http://darwinanimaldoctors.org/" target="_blank">
                     <img
                       className=Css.(
                         style([
                           display(`block),
                           width(`percent(70.)),
                           maxWidth(`px(800)),
                           margin(auto),
                         ])
                       )
                       src=darwinAnimalDoctorsImg
                     />
                   </a>
                   {switch (voteStep) {
                    | 0 =>
                      <Rimble.Button
                        className=Css.(
                          style([
                            display(`block),
                            margin(auto),
                            width(`percent(90.)),
                          ])
                        )
                        disabled=cannotVote
                        onClick={_ =>
                          selectConservation(_ => "Darwin Animal Doctors")
                        }>
                        "Vote"->restr
                      </Rimble.Button>
                    | 1 => <QVSelect selectVote redeemedLoyaltyTokenBalance />
                    | 2 =>
                      <CustomVote
                        redeemedLoyaltyTokenBalance
                        voteValue
                        customVote
                        makeVote
                      />
                    | 3 => <Rimble.Loader />
                    | 4 =>
                      <p
                        className=Css.(
                          style([textAlign(center), color(teal)])
                        )>
                        {darwinAnimalLead->Int.toString->restr}
                        "% of votes"->restr
                      </p>
                    | _ => React.null
                    }}
                 </Rimble.Box>
               : React.null}
          </Rimble.Flex>
        </Rimble.Box>
      </Rimble.Flex>
    </Rimble.Box>
  </Rimble.Box>;
};