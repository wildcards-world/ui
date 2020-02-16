open Components;

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
        ". The focus of the project is raising funds for endangered animals."
      </S>
    </p>
  </Rimble.Box>;
};
