let gorillaOnVine = [%bs.raw
  {|require('../../img/wildcardsimages/hanging-gorilla-from-vine.png')|}
];

open Components;

module CoreConcepts = {
  [@react.component]
  let make = () =>
    <Rimble.Box className=Styles.infoBackground>
      <Rimble.Flex flexWrap="wrap">
        <Rimble.Box width=[|0.75|]>
          <Rimble.Card className=Styles.infoCardStyles>
            <h3> <S> "How it works" </S> </h3>
            <hr />
            <p>
              <S> "A " </S>
              <strong> <S> "non-fungible token" </S> </strong>
              <S>
                " is a way to ensure digital assets are unique and easily tradable on a blockchain."
              </S>
            </p>
            <p>
              <span className="red-disclaimer">
                <S>
                  "For the first gorilla, Vitalik, the 30% Harberger tax will flow equally between both the conservation fund and the development of the platform. The future gorillas patronage fees will go directly to the relevant conservations' wallets."
                </S>
              </span>
            </p>
          </Rimble.Card>
        </Rimble.Box>
        <Rimble.Box width=[|0.25|]>
          <img src=gorillaOnVine width="100%" />
        </Rimble.Box>
      </Rimble.Flex>
    </Rimble.Box>;
};
