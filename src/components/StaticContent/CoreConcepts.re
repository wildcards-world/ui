let gorillaOnVine = [%bs.raw
  {|require('../../img/wildcardsimages/gorilla-on-vine.png')|}
];

open Components;
open Rimble;

module CoreConcepts = {
  [@react.component]
  let make = () => {
    open ReactTranslate;
    let usedtranslationModeContext = useTranslationModeContext();
    let translation =
      useTranslate(. usedtranslationModeContext->translationMode);

    <Box className=Styles.infoBackground>
      <Flex flexWrap="wrap">
        // <Flex flexWrap="wrap" alignItems="center">

          <Box width=[|0.28|] className=Styles.gorillaImage>
            <img src=gorillaOnVine width="100%" />
          </Box>
          <Box width=[|1., 1., 0.59|] className=Styles.infoCardContainer>
            <Card className=Styles.infoCardStyles>
              <Heading className=Styles.centerText _as="h2" fontSize=5>
                <S> "Core Concepts" </S>
              </Heading>
              <br />
              <hr />
              <br />
              <Heading _as="h3"> <S> {translation(. "nft")} </S> </Heading>
              <br />
              <Text>
                <S> "A " </S>
                <strong> <S> {translation(. "nft")} </S> </strong>
                <S> {translation(. "whatIsANFT")} </S>
              </Text>
              <br />
              <Heading _as="h3"> <S> "Always for Sale" </S> </Heading>
              <br />
              <Text>
                <S>
                  "When an asset is bought a new selling price is stipulated by the buyer. Assets are "
                </S>
                <strong> <S> "Always for Sale" </S> </strong>
                <S> ", forever!" </S>
              </Text>
              <br />
              <Heading _as="h3">
                <S> {translation(. "harbergerTax")} </S>
              </Heading>
              <br />
              <Text>
                <S> "The owner of an asset pays a " </S>
                <strong> <S> {translation(. "harbergerTax")} </S> </strong>
                <S>
                  "  which is a percentage of the selling price they stipulate."
                </S>
              </Text>
            </Card>
          </Box>
        </Flex>
    </Box>;
  };
};