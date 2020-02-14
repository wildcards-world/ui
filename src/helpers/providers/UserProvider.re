type threeBoxImage;
type threeBoxTwitterVerification = {
  username: string,
  proof: string,
  verifiedBy: string,
};
type threeBoxProfile = {
  coverPhoto: option(threeBoxImage),
  description: option(string),
  image: option(threeBoxImage),
  name: option(string),
};
type threeBoxVerifications = {
  did: string,
  twitter: option(threeBoxTwitterVerification),
};
type threeBoxUserInfo = {
  profile: option(threeBoxProfile),
  wildcardsSpace: option(string),
  verifications: option(threeBoxVerifications),
};
type userVerification = {threeBox: threeBoxUserInfo};

type userInfo = {
  userInfo: Js.Dict.t(userVerification),
  update: (string, bool) => unit,
};

[@bs.module "./UserProvider"]
external useUserInfoContext: unit => userInfo = "useUserInfoContext";

type userId =
  | TwitterHandle(string)
  | EthAddress(string);

let useUserNameOrTwitterHandle: string => userId =
  ethAddress => {
    let userContext = useUserInfoContext();
    let ethAddressLower = Js.String.toLowerCase(ethAddress);

    switch (Js.Dict.get(userContext.userInfo, ethAddressLower)) {
    | None =>
      if (ethAddressLower !== "loading") {
        userContext.update(ethAddressLower, false);
      } else {
        ();
      };
      EthAddress(ethAddress);
    | Some(userInfo) =>
      switch (
        userInfo.threeBox.verifications
        ->Belt.Option.flatMap(verification => {verification.twitter})
        ->Belt.Option.flatMap(twitter => {Some(twitter.username)})
      ) {
      | None => EthAddress(ethAddress)
      | Some(twitterUserName) => TwitterHandle(twitterUserName)
      }
    };
  };

let use3BoxUserData: string => option(threeBoxUserInfo) =
  ethAddress => {
    let userContext = useUserInfoContext();
    let ethAddressLower = Js.String.toLowerCase(ethAddress);

    switch (Js.Dict.get(userContext.userInfo, ethAddressLower)) {
    | None => None
    | Some(userInfo) => Some(userInfo.threeBox)
    };
  };

let useIsUserValidated: string => bool =
  ethAddress =>
    switch (useUserNameOrTwitterHandle(ethAddress)) {
    | EthAddress(_) => false
    | TwitterHandle(_) => true
    };

let useUserComponent = user =>
  switch (user) {
  | EthAddress(ethAddress) =>
    <a href={j|https://etherscan.io/address/$ethAddress|j} target="_blank">
      {React.string(Helper.elipsify(ethAddress, 12))}
    </a>
  | TwitterHandle(twitterHandle) =>
    <a href={j|https://twitter.com/$twitterHandle|j} target="_blank">
      {React.string({j|@$twitterHandle|j})}
    </a>
  };

let useUserName = user =>
  switch (user) {
  | EthAddress(ethAddress) => Helper.elipsify(ethAddress, 14)
  | TwitterHandle(twitterHandle) => {j|@$twitterHandle|j}
  };
