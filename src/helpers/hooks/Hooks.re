open DrizzleReact.Hooks;

let useCurrentUser: unit => option(string) =
  () => {
    Some("0x123456");
  };
// let useWeb3: unit => Web3.t =
//   () => {
//     useDrizzle()##drizzle##web3;
//   };
