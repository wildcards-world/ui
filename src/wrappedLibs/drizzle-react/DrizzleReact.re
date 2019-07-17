module Hooks = {
  [@bs.module "drizzle-react"] [@bs.scope "drizzleReactHooks"]
  external useDrizzleState: ([@bs.uncurry] ('a => 'b)) => 'b = "";

  [@bs.module "drizzle-react"] [@bs.scope "drizzleReactHooks"]
  external useDrizzle: unit => 'drizzle = "";

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
