open DrizzleReact.Hooks;
open BN;
open Belt.Option;
open Web3Utils;
open Js.Float;
open Providers.UsdPriceProvider;

include NewHooks;

let useCurrentUser: unit => option(string) =
  () => {
    let state = useDrizzleState(state => state##accounts);
    state[0];
  };
let useWeb3: unit => Web3.t =
  () => {
    useDrizzle()##drizzle##web3;
  };

let useTotalPatronageWei = () => {
  let totalCollectedOpt =
    useCacheCall((), "VitalikSteward", "totalCollected");
  let patronageOwedOpt = useCacheCall((), "VitalikSteward", "patronageOwed");
  switch (totalCollectedOpt, patronageOwedOpt) {
  | (Some(totalCollected), Some(patronageOwed)) =>
    let totalCollectedBN: BN.bn = BN.new_(totalCollected);
    let patronageOwedBN: BN.bn = BN.new_(patronageOwed);
    Some(totalCollectedBN->addGet(. patronageOwedBN)->toStringGet(.));
  | _ => None
  };
};

let useTotalPatronageEth = (~decimals=18, ()) =>
  useTotalPatronageWei()
  ->flatMap(price =>
      Some(toFixedWithPrecision(fromString(fromWeiToEth(price)), decimals))
    );
let useTotalPatronageUsd = () => {
  let totalPatronageEth = useTotalPatronageEth();
  let currentUsdEthPrice = useUsdPrice();

  switch (totalPatronageEth, currentUsdEthPrice) {
  | (Some(price), Some(conversion)) =>
    Some(toFixedWithPrecision(fromString(price) *. conversion, 2))
  | _ => None
  };
};

let useDepositAbleToWithdrawWei = () =>
  useCacheCall((), "VitalikSteward", "depositAbleToWithdraw");

let useDepositAbleToWithdrawEth = () =>
  useDepositAbleToWithdrawWei()->flatMap(price => Some(fromWeiToEth(price)));
let useDepositAbleToWithdrawUsd = () => {
  let depositeAbleToWithdrawEth = useDepositAbleToWithdrawEth();
  let currentUsdEthPrice = useUsdPrice();

  switch (depositeAbleToWithdrawEth, currentUsdEthPrice) {
  | (Some(price), Some(conversion)) =>
    Some(toFixedWithPrecision(fromString(price) *. conversion, 2))
  | _ => None
  };
};

let useForeclosureTime = () =>
  useCacheCall((), "VitalikSteward", "foreclosureTime")
  ->Belt.Option.map(stringTimeStamp =>
      MomentRe.momentWithUnix(int_of_string(stringTimeStamp))
    );

let useCurrentPriceWei = () => useCacheCall((), "VitalikSteward", "price");
let useCurrentPriceEth = () =>
  useCurrentPriceWei()->flatMap(price => Some(fromWeiToEth(price)));
let useCurrentPriceUsd = () => {
  let currentPriceEth = useCurrentPriceEth();
  let currentUsdEthPrice = useUsdPrice();

  switch (currentPriceEth, currentUsdEthPrice) {
  | (Some(price), Some(conversion)) =>
    Some(toFixedWithPrecision(fromString(price) *. conversion, 2))
  | _ => None
  };
};

let useCurrentPatron: unit => option(string) =
  () => (useCacheCall())(. "ERC721Full", "ownerOf", 42);
// let useTotalTimeHeld = (addressOfUser) =>
//       let currentTimeHeld = parseInt(this.getTimeHeld(props, timeHeldKey)) + (parseInt(date.getTime()/1000) - parseInt(this.getTimeAcquired(props))

let useAvailableDeposit = () =>
  useCacheCall((), "VitalikSteward", "availableDeposit");
let useBuyTransaction = () => (useCacheSend())(. "VitalikSteward", "buy");
let useChangePriceTransaction = () =>
  (useCacheSend())(. "VitalikSteward", "changePrice");
let useExitTransaction = () => (useCacheSend())(. "VitalikSteward", "exit");
let useAddDepositTransaction = () =>
  (useCacheSend())(. "VitalikSteward", "depositWei");
let useWithdrawTransaction = () =>
  (useCacheSend())(. "VitalikSteward", "withdrawDeposit");
