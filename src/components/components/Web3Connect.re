module UserComponent = {
  [@react.component]
  let make = (~currentUser) => {
    let userId = UserProvider.useUserNameOrTwitterHandle(currentUser);
    let userIdComponent = UserProvider.useUserComponent(userId);

    userIdComponent;
  };
};
[@react.component]
let make = () => {
  let connectWeb3 = RootProvider.useConnectWeb3();
  let networkId = RootProvider.useNetworkId();
  let currentUser = RootProvider.useCurrentUser();

  let connected = message =>
    switch (currentUser) {
    | Some(currentUser) =>
      <div> <p> message </p> <UserComponent currentUser /> </div>

    | None =>
      <div>
        <p> message </p>
        <Rimble.Button onClick={_e => {connectWeb3()}}>
          {React.string("Connect User")}
        </Rimble.Button>
      </div>
    };

  <div className=Styles.loginButton>
    {switch (networkId) {
     | Some(1) =>
       connected(
         <React.Fragment>
           "connected to "->React.string
           <strong> "MAINNET"->React.string </strong>
         </React.Fragment>,
       )
     | Some(5) =>
       connected(
         <React.Fragment>
           "connected to "->React.string
           <strong> "TESTNET"->React.string </strong>
         </React.Fragment>,
       )
     | Some(_) =>
       <div>
         <p> "UNKNOWN Network"->React.string </p>
         <p> "please use MAINNET or goerli testnet"->React.string </p>
       </div>
     | None =>
       <Rimble.Button
         onClick={_e => {
           ReasonReactRouter.push("#");
           connectWeb3();
         }}>
         {React.string("Connect")}
       </Rimble.Button>
     }}
  </div>;
};
