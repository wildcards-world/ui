open Globals;

let priceWeiToTuple = (wei, optCurrentUsdEthPrice) => {
  let totalPatronageEth = wei->Eth.toFixedWithPrecisionNoTrailingZeros;

  let optTotaPatronageUsd =
    optCurrentUsdEthPrice->Option.map(currentUsdEthPrice =>
      toFixedWithPrecisionNoTrailingZeros(
        Float.fromString(totalPatronageEth)->mapd(0., a => a)
        *. currentUsdEthPrice,
        ~digits=2,
      )
    );

  (totalPatronageEth, optTotaPatronageUsd);
};

let usePrice = animal => {
  let optPriceWei = QlHooks.usePrice(animal); //->Web3Utils.fromWeiBNToEth;
  let optCurrentUsdEthPrice = UsdPriceProvider.useUsdPrice();

  switch (optPriceWei) {
  | Price(totalPatronageWei) =>
    priceWeiToTuple(totalPatronageWei, optCurrentUsdEthPrice)->Some
  | Foreclosed(_) => Some(("0", Some("0")))
  | Loading => None
  };
};

module PurePriceDisplay = {
  [@react.component]
  let make = (~priceEth, ~optPriceUsd) => {
    <>
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
    </>;
  };
};

[@react.component]
let make = (~animal: TokenId.t) => {
  let optCurrentPrice = usePrice(animal);

  switch (optCurrentPrice) {
  | Some((priceEth, optPriceUsd)) => <PurePriceDisplay priceEth optPriceUsd />
  | None => <Rimble.Loader />
  };
};
