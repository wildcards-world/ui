open DrizzleReact.Hooks;
open Belt.Option;
open Web3Utils;
open Js.Float;
open Providers.UsdPriceProvider;

let useDepositAbleToWithdrawWeiNew = (userAddress: string) =>
  useGetAvailableDeposit(userAddress);
let useDepositAbleToWithdrawEthNew = (userAddress: string) =>
  useDepositAbleToWithdrawWeiNew(userAddress)
  ->flatMap(price => Some(fromWeiToEth(price)));
let useDepositAbleToWithdrawUsdNew = (userAddress: string) => {
  let depositeAbleToWithdrawEth = useDepositAbleToWithdrawEthNew(userAddress);
  let currentUsdEthPrice = useUsdPrice();

  switch (depositeAbleToWithdrawEth, currentUsdEthPrice) {
  | (Some(price), Some(conversion)) =>
    Some(toFixedWithPrecision(fromString(price) *. conversion, 2))
  | _ => None
  };
};

let useCurrentPriceWeiNew = (tokenId: string) => useGetPriceNew(tokenId);
let useCurrentPriceEthNew = (tokenId: string) =>
  useCurrentPriceWeiNew(tokenId)
  ->flatMap(price => Some(fromWeiToEth(price)));
let useCurrentPriceUsdNew = (tokenId: string) => {
  let currentPriceEth = useCurrentPriceEthNew(tokenId);
  let currentUsdEthPrice = useUsdPrice();

  switch (currentPriceEth, currentUsdEthPrice) {
  | (Some(price), Some(conversion)) =>
    Some(toFixedWithPrecision(fromString(price) *. conversion, 2))
  | _ => None
  };
};

let useChangePriceTransactionNew = () =>
  (useCacheSend())(. "VitalikSteward", "changePrice");
// let useExitTransactionNew = () =>
//   (useCacheSend())(. "VitalikSteward", "exit");
let useAddDepositTransactionNew = () =>
  (useCacheSend())(. "VitalikSteward", "depositWei");
let useWithdrawTransactionNew = () =>
  (useCacheSend())(. "VitalikSteward", "withdrawDeposit");
let useCurrentPatronNew: int => option(string) = {
  tokenId => (useCacheCall())(. "VitalikSteward", "currentPatron", tokenId);
};
let useBuyTransactionNew = () => (useCacheSend())(. "VitalikSteward", "buy");
