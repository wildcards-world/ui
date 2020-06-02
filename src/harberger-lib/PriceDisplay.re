open Globals;

let usePrice = animal => {
  let optPriceWei = QlHooks.usePrice(animal); //->Web3Utils.fromWeiBNToEth;
  let optCurrentUsdEthPrice = UsdPriceProvider.useUsdPrice(); //->mapWithDefault(0., a => a);
  // let optCurrentUsdEthPrice = Some(0.5); //->mapWithDefault(0., a => a);

  switch (optPriceWei) {
  | Price(totalPatronageWei) =>
    let totalPatronageEth =
      totalPatronageWei->BN.toStringGet(.)->Web3Utils.fromWeiToEth;

    let optTotaPatronageUsd =
      optCurrentUsdEthPrice->oFlatMap(currentUsdEthPrice =>
        Some(
          toFixedWithPrecisionNoTrailingZeros(
            Float.fromString(totalPatronageEth)->mapd(0., a => a)
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
let make = (~animal: TokenId.t) => {
  let optCurrentPrice = usePrice(animal);

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
