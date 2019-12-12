open Hooks;
open Providers.UsdPriceProvider;

let useCurrentPatronAnimal = animal => {
  useCurrentPatronNew(
    Belt.Int.fromString(Animal.getId(animal))
    ->Belt.Option.mapWithDefault(1, a => a),
  );
};

let useDepositAbleToWithdrawEthAnimal = animal => {
  let patron =
    useCurrentPatronAnimal(animal)->Belt.Option.mapWithDefault("0x0", a => a);

  useDepositAbleToWithdrawEthNew(patron);
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

let useTotalPatronageEthAnimal = animal =>
  animal->Animal.getId->useTotalPatronageTokenEthNew;

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
  animal->Animal.getId->useForeclosureTimeNew;
