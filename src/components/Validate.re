type threeBoxError;
type boxSpaceFunctions = {
  get: (. string) => Promise.Js.t(string, threeBoxError),
  set: (. string, string) => Promise.Js.t(unit, threeBoxError),
  all: (. unit) => Promise.Js.t(string, threeBoxError),
};
type space = {
  syncDone: Promise.Js.t(bool, threeBoxError),
  public: boxSpaceFunctions,
};
type threeBox = {
  syncDone: Promise.Js.t(bool, threeBoxError),
  openSpace: (. string) => Promise.Js.t(space, threeBoxError),
  public: boxSpaceFunctions,
};
type threeBoxStatus =
  | Loading3Box
  | Load3BoxError
  | LoggedIn(threeBox)
  | SyncedBox(threeBox, space)
  | SyncedSpace(threeBox, space);

[@bs.module "3box"]
external openBox:
  (. Web3.ethAddress, RootProviderTypes.rawProvider) =>
  Promise.Js.t(threeBox, threeBoxError) =
  "openBox";

module DisplayInfo = {
  [@react.component]
  let make = (~box, ~space: space) => {
    let (name, setName) = React.useState(_ => "");
    let (twitterNotifications, setTwitterNotifications) =
      React.useState(_ => "");

    React.useEffect(() => {
      box.public.get(. "name")
      ->Promise.Js.toResult
      ->Promise.getOk(name => setName(_ => name));
      None;
    });

    React.useEffect(() => {
      space.public.get(. "twitterNotifications")
      ->Promise.Js.toResult
      ->Promise.getOk(name => setTwitterNotifications(_ => name));
      None;
    });
    <p> {("info" ++ name ++ "  " ++ twitterNotifications)->React.string} </p>;
  };
};
module Input = {
  [@react.component]
  let make = () => {
    let (threeBoxState, setThreeBoxState) = React.useState(() => Loading3Box);
    let optEthereumWallet = RootProvider.useCurrentUser();
    let optWeb3Provider = RootProvider.useWeb3();

    ignore(
      // Using memo here rather than effect so that we don't load 3box unnecessarily.
      React.useMemo2(
        () => {
          switch (optEthereumWallet, optWeb3Provider) {
          | (Some(ethereumWallet), Some(web3Provider)) =>
            openBox(. ethereumWallet, web3Provider.provider)
            ->Promise.Js.toResult
            ->Promise.getOk(threeBoxInstance => {
                setThreeBoxState(_ => LoggedIn(threeBoxInstance));
                Promise.all2(
                  threeBoxInstance.syncDone->Promise.Js.toResult,
                  threeBoxInstance.openSpace(. "wildcards")
                  ->Promise.Js.toResult,
                )
                ->Promise.get(isBoxLoaded => {
                    let (finishedBoxSyncResult, wildcardsSpaceResult) = isBoxLoaded;
                    // setThreeBoxState(_ =>
                    // );
                    let state =
                      switch (finishedBoxSyncResult, wildcardsSpaceResult) {
                      | (Ok(_finishedBoxSync), Ok(wildcardsSpace)) =>
                        wildcardsSpace.syncDone
                        ->Promise.Js.toResult
                        ->Promise.getOk(_isSpaceLoaded => {
                            setThreeBoxState(_ =>
                              SyncedSpace(threeBoxInstance, wildcardsSpace)
                            )
                          });
                        SyncedBox(threeBoxInstance, wildcardsSpace);
                      | _ => Load3BoxError
                      };
                    setThreeBoxState(_ => state);
                  });
              });

            None;
          | _ => None
          }
        },
        (optEthereumWallet, optWeb3Provider),
      ),
    );

    <Rimble.Box p=1>
      {switch (threeBoxState) {
       | Loading3Box =>
         <Rimble.Heading> "Loading 3Box..."->React.string </Rimble.Heading>
       | LoggedIn(_threeBoxInstance) =>
         <Rimble.Heading> "3 box is loaded"->React.string </Rimble.Heading>
       | SyncedBox(_threeBoxInstance, _) =>
         <Rimble.Heading>
           "3 box is synced, loading 'wildcards' space"->React.string
         </Rimble.Heading>
       | SyncedSpace(box, space) =>
         <React.Fragment>
           <Rimble.Heading>
             "3 box and 'wildcards' space is synced"->React.string
           </Rimble.Heading>
           <DisplayInfo box space />
         </React.Fragment>
       | Load3BoxError =>
         <Rimble.Heading>
           "There was an error loading 3 box. Please contact support."
           ->React.string
         </Rimble.Heading>
       }}
    </Rimble.Box>;
  };
};

[@react.component]
let make = () => {
  let goToVerifyUser = RootProvider.useVerifyUser();

  <React.Fragment>
    <Rimble.Box p=1>
      <Rimble.Button onClick={_e => {goToVerifyUser()}}>
        {React.string("Verify")}
      </Rimble.Button>
    </Rimble.Box>
  </React.Fragment>;
};
