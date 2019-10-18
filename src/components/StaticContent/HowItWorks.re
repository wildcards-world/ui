let gorillaOnVine = [%bs.raw
  {|require('../../img/wildcardsimages/hanging-gorilla-from-vine.png')|}
];

open Components;
open Rimble;

module YoutubeVid = {
  [@bs.module "./YoutubeVideo.js"] [@react.component]
  external make: unit => React.element = "default";
};

module HowItWorks = {
  [@react.component]
  let make = () => {
    let (isModalOpen, setModalOpen) = React.useState(() => false);

    <Rimble.Box className=Styles.infoBackground>
      <Rimble.Flex flexWrap="wrap" /* alignItems="center"*/>
        // <Rimble.Flex flexWrap="wrap" >

          <Rimble.Box
            width=[|1., 1., 0.59|] className=Styles.infoCardContainer>
            <Rimble.Card className=Styles.infoCardStyles>
              <YoutubeVid />
              <br />
              <Rimble.Box width=[|1.|]>
                <Rimble.Button
                  className=Styles.centerItemsMargin
                  onClick={_ => setModalOpen(_ => true)}>
                  <S> "See More" </S>
                </Rimble.Button>
              </Rimble.Box>
              <Rimble.Modal isOpen=isModalOpen>
                <Rimble.Card
                  className=Styles.infoModal
                  width={Rimble.AnyStr("640px")}
                  p=0>
                  <Rimble.Button.Text
                    icononly=true
                    icon="Close"
                    color="moon-gray"
                    position="absolute"
                    top=0
                    right=0
                    m=3
                    onClick={_ => setModalOpen(_ => false)}
                  />
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
                    <strong>
                      <S> "Here's where it gets interesting." </S>
                    </strong>
                    <S>
                      " When someone purchases your wildcard from you, you will receive the sale price you set for that wildcard. Depending on what sale price you set and the harberger tax you have already paid, you may make a profit. The only constant is that the conservation organisation will continually receive funding based on the current selling price of each wildcard."
                    </S>
                  </Text>
                  <br />
                  <Text>
                    <span className=Styles.redDisclaimer>
                      <S>
                        "The first wildcard, Vitalik, has a 2.5% Harberger tax rate per month. Andy and Simon have a Harberger tax rate of 20% per month. Future wildcards will have different harberger tax rates."
                      </S>
                    </span>
                  </Text>
                </Rimble.Card>
              </Rimble.Modal>
            </Rimble.Card>
          </Rimble.Box>
          <Rimble.Box width=[|0.41|] className=Styles.gorillaImage>
            <img src=gorillaOnVine width="100%" />
          </Rimble.Box>
        </Rimble.Flex>
    </Rimble.Box>;
  };
};
