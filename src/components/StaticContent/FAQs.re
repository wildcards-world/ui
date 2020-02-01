open Components;
open Rimble;

// FAQ Styles

let faqExplainerText =
  Css.(style([paddingTop(`px(10)), lineHeight(`rem(1.6))]));

let accordionContent =
  Css.(
    style([
      overflow(hidden),
      transition(~duration=800, ~delay=0, ~timingFunction=ease, "all"),
    ])
  );

let rotate = angle =>
  Css.(
    style([
      transform(`rotate(deg(angle))),
      transition(~duration=400, ~delay=0, ~timingFunction=ease, "all"),
    ])
  );

let accordionSection = Css.(style([display(`flex), flexDirection(column)]));

/* FAQs End */

let inlineStyle = ReactDOMRe.Style.make;

module Chevron = {
  [@react.component]
  let make = (~className) => {
    <svg className height="14px" width="14px" viewBox="0 0 320 512">
      <path
        fill="#777"
        d="M285.476 272.971L91.132 467.314c-9.373 9.373-24.569 9.373-33.941 0l-22.667-22.667c-9.357-9.357-9.375-24.522-.04-33.901L188.505 256 34.484 101.255c-9.335-9.379-9.317-24.544.04-33.901l22.667-22.667c9.373-9.373 24.569-9.373 33.941 0L285.475 239.03c9.373 9.372 9.373 24.568.001 33.941z"
      />
    </svg>;
  };
};

module FAQs = {
  [@react.component]
  let make = () => {
    let (activeIndex, setActiveIndex) = React.useState(() => (-1));
    let toggleAccordion = faqItemIndex => {
      setActiveIndex(_ => faqItemIndex == activeIndex ? (-1) : faqItemIndex);
    };
    <div>
      <Rimble.Box className=Styles.floatingSignupBox>
        <Rimble.Flex
          flexWrap="wrap"
          alignItems="center"
          className=Styles.floatingSignupBoxInner>
          <Rimble.Box width=[|1., 1., 1.|]>
            <Rimble.Card>
              <Heading> <S> "FAQs" </S> </Heading>
              <div className=accordionSection>
                <div onClick={_ => toggleAccordion(0)}>
                  <Text className="accordion-title">
                    <S>
                      "Do the different wildcards represent real animals?"
                    </S>
                    <Chevron
                      className={rotate(activeIndex == 0 ? 90. : 0.)}
                    />
                  </Text>
                </div>
                <div
                  className=accordionContent
                  style={inlineStyle(
                    ~maxHeight={activeIndex == 0 ? "300px" : "0px"},
                    (),
                  )}>
                  <div>
                    <div className=faqExplainerText>
                      <Text>
                        <S>
                          "Yes! but not all of them. Some represent real animals and some like Simon and Andy don't but all the funds raised by the animals go to the conservations."
                        </S>
                      </Text>
                    </div>
                  </div>
                </div>
                <div onClick={_ => toggleAccordion(1)}>
                  <Text className="accordion-title">
                    <S>
                      "How do I know my funds are going to the organization?"
                    </S>
                    <Chevron
                      className={rotate(activeIndex == 1 ? 90. : 0.)}
                    />
                  </Text>
                </div>
                <div
                  className=accordionContent
                  style={inlineStyle(
                    ~maxHeight={activeIndex == 1 ? "300px" : "0px"},
                    (),
                  )}>
                  <div>
                    <div className=faqExplainerText>
                      <Text>
                        <S>
                          "Thanks to the help of the blockchain! Read our blog post here and you and anyone else can follow the public transactions to verify that the funds are going to our conservation partners."
                        </S>
                      </Text>
                    </div>
                  </div>
                </div>
                <div onClick={_ => toggleAccordion(2)}>
                  <Text className="accordion-title">
                    <S> "Can I only buy a wildcard using cryptocurrency?
" </S>
                    <Chevron
                      className={rotate(activeIndex == 2 ? 90. : 0.)}
                    />
                  </Text>
                </div>
                <div
                  className=accordionContent
                  style={inlineStyle(
                    ~maxHeight={activeIndex == 2 ? "300px" : "0px"},
                    (),
                  )}>
                  <div>
                    <div className=faqExplainerText>
                      <Text>
                        <S>
                          "Unfortunately for now this is the only way to purchase a wildcard. You can exchange your fiat currency into Eth through Torus and this will allow you to become the owner of a wildcard.
"
                        </S>
                      </Text>
                    </div>
                  </div>
                </div>
                <div onClick={_ => toggleAccordion(3)}>
                  <Text className="accordion-title">
                    <S> "What is a Non-fungible token?" </S>
                    <Chevron
                      className={rotate(activeIndex == 3 ? 90. : 0.)}
                    />
                  </Text>
                </div>
                <div
                  className=accordionContent
                  style={inlineStyle(
                    ~maxHeight={activeIndex == 3 ? "300px" : "0px"},
                    (),
                  )}>
                  <div>
                    <div className=faqExplainerText>
                      <Text>
                        <S>
                          "A Non-Fungible Token or NFT is a cryptographic token that is unique. Wildcards uses NFT’s to represent digitally unique wild animals.

"
                        </S>
                      </Text>
                    </div>
                  </div>
                </div>
                <div onClick={_ => toggleAccordion(4)}>
                  <Text className="accordion-title">
                    <S> "What does 'always for sale' mean?
" </S>
                    <Chevron
                      className={rotate(activeIndex == 4 ? 90. : 0.)}
                    />
                  </Text>
                </div>
                <div
                  className=accordionContent
                  style={inlineStyle(
                    ~maxHeight={activeIndex == 4 ? "300px" : "0px"},
                    (),
                  )}>
                  <div>
                    <div className=faqExplainerText>
                      <Text>
                        <S>
                          "Always for sale refers to the economic model used on the animals to ensure a recurring revenue generation for conservations. Wildcards are always for sale! This means that as soon as you buy a wildcard you are required to set a selling price that someone can buy it from you for. It’s always for sale. You won’t set the selling price to high because you have to pay a percentage of the selling price in order to own the wildcard.
"
                        </S>
                      </Text>
                    </div>
                  </div>
                </div>
                <div onClick={_ => toggleAccordion(5)}>
                  <Text className="accordion-title">
                    <S> "What happens if my deposit runs out?" </S>
                    <Chevron
                      className={rotate(activeIndex == 5 ? 90. : 0.)}
                    />
                  </Text>
                </div>
                <div
                  className=accordionContent
                  style={inlineStyle(
                    ~maxHeight={activeIndex == 5 ? "300px" : "0px"},
                    (),
                  )}>
                  <div>
                    <div className=faqExplainerText>
                      <Text>
                        <S>
                          "If your deposit runs out then anyone can claim the wildcard for free and set a new selling price.
"
                        </S>
                      </Text>
                    </div>
                  </div>
                </div>
                <div onClick={_ => toggleAccordion(6)}>
                  <Text className="accordion-title">
                    <S>
                      "Can I join wildcards and raise funds for my animal conservation project?
"
                    </S>
                    <Chevron
                      className={rotate(activeIndex == 6 ? 90. : 0.)}
                    />
                  </Text>
                </div>
                <div
                  className=accordionContent
                  style={inlineStyle(
                    ~maxHeight={activeIndex == 6 ? "300px" : "0px"},
                    (),
                  )}>
                  <div>
                    <div className=faqExplainerText>
                      <Text> <S> "" </S> </Text>
                    </div>
                  </div>
                </div>
              </div>
            </Rimble.Card>
          </Rimble.Box>
        </Rimble.Flex>
      </Rimble.Box>
    </div>;
  };
};