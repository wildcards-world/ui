open Hooks;
open Gorilla;
open Providers.UsdPriceProvider;

let useCurrentPatronGorilla = gorilla => {
  switch (getId(gorilla)) {
  | Some(gorillaId) => useCurrentPatronNew(int_of_string(gorillaId))
  | None => useCurrentPatron()
  };
};

let useDepositAbleToWithdrawEthGorilla = gorilla => {
  let patron =
    useCurrentPatronGorilla(gorilla)
    ->Belt.Option.mapWithDefault("0x0", a => a);
  switch (getId(gorilla)) {
  | Some(gorillaId) => useDepositAbleToWithdrawEthNew(patron)
  | None => useDepositAbleToWithdrawEth()
  };
};

let useDepositAbleToWithdrawUsdGorilla = gorilla => {
  let depositeAbleToWithdrawEth = useDepositAbleToWithdrawEthGorilla(gorilla);
  let currentUsdEthPrice = useUsdPrice();
  Js.log(currentUsdEthPrice);
  switch (depositeAbleToWithdrawEth, currentUsdEthPrice) {
  | (Some(deposit), Some(conversion)) =>
    Some(
      Js.Float.toFixedWithPrecision(
        Js.Float.fromString(deposit) *. conversion,
        2,
      ),
    )
  | _ => None
  };
};

let useTotalPatronageEthGorilla = gorilla => {
  switch (getId(gorilla)) {
  | Some(gorillaId) => useTotalPatronageTokenEthNew(gorillaId)
  | None => useTotalPatronageEth()->Belt.Option.mapWithDefault("0", a => a)
  };
};

let useTotalPatronageUsdGorilla = gorilla => {
  let totalPatronange = useTotalPatronageEthGorilla(gorilla);
  let currentUsdEthPrice = useUsdPrice();

  switch (currentUsdEthPrice) {
  | Some(conversion) =>
    Some(
      Js.Float.toFixedWithPrecision(
        Js.Float.fromString(totalPatronange) *. conversion,
        2,
      ),
    )
  | _ => None
  };
};

let useForeclosureTimeGorilla = gorilla =>
  switch (getId(gorilla)) {
  | Some(gorillaId) => useForeclosureTimeNew(gorillaId)
  | None => useForeclosureTime()
  };
