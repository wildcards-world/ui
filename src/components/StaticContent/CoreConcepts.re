let gorillaOnVine = [%bs.raw
  {|require('../../img/wildcardsimages/gorilla-on-vine.png')|}
];

open Components;
open Rimble;

module CoreConcepts = {
  [@react.component]
  let make = () =>
    <Box className=Styles.infoBackground>
      <Flex flexWrap="wrap">
        // <Flex flexWrap="wrap" alignItems="center">

          <Box width=[|0.25|]> <img src=gorillaOnVine width="100%" /> </Box>
          <Box width=[|0.60|]>
            <Card className=Styles.infoCardStyles>
              <Heading _as="h2" fontSize=5> <S> "Core Concepts" </S> </Heading>
              <br />
              <hr />
              <br />
              <Heading _as="h3"> <S> "Non-fungible token" </S> </Heading>
              <br />
              <Text>
                <S> "A " </S>
                <strong> <S> "non-fungible token" </S> </strong>
                <S>
                  " is a way to ensure digital assets are unique and easily tradable on a blockchain."
                </S>
              </Text>
              <br />
              <Heading _as="h3"> <S> "Always For Sale" </S> </Heading>
              <br />
              <Text>
                <S>
                  "When an asset is bought a new selling price is stipulated by the buyer. Assets are "
                </S>
                <strong> <S> "always for sale" </S> </strong>
                <S> " ferever!" </S>
              </Text>
              <br />
              <Heading _as="h3"> <S> "Harberger Tax" </S> </Heading>
              <br />
              <Text>
                <S>
                  "When an asset is bought a new selling price is stipulated by the buyer. Assets are "
                </S>
                <strong> <S> "always for sale" </S> </strong>
                <S> " ferever!" </S>
              </Text>
            </Card>
          </Box>
        </Flex>
    </Box>;
};
