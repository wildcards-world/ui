// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Helper from "../../Helper.bs.js";
import * as Js_dict from "bs-platform/lib/es6/js_dict.js";
import * as Belt_Option from "bs-platform/lib/es6/belt_Option.js";
import * as UserProvider from "./UserProvider";

var make = UserProvider.UserInfoProvider;

function useUserInfoContext(prim) {
  return UserProvider.useUserInfoContext();
}

function useDisplayName(ethAddress) {
  var userContext = UserProvider.useUserInfoContext();
  var ethAddressLower = ethAddress.toLowerCase();
  var opt3box = Belt_Option.map(Js_dict.get(userContext.userInfo, ethAddressLower), (function (a) {
          return a.threeBox;
        }));
  var opt3boxName = Belt_Option.flatMap(Belt_Option.flatMap(opt3box, (function (a) {
              return a.profile;
            })), (function (a) {
          return a.name;
        }));
  return Belt_Option.mapWithDefault(opt3boxName, Belt_Option.mapWithDefault(Belt_Option.map(Belt_Option.flatMap(Belt_Option.flatMap(opt3box, (function (a) {
                                return a.verifications;
                              })), (function (a) {
                            return a.twitter;
                          })), (function (a) {
                        return a.username;
                      })), {
                  TAG: /* EthAddress */2,
                  _0: ethAddress
                }, (function (a) {
                    return {
                            TAG: /* TwitterHandle */0,
                            _0: a
                          };
                  })), (function (a) {
                return {
                        TAG: /* ThreeBoxName */1,
                        _0: a
                      };
              }));
}

function use3BoxUserData(ethAddress) {
  var userContext = UserProvider.useUserInfoContext();
  var ethAddressLower = ethAddress.toLowerCase();
  var userInfo = Js_dict.get(userContext.userInfo, ethAddressLower);
  if (userInfo !== undefined) {
    return userInfo.threeBox;
  }
  
}

function useIsUserValidated(ethAddress) {
  var match = useDisplayName(ethAddress);
  switch (match.TAG | 0) {
    case /* TwitterHandle */0 :
        return true;
    case /* ThreeBoxName */1 :
    case /* EthAddress */2 :
        return false;
    
  }
}

function displayNameToString(displayName) {
  switch (displayName.TAG | 0) {
    case /* TwitterHandle */0 :
    case /* ThreeBoxName */1 :
        return displayName._0;
    case /* EthAddress */2 :
        return Helper.elipsifyMiddle(displayName._0, 8, 2);
    
  }
}

export {
  make ,
  useUserInfoContext ,
  useDisplayName ,
  use3BoxUserData ,
  useIsUserValidated ,
  displayNameToString ,
  
}
/* make Not a pure module */
