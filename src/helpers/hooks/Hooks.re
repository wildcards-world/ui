open DrizzleReact.Hooks;

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
