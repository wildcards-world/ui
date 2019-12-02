open Hooks;
open Providers.UsdPriceProvider;

let useCurrentPatronAnimal = animal => {
  switch (Animal.getId(animal)) {
  | Some(animalId) =>
    useCurrentPatronNew(
      Belt.Int.fromString(animalId)->Belt.Option.mapWithDefault(1, a => a),
    )
  | None => useCurrentPatron()
  };
};

let useDepositAbleToWithdrawEthAnimal = animal => {
  let patron =
    useCurrentPatronAnimal(animal)->Belt.Option.mapWithDefault("0x0", a => a);
  switch (Animal.getId(animal)) {
  | Some(_animalId) => useDepositAbleToWithdrawEthNew(patron)
  | None => useDepositAbleToWithdrawEth()
  };
};

let useDepositAbleToWithdrawUsdAnimal = animal => {
  let depositeAbleToWithdrawEth = useDepositAbleToWithdrawEthAnimal(animal);
  let currentUsdEthPrice = useUsdPrice();
  switch (depositeAbleToWithdrawEth, currentUsdEthPrice) {
  | (Some(deposit), Some(conversion)) =>
    Some(
      Js.Float.toFixedWithPrecision(
        Js.Float.fromString(deposit) *. conversion,
        ~digits=2,
      ),
    )
  | _ => None
  };
};

let useTotalPatronageEthAnimal = animal => {
  switch (Animal.getId(animal)) {
  | Some(animalId) => useTotalPatronageTokenEthNew(animalId)
  | None => useTotalPatronageEth()->Belt.Option.mapWithDefault("0", a => a)
  };
};

let useTotalPatronageUsdAnimal = animal => {
  let totalPatronange = useTotalPatronageEthAnimal(animal);
  let currentUsdEthPrice = useUsdPrice();

  switch (currentUsdEthPrice) {
  | Some(conversion) =>
    Some(
      Js.Float.toFixedWithPrecision(
        Js.Float.fromString(totalPatronange) *. conversion,
        ~digits=2,
      ),
    )
  | _ => None
  };
};

let useForeclosureTimeAnimal = animal =>
  switch (Animal.getId(animal)) {
  | Some(animalId) => useForeclosureTimeNew(animalId)
  | None => useForeclosureTime()
  };
