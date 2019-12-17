open Components;

let uesTotalPatronage = animal => {
  let optTotalPatronageWei = QlHooks.usePrice(animal); //->Web3Utils.fromWeiBNToEth;
  let optCurrentUsdEthPrice = Providers.UsdPriceProvider.useUsdPrice(); //->mapWithDefault(0., a => a);
  // let optCurrentUsdEthPrice = Some(0.5); //->mapWithDefault(0., a => a);

  switch (optTotalPatronageWei) {
  | Some(totalPatronageWei) =>
    let totalPatronageEth =
      totalPatronageWei->BN.toStringGet(.)->Web3Utils.fromWeiToEth;

    let optTotaPatronageUsd =
      optCurrentUsdEthPrice->Belt.Option.flatMap(currentUsdEthPrice =>
        Some(
          Js.Float.toFixedWithPrecision(
            Belt.Float.fromString(totalPatronageEth)
            ->Belt.Option.mapWithDefault(0., a => a)
            *. currentUsdEthPrice,
            ~digits=2,
          ),
        )
      );

    Some((totalPatronageEth, optTotaPatronageUsd));
  | _ => None
  };
};

[@react.component]
let make = (~animal: Animal.t) => {
  let optCurrentPrice = uesTotalPatronage(animal);

  switch (optCurrentPrice) {
  | Some((priceEth, optPriceUsd)) =>
    <React.Fragment>
      <p className={Styles.noMarginTop ++ " " ++ Styles.noMarginBottom}>
        <S> {priceEth ++ " ETH"} </S>
        {switch (optPriceUsd) {
         | Some(priceUsd) =>
           <p className=Styles.noMarginTop>
             <small> <S> {"(" ++ priceUsd ++ " USD)"} </S> </small>
           </p>
         | None => React.null
         }}
      </p>
    </React.Fragment>
  | None => <Rimble.Loader />
  };
};
