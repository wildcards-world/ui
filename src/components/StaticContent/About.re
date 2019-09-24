open Components;

module About = {
  [@react.component]
  let make = () => {
    <Rimble.Box className=Styles.horizantalBlueTile>
      <p className=Styles.explainerMediumText>
        <span className=Styles.boldExplainerText> <S> "Wildcards" </S> </span>
        <S> " is an idea born out of the " </S>
        <a href="https://ethcapetown.com/"> <S> "#ETHCapeTown2019" </S> </a>
        <S> " hackathon which saw team Wildcards as overall " </S>
        <a href="https://devpost.com/software/ethcapetown_wildcards">
          <S> "winners" </S>
        </a>
        <S>
          ". The focus of the project is providing a platform where funds can be generated for endangered animals. The wildcards platform hosts always for sale "
        </S>
        <span> <S> "non-fungible tokens" </S> </span>
        <S> "representing real " </S>
        <span> <S> "endangered animals" </S> </span>
        <S> "." </S>
      </p>
    </Rimble.Box>;
  };
};
