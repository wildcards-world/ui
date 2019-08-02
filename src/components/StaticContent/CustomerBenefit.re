open Components;

module CustomerBenefit = {
  [@react.component]
  let make = () => {
    <Rimble.Box className=Styles.horizantalBlueTile>
      <p className=Styles.explainerLargeText>
        <span className=Styles.boldExplainerLargeText>
          <S> "Wildcards" </S>
        </span>
        <S>
          " lets you support conservation, become a patron of endangered gorillas and even earn a profit from trading your digital crypto collectable."
        </S>
      </p>
    </Rimble.Box>;
  };
};
