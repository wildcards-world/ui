module Hooks = {
  [@bs.module "@drizzle/react-plugin"] [@bs.scope "drizzleReactHooks"]
  external useDrizzleState: ([@bs.uncurry] ('a => 'b)) => 'b = "";

  [@bs.module "@drizzle/react-plugin"] [@bs.scope "drizzleReactHooks"]
  external useDrizzle: unit => 'drizzle = "";

  // [@bs.module "./jsDrizzleReactHooks.js"]
  // external useGetPriceNew: string => option(string) = "useGetPrice";

  [@bs.module "./jsDrizzleReactHooks.js"]
  external useUserBalance: unit => option(string) = "useUserBalance";
  [@bs.module "./jsDrizzleReactHooks.js"]
  external useGetTotalCollected: string => option(string) =
    "useGetTotalCollected";
  [@bs.module "./jsDrizzleReactHooks.js"]
  external useGetPatronageOwed: string => option(string) =
    "useGetPatronageOwed";

  [@bs.module "./jsDrizzleReactHooks.js"]
  external useGetAvailableDeposit: string => option(string) =
    "useGetAvailableDeposit";

  [@bs.module "./jsDrizzleReactHooks.js"]
  external useGetForeclosureTime: string => option(string) =
    "useGetForeclosureTime";

  // TODO: Need to make this return a `some` since drizzle has a bug where if the contract isn't initialised then it throws an error!
  let useCacheCall = () => useDrizzle()##useCacheCall;

  let useCacheSend = () => useDrizzle()##useCacheSend;
  // let useCacheCall1 = () => useDrizzle()##useCacheCall;
  // WIP: https://reasonml.github.io/docs/en/exception
  // let useCacheCall = (contract, function_, args) =>
  //   try (useDrizzle()##useCacheCall(contract, function_, args)) {
  //   | Not_found => None /* Default value if getItem throws */
  //   };
};
