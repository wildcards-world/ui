open Components;
open Rimble;

module FAQs = {
  [@react.component]
  let make = () => {
    <div>
      <Rimble.Box className=Styles.floatingSignupBox>
        <Rimble.Flex
          flexWrap="wrap"
          alignItems="center"
          className=Styles.floatingSignupBoxInner>
          <Rimble.Box width=[|1., 1., 1.|]>
            <Rimble.Card>
              <Heading> <S> "FAQs" </S> </Heading>
              <Text>
                <S> "Do the different wildcards represent real animals?" </S>
              </Text>
              <Text>
                <S>
                  "Some of the wildcards represent real animals and some don't but all the funds raised by the animals go to the conservations"
                </S>
              </Text>
            </Rimble.Card>
          </Rimble.Box>
        </Rimble.Flex>
      </Rimble.Box>
    </div>;
  };
};