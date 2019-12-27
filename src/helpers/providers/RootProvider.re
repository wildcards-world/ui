type rawProvider;
type web3Library = {
  getBalance: (. Web3.ethAddress) => Promise.promise(option(Eth.t)),
  // pollingInterval: ref(int),
};
type injectedType = {isAuthorized: unit => Promise.promise(bool)};
[@bs.module "./web3-react/connectors"]
external injected: injectedType = "injected";

type web3reactContext = {
  active: bool,
  activate: (injectedType, unit => unit, bool) => Promise.promise(unit),
  account: option(Web3.ethAddress),
  library: option(web3Library),
  chainId: option(int),
};
[@bs.module "@web3-react/core"]
external useWeb3React: unit => web3reactContext = "useWeb3React";

module Web3ReactProvider = {
  [@bs.module "@web3-react/core"] [@react.component]
  external make:
    (~getLibrary: rawProvider => web3Library, ~children: React.element) =>
    React.element =
    "Web3ReactProvider";
};
module DrizzleUserAndTranslationProvider = {
  [@bs.module "./DrizzleUserAndTranslationProvider"] [@react.component]
  external make: (~children: React.element) => React.element = "default";
};
[@bs.module "@ethersproject/providers"] [@bs.new]
external createWeb3Provider: rawProvider => web3Library = "Web3Provider";

let getLibrary = provider => {
  let library = createWeb3Provider(provider);

  let setPollingInterval: web3Library => web3Library = [%raw
    lib => "{lib.pollingInterval = 8000; return lib; }"
  ];
  setPollingInterval(library);
};
type rootActions =
  | GoToBuy(Animal.t)
  | ClearNonUrlState
  | LoadAddress(Web3.ethAddress, option(Eth.t));
type nonUrlState =
  | LoginScreen(rootActions)
  | UpdareDepositScreen(Animal.t)
  | UpdarePriceScreen(Animal.t)
  | BuyScreen(Animal.t)
  | NoExtraState;
type ethState =
  | Disconnected
  | Connected(Web3.ethAddress, option(Eth.t));

type state = {
  nonUrlState,
  ethState,
};

let initialState = {nonUrlState: NoExtraState, ethState: Disconnected};

let reducer = (prevState, action) =>
  switch (action) {
  | LoadAddress(address, optBalance) => {
      ...prevState,
      ethState: Connected(address, optBalance),
    }
  | GoToBuy(animal) =>
    switch (prevState.ethState) {
    | Connected(_, _) => {...prevState, nonUrlState: BuyScreen(animal)}
    | Disconnected => {
        ...prevState,
        nonUrlState: LoginScreen(GoToBuy(animal)),
      }
    }
  | ClearNonUrlState => {...prevState, nonUrlState: NoExtraState}
  | _ => prevState
  };

module TestComp = {
  [@react.component]
  let make = () => {
    <div> "hello"->React.string </div>;
  };
};

module RootContext = {
  let context = React.createContext((initialState, a' => ()));
  // Create a provider component
  let make = React.Context.provider(context);

  // Tell bucklescript how to translate props into JS
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
};

module RootWithWeb3 = {
  [@react.component]
  let make = (~children) => {
    let (rootState, dispatch) = React.useReducer(reducer, initialState);
    let context = useWeb3React();

    let (tried, setTried) = React.useState(() => false);
    React.useEffect1(
      () => {
        injected.isAuthorized()
        ->Promise.get(authorised =>
            if (authorised) {
              ignore(
                context.activate(injected, () => (), true)
                // We catch any errors here, and set tried to true to prevent an infinite loop of failing tries!
                ->Promise.Js.catch(_ => {
                    setTried(_ => true);
                    Promise.resolved();
                  }),
              );
              ();
            } else {
              setTried(_ => true);
            }
          );
        None;
      },
      // intentionally only running on mount (make sure it's only mounted once :))
      [|context.activate|],
    );

    // if the connection worked, wait until we get confirmation of that to flip the flag
    React.useEffect2(
      () => {
        !tried && context.active ? setTried(_ => true) : ();

        // context.active ? dispatch()
        None;
      },
      (tried, context.active),
    );

    React.useEffect3(
      () => {
        switch (context.library, context.account) {
        | (Some(library), Some(account)) =>
          library.getBalance(. account)
          ->Promise.Js.catch(_ => {Promise.resolved(None)})
          ->Promise.get(newBalance => {
              dispatch(LoadAddress(account, newBalance))
            });

          None;
        | _ => None
        }
      },
      (context.library, context.account, context.chainId),
    );

    <RootContext value=(rootState, dispatch)>
      <TestComp />
      children
    </RootContext>;
  };
};
let useCurrentUser: unit => option(Web3.ethAddress) =
  () => {
    let (state, _) = React.useContext(RootContext.context);
    switch (state.ethState) {
    | Connected(address, _balance) => Some(address)
    | Disconnected => None
    };
  };
let useIsProviderSelected: unit => bool =
  () => {
    let (state, _) = React.useContext(RootContext.context);
    switch (state.ethState) {
    | Connected(_address, _balance) => true
    | Disconnected => false
    };
  };
let useEthBalance: unit => option(Eth.t) =
  () => {
    let (state, _) = React.useContext(RootContext.context);
    switch (state.ethState) {
    | Connected(_address, balance) => balance
    | Disconnected => None
    };
  };
let useNonUrlState: unit => nonUrlState =
  () => {
    let (state, _) = React.useContext(RootContext.context);
    state.nonUrlState;
  };
let useGoToBuy: (unit, Animal.t) => unit =
  () => {
    let (_, dispatch) = React.useContext(RootContext.context);
    animal => {
      dispatch(GoToBuy(animal));
    };
  };
let useClearNonUrlState: (unit, unit) => unit =
  () => {
    let (_, dispatch) = React.useContext(RootContext.context);
    () => {
      dispatch(ClearNonUrlState);
    };
  };
let useClearNonUrlStateAndPushRoute: (unit, string) => unit =
  () => {
    let clearNonUrlState = useClearNonUrlState();
    url => {
      clearNonUrlState();
      ReasonReactRouter.push(url);
    };
  };
type connector;
let useActivateConnector: (unit, connector) => unit = ((), _connection) => ();

[@react.component]
let make = (~children) => {
  <Web3ReactProvider getLibrary>
    <DrizzleUserAndTranslationProvider>
      <RootWithWeb3> children </RootWithWeb3>
    </DrizzleUserAndTranslationProvider>
  </Web3ReactProvider>;
};
