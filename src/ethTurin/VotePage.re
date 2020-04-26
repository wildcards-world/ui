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

let redeemedLoyaltyTokenBalance = "5";

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

  let (voteStep, setVoteStep) = React.useState(() => 0);
  let (voteValue, setVoteValue) = React.useState(() => 1);
  let (conservationVoted, setConservationVoted) = React.useState(() => "");

  let nextVoteStep = () => setVoteStep(voteStep => voteStep + 1);
  
  let selectConservation = (conservationName) => {
    setConservationVoted(conservationName);
    nextVoteStep();
  };

  let selectVote = votes => {
   if(votes == -1){
      Js.log("custom"->restr);
   }
   else {
      setVoteValue(_ => votes);
      Js.log(votes);
      nextVoteStep();
   }
   };

   let test = () => {
     Js.log(voteValue);
   }
  
  let resetVoting = () => {
    setVoteStep(_ => 0);
    setVoteValue(_ => 1);
    setConservationVoted(_ => "");
  };
  
   let notGoerliNetworkWarning = networkId =>
    switch (networkId) {    
    | 5 => React.null
    | _ => <p className=Css.(style([color(red)]))>
                       "Voting is only available on the Goerli test net right now but will be live once sufficient testing and coverage is accounted for"
                       ->restr
                     </p>
    };

  let _notGoerliNetwork = networkId => networkId != 5;

  let _canVote = _ => true; // if owns a wildcard & on goerli network

 let optCurrentPrice = PriceDisplay.uesPrice(Animal.Glen); //Dont think this is correct 

let (_, _, ratio, _) = Animal.pledgeRate(Animal.Glen);

 let (optMonthlyPledgeEth, optMonthlyPledgeUsd) =
      switch (optCurrentPrice) {
      | Some((priceEth, optPriceUsd)) => (
          Some(
            Js.Float.toString(
              Belt.Float.fromString(priceEth)->Accounting.defaultZeroF
              *. ratio,
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
          <a href="/#details/Glen">
            <h3 className=Css.(style([textAlign(`center)]))>
              "Glen the Dragon from Turin"->restr
            </h3>
          </a>
          <p className=Css.(style([textAlign(`center)]))>"Monthly contribution: "->restr  
          {switch (optMonthlyPledgeEth) {
         | Some(monthlyPledgeEth) => (monthlyPledgeEth ++ " ETH")->restr
         | None => <Rimble.Loader className=Css.(style([margin(auto)]))/>
         }}
        <br />
        <small>
          {switch (optMonthlyPledgeUsd) {
           | Some(monthlyPledgeUsd) =>
              ("(" ++ monthlyPledgeUsd ++ " USD)")->restr
           | None => React.null
           }}
        </small></p>
          
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
           <Rimble.Button
                className=Css.(
                  style([
                    display(`block),
                    margin(auto),
                    width(`percent(90.)),
                  ])
                )                
                onClick={_ => test()}>
                "Test"->restr
              </Rimble.Button>
          <h3 className=Css.(style([textDecoration(`underline)]))>
            "Quadratic Vote    "->restr 
              {
            voteStep != 0 ?            
           <img onClick={_ => resetVoting()} src=refreshImg className=Css.(
                    style([
                      maxHeight(`px(16)),
                      paddingLeft(`rem(1.))                     
                    ])
                  )/>
                   :
            React.null 
          }
          </h3>
          <small>
            {false
               //userOwnsAWildcard
               ? <p>
                   "Redeemed loyalty token balance: "->restr
                   redeemedLoyaltyTokenBalance->restr
                 </p>
               : switch(networkIdOpt){                    
                   | Some(networkId) => 
                      notGoerliNetworkWarning(networkId)
                   | _ => <p className=Css.(style([color(red)]))>
                       "Voting is only available on the Goerli testnet right now but will be live once sufficient testing and coverage is accounted for"
                       ->restr
                     </p>}
                     }
          </small>
        
          <Rimble.Flex flexWrap="wrap" alignItems="center">
          { conservationVoted == "" || conservationVoted ==  "The Wild Tomorrow Fund" ? 
            <Rimble.Box width=[|1., 0.25|]>
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
              { voteStep == 0 ?
              <Rimble.Button
                className=Css.(
                  style([
                    display(`block),
                    margin(auto),
                    width(`percent(90.)),
                  ])
                )
                // disabled=canVote
                onClick={_ => selectConservation(_ => "The Wild Tomorrow Fund")}>
                "Vote"->restr
              </Rimble.Button>
              : voteStep == 1 ?
              <form>
             <select  onChange={{event => selectVote(ReactEvent.Form.target(event)##value)}}>
              <option value="1">"1 Vote = 1 Loyalty Token"->restr</option>
              <option value="2">"2 Votes = 4 Loyalty Token"->restr</option>
              <option value="3">"3 Votes = 9 Loyalty Token"->restr</option>
              <option value="4">"4 Votes = 16 Loyalty Token"->restr</option>
              <option value="5">"5 Votes = 25 Loyalty Token"->restr</option>
              <option value="6">"6 Votes = 36 Loyalty Token"->restr</option>
              <option value="7">"7 Votes = 49 Loyalty Token"->restr</option>
              <option value="8">"8 Votes = 64 Loyalty Token"->restr</option>
              <option value="9">"9 Votes = 81 Loyalty Token"->restr</option>
              <option value="10">"10 Votes = 100 Loyalty Token"->restr</option>
              <option value="-1">"custom"->restr</option>
             </select>
             </form>
               : voteValue == -1 ?
               <p>"this should show"->restr</p>
               : <Rimble.Loader/>
              }
            </Rimble.Box>
            : React.null
              }
            { conservationVoted == "" || conservationVoted ==  "The Great Whale Conservancy" ? 
            <Rimble.Box width=[|1., 0.25|]>
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
              <Rimble.Button
                className=Css.(
                  style([
                    display(`block),
                    margin(auto),
                    width(`percent(90.)),
                  ])
                )
                onClick={_ => selectConservation(_ => "The Great Whale Conservancy")}>
                "Vote"->restr
              </Rimble.Button>
            </Rimble.Box>
            : React.null
            }
              { conservationVoted == "" || conservationVoted ==  "La Senda Verde" ? 
            <Rimble.Box width=[|1., 0.25|]>
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
              <Rimble.Button
                className=Css.(
                  style([
                    display(`block),
                    margin(auto),
                    width(`percent(90.)),
                  ])
                )
                  onClick={_ => selectConservation(_ => "La Senda Verde")}>
                "Vote"->restr
              </Rimble.Button>
            </Rimble.Box>
            : React.null
            }
              { conservationVoted == "" || conservationVoted ==  "Darwin Animal Doctors" ? 
            <Rimble.Box width=[|1., 0.25|]>
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
              <Rimble.Button
                className=Css.(
                  style([
                    display(`block),
                    margin(auto),
                    width(`percent(90.)),
                  ])
                )
                onClick={_ => selectConservation(_ => "Darwin Animal Doctors")}>
                "Vote"->restr
              </Rimble.Button>
            </Rimble.Box>
             : React.null}
          </Rimble.Flex>
        </Rimble.Box>
      </Rimble.Flex>
    </Rimble.Box>
  </Rimble.Box>;
};