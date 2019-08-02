let gorillaOnVine = [%bs.raw
  {|require('../../img/wildcardsimages/hanging-gorilla-from-vine.png')|}
];

open Components;

module HowItWorks = {
  [@react.component]
  let make = () =>
    <Rimble.Box className=Styles.infoBackground>
      <Rimble.Flex flexWrap="wrap">
        <Rimble.Box width=[|0.75|]>
          <Rimble.Card className=Styles.infoCardStyles>
            <h3> <S> "How it works" </S> </h3>
            <hr />
            <p>
              <S>
                "You can become the patron of a gorilla, Vitalik, by adopting him for his 'for sale' price (for example 1 ETH). Vitalik the gorilla is <strong>always for sale</strong> which means that when you purchase Vitalik the gorilla, you are required to set the 'for sale' price that someone else can adopt the gorilla from you (for example 5 ETH)."
              </S>
            </p>
            <p>
              <S>
                "Through adopting Vitalik the gorilla, you pay an annual <strong>Harberger tax</strong> of 30% on your newly set sale price (5 ETH). Your harberger tax of 30% goes towards supporting gorilla conservation. <strong>Here's where it gets interesting.</strong> If someone else decides to purchase Vitalik the gorilla from you, they pay your 'for sale' price (of 5 Eth). You benefit from the upside of 4 ETH on the trade but lose patronage of Vitalik the gorilla. The new owner pays an annual harberger tax on the 'for sale' price of 30% which goes towards gorilla conservation."
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
  // <div>
  //   <div>
  //   </div>
  // </div>
};
