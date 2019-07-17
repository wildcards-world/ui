open Hooks;
open Providers.UsdPriceProvider;
open Belt.Option;

// [@bs.module "../../web3ProvideSwitcher"]
// external toWei: unit => bool = "toWei";
let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));

[@react.component]
let make = () => {
  let (buyAmountEther, setBuyAmountEther) = React.useState(() => "");

  let currentPrice = useCurrentPriceWei();

  let currentUser = useCurrentUser();

  let currentPriceWei = useCurrentPriceWei();

  // TODO: Add this function as part of the "Rimble.Input" component
  let onChangeBuyEther = event => {
    ReactEvent.Form.preventDefault(event);
    let value = ReactEvent.Form.target(event)##value->getWithDefault("");
    setBuyAmountEther(_ => value);
  };

  let buyObj = useBuyTransaction();

  let onSubmitBuy = event => {
    ReactEvent.Form.preventDefault(event);

    let setFunctionObj = [%bs.raw {| (value, from) => ({ value, from }) |}];
    let isUnlocked = Web3Unlocked.unlock();
    let amountToSend =
      BN.new_(buyAmountEther)
      ->BN.addGet(. BN.new_(currentPrice))
      ->BN.toStringGet(.);
    let result =
      buyObj##send(.
        buyAmountEther->Web3Utils.toWeiFromEth,
        setFunctionObj(. amountToSend, currentUser),
      );
    Js.log(result);
    ();
  };

  <div>
    {getToDisplay("useCurrentPriceWei", currentPriceWei)}
    <br />
    {getToDisplay("useCurrentPriceEth", useCurrentPriceEth())}
    <br />
    {getToDisplay("useCurrentPriceUsd", useCurrentPriceUsd())}
    <br />
    {getToDisplay("useCurrentPatron", useCurrentPatron())}
    <br />
    {getToDisplay("useUsdPrice", useUsdPrice()->map(Js.Float.toString))}
    <br />
    {getToDisplay("useTotalPatronageWei", useTotalPatronageWei())}
    <br />
    {getToDisplay("useTotalPatronageEth", useTotalPatronageEth())}
    <br />
    {getToDisplay("useTotalPatronageUsd", useTotalPatronageUsd())}
    <br />
    {getToDisplay(
       "useDepositAbleToWithdrawWei",
       useDepositAbleToWithdrawWei(),
     )}
    <br />
    {getToDisplay(
       "useDepositAbleToWithdrawEth",
       useDepositAbleToWithdrawEth(),
     )}
    <br />
    {getToDisplay(
       "useDepositAbleToWithdrawUsd",
       useDepositAbleToWithdrawUsd(),
     )}
    <br />
    {getToDisplay(
       "useForeclosureTime",
       useForeclosureTime()->map(Js.Date.toString),
     )}
    <br />
    <Rimble.Input
      className="test"
      _type="number"
      placeholder="Your Initial Sale Price"
      onChange=onChangeBuyEther
      value=buyAmountEther
    />
    <Rimble.Button onClick=onSubmitBuy> {React.string("Buy")} </Rimble.Button>
    <br />
  </div>;
};
