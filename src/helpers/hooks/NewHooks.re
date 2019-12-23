open DrizzleReact.Hooks;

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
    (useCacheCall())(. "WildcardSteward_v0", "currentPatron", tokenId)
    ->Js.Nullable.toOption;
};
let useBuyTransactionNew = () =>
  (useCacheSend())(. "WildcardSteward_v0", "buy");
let useDepositAvailableToWithdrawNew = patron =>
  (useCacheCall())(. "WildcardSteward_v0", "depositAbleToWithdraw", patron)
  ->Js.Nullable.toOption;
