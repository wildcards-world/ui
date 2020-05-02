open Globals;

let uesPrice = animal => {
  let optPriceWei = QlHooks.usePrice(animal); //->Web3Utils.fromWeiBNToEth;
  let optCurrentUsdEthPrice = UsdPriceProvider.useUsdPrice(); //->mapWithDefault(0., a => a);
  // let optCurrentUsdEthPrice = Some(0.5); //->mapWithDefault(0., a => a);

  switch (optPriceWei) {
  | Price(totalPatronageWei) =>
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
  | Foreclosed => Some(("0", Some("0")))
  | Loading => None
  };
};

[@react.component]
let make = (~animal: Animal.t) => {
  let optCurrentPrice = uesPrice(animal);

  switch (optCurrentPrice) {
  | Some((priceEth, optPriceUsd)) =>
    <React.Fragment>
      <p className={Styles.noMarginTop ++ " " ++ Styles.noMarginBottom}>
        {{
           priceEth ++ " ETH";
         }
         ->restr}
      </p>
      {switch (optPriceUsd) {
       | Some(priceUsd) =>
         <p className=Styles.noMarginTop>
           <small>
             {{
                "(" ++ priceUsd ++ " USD)";
              }
              ->restr}
           </small>
         </p>
       | None => React.null
       }}
    </React.Fragment>
  | None => <Rimble.Loader />
  };
};
