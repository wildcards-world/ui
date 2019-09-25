let gorillaOnVine = [%bs.raw
  {|require('../../img/wildcardsimages/hanging-gorilla-from-vine.png')|}
];

open Components;
open Rimble;

module HowItWorks = {
  [@react.component]
  let make = () =>
    <Rimble.Box className=Styles.infoBackground>
      <Rimble.Flex flexWrap="wrap" /* alignItems="center"*/>
        // <Rimble.Flex flexWrap="wrap" >

          <Rimble.Box
            width=[|1., 1., 0.59|] className=Styles.infoCardContainer>
            <Rimble.Card className=Styles.infoCardStyles>
              <Heading> <S> "How it works" </S> </Heading>
              <br />
              <hr />
              <Text>
                <S>
                  "You can become the patron of a gorilla, Vitalik, by adopting him for his 'for sale' price (for example 1 ETH). Vitalik the gorilla is "
                </S>
                <strong> <S> "always for sale" </S> </strong>
                <S>
                  " which means that when you purchase Vitalik the gorilla, you are required to set the 'for sale' price that someone else can adopt the gorilla from you (for example 5 ETH)."
                </S>
              </Text>
              <br />
              <Text>
                <S>
                  "Through adopting Vitalik the gorilla, you pay an annual "
                </S>
                <strong> <S> "Harberger tax" </S> </strong>
                <S>
                  " of 30% on your newly set sale price (5 ETH). Your harberger tax of 30% goes towards supporting gorilla conservation. "
                </S>
                <strong> <S> "Here's where it gets interesting." </S> </strong>
                <S>
                  " If someone else decides to purchase Vitalik the gorilla from you, they pay your 'for sale' price (of 5 Eth). You benefit from the upside of 4 ETH on the trade but lose patronage of Vitalik the gorilla. The new owner pays an annual harberger tax on the 'for sale' price of 30% which goes towards gorilla conservation."
                </S>
              </Text>
              <br />
              <Text>
                <span className=Styles.redDisclaimer>
                  <S>
                    "For the first gorilla, Vitalik, the 30% Harberger tax will flow equally between both the conservation fund and the development of the platform. The future gorillas patronage fees will go directly to the relevant conservations' wallets."
                  </S>
                </span>
              </Text>
            </Rimble.Card>
          </Rimble.Box>
          <Rimble.Box width=[|0.41|] className=Styles.gorillaImage>
            <img src=gorillaOnVine width="100%" />
          </Rimble.Box>
        </Rimble.Flex>
    </Rimble.Box>;
};
