open DrizzleReact.Hooks;
open Belt.Option;
open Web3Utils;
open Js.Float;
open Providers.UsdPriceProvider;
open BN;

let useTotalPatronageWeiNew = () => {
  let totalCollected1 =
    useGetTotalCollected("0")->mapWithDefault("0", a => a)->BN.new_;
  let totalCollected2 =
    useGetTotalCollected("1")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed1 =
    useGetPatronageOwed("0")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed2 =
    useGetPatronageOwed("1")->mapWithDefault("0", a => a)->BN.new_;

  totalCollected1
  ->addGet(. totalCollected2)
  ->addGet(. patronageOwed1)
  ->addGet(. patronageOwed2);
};
let useTotalPatronageTokenWeiNew = tokenId => {
  let totalCollected =
    useGetTotalCollected(tokenId)->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed =
    useGetPatronageOwed(tokenId)->mapWithDefault("0", a => a)->BN.new_;

  totalCollected->addGet(. patronageOwed);
};
let useTotalPatronageTokenEthNew = (tokenId: string) =>
  useTotalPatronageTokenWeiNew(tokenId)->toStringGet(.)->fromWeiToEth;
let useTotalPatronageTokenUsdNew = (userAddress: string) => {
  let totalPatronageTokenEth = useTotalPatronageTokenEthNew(userAddress);
  let currentUsdEthPrice = useUsdPrice();

  switch (totalPatronageTokenEth, currentUsdEthPrice) {
  | (totalDeposit, Some(conversion)) =>
    Some(toFixedWithPrecision(fromString(totalDeposit) *. conversion, 2))
  | _ => None
  };
};

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
// TODO: this should use a BN not an int (int is bad :D)
let useCurrentPatronNew: int => option(string) = {
  tokenId =>
    (useCacheCall())(. "WildcardSteward_v0", "currentPatron", tokenId);
};
let useBuyTransactionNew = () =>
  (useCacheSend())(. "WildcardSteward_v0", "buy");
let useDepositAvailableToWithdrawNew = patron =>
  (useCacheCall())(. "WildcardSteward_v0", "depositAbleToWithdraw", patron);
// let useAvailableDepositNew = patron =>
//   (useCacheCall())(. "WildcardSteward_v0", "depositAbleToWithdraw", patron);
