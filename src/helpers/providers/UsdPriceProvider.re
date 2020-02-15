open Async;
open Belt;
open Globals;

type usdPrice = {usdPrice: option(float)};

let initialState = {usdPrice: None};

module PriceProvider = {
  let context: React.Context.t(option(float)) = React.createContext(None);
  let make = React.Context.provider(context);

  /** Tell bucklescript how to translate props into JS */
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
};

[@decco]
[@bs.deriving accessors]
type price = {c: array(string)};
[@decco]
[@bs.deriving accessors]
type ethUsdPrice = {
  [@decco.key "XETHZUSD"]
  ethUsd: option(price),
};
[@decco]
[@bs.deriving accessors]
type krakenPriceResponse = {result: option(ethUsdPrice)};

let getPrice = () =>
  Fetch.(
    {
      let%Async result =
        fetch("https://api.kraken.com/0/public/Ticker?pair=ETHUSD");
      let%Async response = result->Response.json;
      let krakenPriceObj = response->krakenPriceResponse_decode;

      let usdEthPrice =
        switch (krakenPriceObj) {
        | Ok(priceObj) =>
          let getPriceFromArray = a => a->Array.get(0);
          priceObj.result >>= ethUsd <$> c >>= getPriceFromArray;
        | Result.Error(error) =>
          Js.log(error);
          None;
        };

      usdEthPrice->async;
    }
  );

[@react.component]
let make = (~children) => {
  let (etherUsdPrice, setEtherUsdPrice) = React.useState(_ => None);

  React.useEffect0(() => {
    if (etherUsdPrice == None) {
      let _ =
        getPrice()
        ->mapAsync(newPrice =>
            setEtherUsdPrice(_ => newPrice >>= Float.fromString)
          );
      ();
    } else {
      ();
    };
    None;
  });

  <PriceProvider value=etherUsdPrice> children </PriceProvider>;
};

let useUsdPrice = () => React.useContext(PriceProvider.context);
