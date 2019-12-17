open DrizzleReact.Hooks;
open Belt.Option;
open Web3Utils;
open Js.Float;
open Providers.UsdPriceProvider;
open BN;

// let useTotalPatronageWeiNew = () => {
//   let totalCollected0 = useGetTotalCollected("0");
//   let patronageOwed0 = useGetPatronageOwed("0");
//   let totalCollected1 = useGetTotalCollected("1");
//   let patronageOwed1 = useGetPatronageOwed("1");

//   let totalCollected2 = useGetTotalCollected("2");
//   let patronageOwed2 = useGetPatronageOwed("2");

//   let totalCollected3 = useGetTotalCollected("3");
//   let patronageOwed3 = useGetPatronageOwed("3");

//   let totalCollected4 = useGetTotalCollected("4");
//   let patronageOwed4 = useGetPatronageOwed("4");

//   let totalCollected5 = useGetTotalCollected("5");
//   let patronageOwed5 = useGetPatronageOwed("5");

//   let totalCollected6 = useGetTotalCollected("6");
//   let patronageOwed6 = useGetPatronageOwed("6");

//   let totalCollected7 = useGetTotalCollected("7");
//   let patronageOwed7 = useGetPatronageOwed("7");

//   let totalCollected9 = useGetTotalCollected("9");
//   let patronageOwed9 = useGetPatronageOwed("9");

//   let totalCollected10 = useGetTotalCollected("10");
//   let patronageOwed10 = useGetPatronageOwed("10");

//   let totalCollected11 = useGetTotalCollected("11");
//   let patronageOwed11 = useGetPatronageOwed("11");

//   let totalCollected12 = useGetTotalCollected("12");
//   let patronageOwed12 = useGetPatronageOwed("12");

//   let totalCollected42 = useGetTotalCollected("42");
//   let patronageOwed42 = useGetPatronageOwed("42");

//   switch (
//     totalCollected0,
//     totalCollected1,
//     totalCollected2,
//     totalCollected3,
//     totalCollected4,
//     totalCollected5,
//     totalCollected6,
//     totalCollected7,
//     totalCollected9,
//     totalCollected10,
//     totalCollected11,
//     totalCollected12,
//     totalCollected42,
//     patronageOwed0,
//     patronageOwed1,
//     patronageOwed2,
//     patronageOwed3,
//     patronageOwed4,
//     patronageOwed5,
//     patronageOwed6,
//     patronageOwed7,
//     patronageOwed9,
//     patronageOwed10,
//     patronageOwed11,
//     patronageOwed12,
//     patronageOwed42,
//   ) {
//   | (
//       Some(totalCollected0),
//       Some(totalCollected1),
//       Some(totalCollected2),
//       Some(totalCollected3),
//       Some(totalCollected4),
//       Some(totalCollected5),
//       Some(totalCollected6),
//       Some(totalCollected7),
//       Some(totalCollected9),
//       Some(totalCollected10),
//       Some(totalCollected11),
//       Some(totalCollected12),
//       Some(totalCollected42),
//       Some(patronageOwed0),
//       Some(patronageOwed1),
//       Some(patronageOwed2),
//       Some(patronageOwed3),
//       Some(patronageOwed4),
//       Some(patronageOwed5),
//       Some(patronageOwed6),
//       Some(patronageOwed7),
//       Some(patronageOwed9),
//       Some(patronageOwed10),
//       Some(patronageOwed11),
//       Some(patronageOwed12),
//       Some(patronageOwed42),
//     ) =>
//     Some(
//       BN.new_("2697680747781582948")
//       ->addGet(. BN.new_(totalCollected0))
//       ->addGet(. BN.new_(totalCollected1))
//       ->addGet(. BN.new_(totalCollected2))
//       ->addGet(. BN.new_(totalCollected3))
//       ->addGet(. BN.new_(totalCollected4))
//       ->addGet(. BN.new_(totalCollected5))
//       ->addGet(. BN.new_(totalCollected6))
//       ->addGet(. BN.new_(totalCollected7))
//       ->addGet(. BN.new_(totalCollected9))
//       ->addGet(. BN.new_(totalCollected10))
//       ->addGet(. BN.new_(totalCollected11))
//       ->addGet(. BN.new_(totalCollected12))
//       ->addGet(. BN.new_(totalCollected42))
//       ->addGet(. BN.new_(patronageOwed0))
//       ->addGet(. BN.new_(patronageOwed1))
//       ->addGet(. BN.new_(patronageOwed2))
//       ->addGet(. BN.new_(patronageOwed3))
//       ->addGet(. BN.new_(patronageOwed4))
//       ->addGet(. BN.new_(patronageOwed5))
//       ->addGet(. BN.new_(patronageOwed6))
//       ->addGet(. BN.new_(patronageOwed7))
//       ->addGet(. BN.new_(patronageOwed9))
//       ->addGet(. BN.new_(patronageOwed10))
//       ->addGet(. BN.new_(patronageOwed11))
//       ->addGet(. BN.new_(patronageOwed12))
//       ->addGet(. BN.new_(patronageOwed42)),
//     )
//   | _ => None
//   };
// };
// let useTotalPatronageTokenWeiNew = tokenId => {
//   let totalCollected =
//     useGetTotalCollected(tokenId)->mapWithDefault("0", a => a)->BN.new_;
//   let patronageOwed =
//     useGetPatronageOwed(tokenId)->mapWithDefault("0", a => a)->BN.new_;

//   totalCollected->addGet(. patronageOwed);
// };
// let useTotalPatronageTokenEthNew = (tokenId: string) =>
//   useTotalPatronageTokenWeiNew(tokenId)->toStringGet(.)->fromWeiToEth;
// let useTotalPatronageTokenUsdNew = (userAddress: string) => {
//   let totalPatronageTokenEth = useTotalPatronageTokenEthNew(userAddress);
//   let currentUsdEthPrice = useUsdPrice();

//   switch (totalPatronageTokenEth, currentUsdEthPrice) {
//   | (totalDeposit, Some(conversion)) =>
//     Some(toFixedWithPrecision(fromString(totalDeposit) *. conversion, 2))
//   | _ => None
//   };
// };

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

// let useCurrentPriceWeiNew = (tokenId: string) => useGetPriceNew(tokenId);
// let useCurrentPriceEthNew = (tokenId: string) =>
//   useCurrentPriceWeiNew(tokenId)
//   ->flatMap(price => Some(fromWeiToEth(price)));
// let useCurrentPriceUsdNew = (tokenId: string) => {
//   let currentPriceEth = useCurrentPriceEthNew(tokenId);
//   let currentUsdEthPrice = useUsdPrice();

//   switch (currentPriceEth, currentUsdEthPrice) {
//   | (Some(price), Some(conversion)) =>
//     Some(toFixedWithPrecision(fromString(price) *. conversion, 2))
//   | _ => None
//   };
// };

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
