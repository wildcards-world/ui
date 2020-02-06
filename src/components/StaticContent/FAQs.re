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
              <Heading> <S> "Frequently Asked Questions (FAQs)" </S> </Heading>
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
                      <Text className=Styles.colorGreen>
                        <S>
                          "Some wildcards do represent real animals, while other wildcards are symoblic of the coservation efforts of that organisation. When partnering with a conservation organisation, if that organisation has specific ainimals they track and care for, we aim to list those real animals. In other cases, conservation efforts can occur on a more macro level (without specific animals). To raise funds for these efforts we create a fictional animal representative of that organisation. In both cases, an animal will simply represent an organisation, and funds generated from that animal will flow to the organisation it represents. "
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
                      <Text className=Styles.colorGreen>
                        <S>
                          "Our platform uses ethereum (a blockchain), which is essentially a public transaction ledger. This means that every single action happening on this platfrom is completely public, verifiable and auditable. If you would like to learn more, "
                        </S>
                        <a
                          href="https://blog.wildcards.world/where-is-the-money-going/"
                          target="_blank">
                          <S> "read our blog post" </S>
                        </a>
                        <S>
                          "  which details how funds are transparently recieved by our conservation partners."
                        </S>
                      </Text>
                    </div>
                  </div>
                </div>
                <div onClick={_ => toggleAccordion(2)}>
                  <Text className="accordion-title">
                    <S> "Can I only buy a wildcard using cryptocurrency?" </S>
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
                      <Text className=Styles.colorGreen>
                        <S>
                          "Unfortunately, for now cryptocurrency is the only way to buy a wildcard. We are actively working on FIAT currency solutions. In the mean time, we have intergrated with Torus, allowing you to use your credit card easily to buy the required ETH necessary to purchase a wildcard."
                        </S>
                        <a
                          href="https://blog.wildcards.world/how-to-buy-a-wildcard-web3-ethereum/"
                          target="_blank">
                          <S> " Read our guide" </S>
                        </a>
                        <S>
                          " to find out more about how to buy a wildcard."
                        </S>
                      </Text>
                    </div>
                  </div>
                </div>
                <div onClick={_ => toggleAccordion(3)}>
                  <Text className="accordion-title">
                    <S> "What is a non-fungible token?" </S>
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
                      <Text className=Styles.colorGreen>
                        <S>
                          "A non-fungible token (NFT), is simply put, a token that is unique. This is in contrast to bitcoin (which is fungible), as each bitcoin is always equivalent to another one in value. Every wildcard is an NFT representing a unique animal. Since wildcards are non-fungible (unique), every wildcard has a unique history and independant value."
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
                      <Text className=Styles.colorGreen>
                        <S>
                          "Always for sale means exactly what it says, the asset or thing in question can be bought by any person at anytime, and is hence, always for sale. How this functions in practice is very interesting. The owner of the asset is required to always have a listed selling price for this asset at which anyone could buy this asset from them at any point in time. The owner won’t set the selling price too high because they are required to continually pay a percentage of the selling price in order to keep ownership of the asset.
"
                        </S>
                      </Text>
                    </div>
                  </div>
                </div>
                <div onClick={_ => toggleAccordion(5)}>
                  <Text className="accordion-title">
                    <S>
                      "What is a deposit and what happens if my deposit runs out?"
                    </S>
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
                      <Text className=Styles.colorGreen>
                        <S>
                          "Your deposit is used to contiually pay the percentage fee of the selling price you set, in order for you to continue your ownership of the asset. If your deposit runs out then anyone can claim the asset for free and set a new selling price."
                        </S>
                      </Text>
                    </div>
                  </div>
                </div>
                <div onClick={_ => toggleAccordion(6)}>
                  <Text className="accordion-title">
                    <S>
                      "Can I join wildcards and raise funds for my animal conservation project?"
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
                      <Text className=Styles.colorGreen>
                        <S>
                          "Yes! Please send an email to jonjon@wildcards.world"
                        </S>
                      </Text>
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