open Globals;

let dragonImg = [%bs.raw {|require('../img/animals/Glen.svg')|}];

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

[@react.component]
let make = () => {
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
            "Quadratic Vote"->restr
          </h3>
          {true
             //userOwnsAWildcard
             ? <small> "Your loyalty token balance:"->restr </small>
             : true
                 //userIsConnectedToGoerli
                 ? <p>
                     "You need to own a wildcard in order to vote"->restr
                   </p>
                 : <p>
                     "This feature is only available on the Goerli test net right now but will be live }once sufficient testing and coverage is accounted for"
                     ->restr
                   </p>}
          <Rimble.Flex flexWrap="wrap">
            <Rimble.Box width=[|0.25|]>
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
              // laSendaVerdeImg
              // greatWhaleConservancyImg
              // darwinAnimalDoctorsImg
              <Rimble.Button
                className=Styles.centerItemsMargin
                onClick={_ => Js.log("vote conservation 1")}>
                "Vote"->restr
              </Rimble.Button>
            </Rimble.Box>
            <Rimble.Box width=[|0.25|]>
              <h3> "Conservation 2"->restr </h3>
              <Rimble.Button
                className=Styles.centerItemsMargin
                onClick={_ => Js.log("vote conservation 2")}>
                "Vote"->restr
              </Rimble.Button>
            </Rimble.Box>
            <Rimble.Box width=[|0.25|]>
              <h3> "Conservation 3"->restr </h3>
              <Rimble.Button
                className=Styles.centerItemsMargin
                onClick={_ => Js.log("vote conservation 3")}>
                "Vote"->restr
              </Rimble.Button>
            </Rimble.Box>
            <Rimble.Box width=[|0.25|]>
              <h3> "Conservation 4"->restr </h3>
              <Rimble.Button
                className=Styles.centerItemsMargin
                onClick={_ => Js.log("vote conservation 4")}>
                "Vote"->restr
              </Rimble.Button>
            </Rimble.Box>
          </Rimble.Flex>
        </Rimble.Box>
      </Rimble.Flex>
    </Rimble.Box>
  </Rimble.Box>;
};