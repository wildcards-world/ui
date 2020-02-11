let gorillaOnVine = [%bs.raw
  {|require('../../img/wildcardsimages/gorilla-on-vine.png')|}
];

open Rimble;

[@react.component]
let make = () => {
  <Box className=Styles.infoBackground>
    <Flex flexWrap="wrap">
      <Box width=[|0.28|] className=Styles.animalImage>
        <img src=gorillaOnVine width="100%" />
      </Box>
      <Box width=[|1., 1., 0.59|] className=Styles.infoCardContainer>
        <Card className=Styles.infoCardStyles>
          <MonthlyContribution numberOfLeaders=6 />
        </Card>
      </Box>
    </Flex>
  </Box>;
};
