open DrizzleReact.Hooks;
open Belt.Option;
open Web3Utils;
open Js.Float;
open Providers.UsdPriceProvider;
open BN;

let useTotalPatronageWeiNew = () => {
  let totalCollected0 =
    useGetTotalCollected("0")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed0 =
    useGetPatronageOwed("0")->mapWithDefault("0", a => a)->BN.new_;
  let totalCollected1 =
    useGetTotalCollected("1")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed1 =
    useGetPatronageOwed("1")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected3 =
    useGetTotalCollected("3")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed3 =
    useGetPatronageOwed("3")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected2 =
    useGetTotalCollected("2")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed2 =
    useGetPatronageOwed("2")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected4 =
    useGetTotalCollected("4")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed4 =
    useGetPatronageOwed("4")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected5 =
    useGetTotalCollected("5")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed5 =
    useGetPatronageOwed("5")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected6 =
    useGetTotalCollected("6")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed6 =
    useGetPatronageOwed("6")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected7 =
    useGetTotalCollected("7")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed7 =
    useGetPatronageOwed("7")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected9 =
    useGetTotalCollected("9")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed9 =
    useGetPatronageOwed("9")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected10 =
    useGetTotalCollected("10")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed10 =
    useGetPatronageOwed("10")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected11 =
    useGetTotalCollected("11")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed11 =
    useGetPatronageOwed("11")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected12 =
    useGetTotalCollected("12")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed12 =
    useGetPatronageOwed("12")->mapWithDefault("0", a => a)->BN.new_;

  let totalCollected42 =
    useGetTotalCollected("42")->mapWithDefault("0", a => a)->BN.new_;
  let patronageOwed42 =
    useGetPatronageOwed("42")->mapWithDefault("0", a => a)->BN.new_;

  totalCollected0
  ->addGet(. totalCollected1)
  ->addGet(. totalCollected2)
  ->addGet(. totalCollected3)
  ->addGet(. totalCollected4)
  ->addGet(. totalCollected5)
  ->addGet(. totalCollected6)
  ->addGet(. totalCollected7)
  ->addGet(. totalCollected9)
  ->addGet(. totalCollected10)
  ->addGet(. totalCollected11)
  ->addGet(. totalCollected12)
  ->addGet(. totalCollected42)
  ->addGet(. patronageOwed0)
  ->addGet(. patronageOwed1)
  ->addGet(. patronageOwed2)
  ->addGet(. patronageOwed3)
  ->addGet(. patronageOwed4)
  ->addGet(. patronageOwed5)
  ->addGet(. patronageOwed6)
  ->addGet(. patronageOwed7)
  ->addGet(. patronageOwed9)
  ->addGet(. patronageOwed10)
  ->addGet(. patronageOwed11)
  ->addGet(. patronageOwed12)
  ->addGet(. patronageOwed42);
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
    (useCacheCall())(. "WildcardSteward_v0", "currentPatron", tokenId)
    ->Js.Nullable.toOption;
};
let useBuyTransactionNew = () =>
  (useCacheSend())(. "WildcardSteward_v0", "buy");
let useDepositAvailableToWithdrawNew = patron =>
  (useCacheCall())(. "WildcardSteward_v0", "depositAbleToWithdraw", patron)
  ->Js.Nullable.toOption;
// let useAvailableDepositNew = patron =>
//   (useCacheCall())(. "WildcardSteward_v0", "depositAbleToWithdraw", patron);

let useForeclosureTimeNew = id => {
  let date = useGetForeclosureTime(id);
  date->map(stringTimeStamp =>
    MomentRe.momentWithUnix(
      Belt.Int.fromString(stringTimeStamp)
      ->Belt.Option.mapWithDefault(0, a => a),
    )
  );
};
