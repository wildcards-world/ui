open Components;

[@react.component]
let make = (~animal: Animal.t) => {
  let currentPrice = Animal.useCurrentPriceEth(animal);
  let currentPriceUsd = Animal.useCurrentPriceUsd(animal);

  <React.Fragment>
    <p className={Styles.noMarginTop ++ " " ++ Styles.noMarginBottom}>
      <S> {currentPrice ++ " ETH"} </S>
    </p>
    <p className=Styles.noMarginTop>
      <small> <S> {"(" ++ currentPriceUsd ++ " USD)"} </S> </small>
    </p>
  </React.Fragment>;
};
