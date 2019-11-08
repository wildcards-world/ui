[@bs.deriving {abstract: light}]
type userVerification = {twitter: option(string)};

[@bs.deriving {abstract: light}]
type userInfo = {
  verifications: Js.Dict.t(userVerification),
  update: string => unit,
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

    switch (Js.Dict.get(userContext->verifications, ethAddressLower)) {
    | None =>
      if (ethAddressLower !== "loading") {
        userContext->update(ethAddressLower);
      } else {
        ();
      };
      EthAddress(ethAddress);
    | Some(verification) =>
      switch (verification->twitter) {
      | None => EthAddress(ethAddress)
      | Some(twitterId) => TwitterHandle(twitterId)
      }
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
  | EthAddress(ethAddress) => React.string(ethAddress)
  | TwitterHandle(twitterHandle) =>
    <a href={j|https://twitter.com/$twitterHandle|j} target="_blank">
      {React.string({j|@$twitterHandle|j})}
    </a>
  };
