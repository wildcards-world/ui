open Globals;

[@react.component]
let make = () => {
  let currentUser = RootProvider.useCurrentUser();
  let networkIdOpt = RootProvider.useNetworkId();

  let displayName =
    UserProvider.useDisplayName(
      currentUser->Option.mapWithDefault("loading", a => a),
    );
  let displayNameStr = UserProvider.displayNameToString(displayName);

  let userAddressLowerCase =
    switch (currentUser) {
    | Some(currentUser) => currentUser->Js.String.toLowerCase //TODO - check with zuck this cant be a 3box name name
    | _ => "0x0000000000000000000000000000000000000000"
    };

  let optThreeBoxData = UserProvider.use3BoxUserData(userAddressLowerCase);
  let optProfile = optThreeBoxData >>= (a => a.profile);
  let profileImage: string =
    (
      optProfile
      >>= (a => a.image)
      >>= (img => img->Array.get(0))
      <$> (a => a.contentUrl)
      >>= (content => Js.Dict.get(content, "/"))
    )
    ->Option.mapWithDefault(Blockie.makeBlockie(. userAddressLowerCase), hash =>
        "https://ipfs.infura.io/ipfs/" ++ hash
      );

  let message =
    switch (networkIdOpt, currentUser) {
    | (None, _) => "Connect to network"
    | (Some(_), None) => "Loading user"
    | (Some(_), Some(_)) => displayNameStr
    };

  switch (networkIdOpt) {
  | None => React.null
  | Some(_) =>
    <Rimble.Tooltip message placement="bottom">
      <a href={"/#user/" ++ userAddressLowerCase}>
        <img
          className=Css.(
            style([
              borderRadius(`percent(50.)),
              width(`px(40)),
              height(`px(40)),
              marginLeft(`px(10)),
            ])
          )
          src=profileImage
        />
      </a>
    </Rimble.Tooltip>
  };
};
