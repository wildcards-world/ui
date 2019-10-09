open Components;

module CustomerBenefit = {
  [@react.component]
  let make = () => {
    <Rimble.Box className=Styles.horizantalBlueTile>
      <p className=Styles.explainerLargeText>
        <span className=Styles.boldExplainerText> <S> "Wildcards" </S> </span>
        <S>
          " lets you support conservation, become a patron of endangered animals and even earn a profit since wildcards are always for sale."
        </S>
      </p>
    </Rimble.Box>;
  };
};
