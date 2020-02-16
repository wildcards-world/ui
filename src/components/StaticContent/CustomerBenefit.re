open Components;

[@react.component]
let make = () => {
  <Rimble.Box className=Styles.horizantalBlueTile>
    <p className=Styles.explainerLargeText>
      <span className=Styles.boldExplainerText> <S> "Wildcards" </S> </span>
      <S> " connects global conservation organisations to users. " </S>
    </p>
    <p className=Styles.explainerMediumText>
      <S>
        "Social, gamified and transparent giving makes wildcards the perfect platfrom to start making your difference."
      </S>
    </p>
  </Rimble.Box>;
};
