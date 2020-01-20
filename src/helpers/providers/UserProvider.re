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
type userVerification = {
  twitter: option(string),
  threeBox: threeBoxUserInfo,
};

type userInfo = {
  verifications: Js.Dict.t(userVerification),
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

    switch (Js.Dict.get(userContext.verifications, ethAddressLower)) {
    | None =>
      if (ethAddressLower !== "loading") {
        userContext.update(ethAddressLower, false);
      } else {
        ();
      };
      EthAddress(ethAddress);
    | Some(verification) =>
      switch (verification.twitter) {
      | None => EthAddress(ethAddress)
      | Some(twitterId) =>
        Js.log(ethAddress ++ " has 3box");
        TwitterHandle(twitterId);
      }
    };
  };

let use3BoxUserData: string => option(threeBoxUserInfo) =
  ethAddress => {
    let userContext = useUserInfoContext();
    let ethAddressLower = Js.String.toLowerCase(ethAddress);

    switch (Js.Dict.get(userContext.verifications, ethAddressLower)) {
    | None => None
    | Some(verifications) => Some(verifications.threeBox)
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
      // I couldn't get this to work: http://www.mattsnider.com/css-string-truncation-with-ellipsis/

        {let stringReplace: string => string = [%raw
           {|
              function(longAddress) {
                return longAddress.replace(/(.{7})..+/, "$1...");
              }
            |}
         ];
         React.string({j|$ethAddress|j}->stringReplace)}
      </a>
  | TwitterHandle(twitterHandle) =>
    <a href={j|https://twitter.com/$twitterHandle|j} target="_blank">
      {React.string({j|@$twitterHandle|j})}
    </a>
  };
