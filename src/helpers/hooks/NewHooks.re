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
  (useCacheSend())(. "WildcardSteward_v0", "changePrice");
// let useExitTransactionNew = () =>
//   (useCacheSend())(. "WildcardSteward_v0", "exit");
let useAddDepositTransactionNew = () =>
  (useCacheSend())(. "WildcardSteward_v0", "depositWei");
let useWithdrawTransactionNew = () =>
  (useCacheSend())(. "WildcardSteward_v0", "withdrawDeposit");
let useCurrentPatronNew: int => option(string) = {
  tokenId =>
    (useCacheCall())(. "WildcardSteward_v0", "currentPatron", tokenId);
};
let useBuyTransactionNew = () =>
  (useCacheSend())(. "WildcardSteward_v0", "buy");
