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
              <Heading className=Styles.centerText>
                <S> "How it works" </S>
              </Heading>
              <br />
              <hr />
              <Text>
                <S>
                  "You can become the patron of Vitalik the gorilla by purchasing him for his current sale price. Vitalik the gorilla is "
                </S>
                <strong> <S> "always for sale" </S> </strong>
                <S>
                  " since anyone can purchase him at anytime for his current sale price. When someone purchases a wildcard, they are required to immediately set a sale price for that wildcard. This ensures wildcards are "
                </S>
                <strong> <S> "always for sale" </S> </strong>
              </Text>
              <br />
              <Text>
                <S> "As the gaurdian of a wildcard, you pay a " </S>
                <strong> <S> "Harberger tax" </S> </strong>
                <S>
                  " (percentage based fee) on your newly set sale price. Your harberger tax goes towards supporting conservation. "
                </S>
              </Text>
              <br />
              <Text>
                <strong> <S> "Here's where it gets interesting." </S> </strong>
                <S>
                  " When someone purchases your wildcard from you, you will receive the sale price you set for that wildcard. Depending on what sale price you set and the harberger tax you have already paid, you may make a profit. The only constant is that the conservation organisation will continually receive funding based on the current selling price of each wildcard."
                </S>
              </Text>
              <br />
              <Text>
                <span className=Styles.redDisclaimer>
                  <S>
                    "The first wildcard, Vitalik, has a 30% Harberger tax rate. Future wildcards will have different harberger tax rates."
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