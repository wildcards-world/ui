import React, { createContext, useContext, useEffect, useReducer } from "react";
// import { useAppStatus } from "./RootProvider";
import { useCurrentUser } from "./RootProvider.bs";
export const UserInfoContext = createContext("");

const {
  // profileGraphQL,
  getProfile,
  // getProfiles,
  // getConfig,
  getVerifiedAccounts,
  getSpace
} = require("3box/lib/api");
const ADD_TWITTER_HANDLE_LEGACY = "ADD_TWITTER_HANDLE_LEGACY";
const ADD_3BOX_PROFILE = "ADD_3BOX_PROFILE";
const ADD_3BOX_VERIFICATIONS = "ADD_3BOX_VERIFICATIONS";
const emptyProfileData = {
  twitter: undefined,
  threeBox: { profile: undefined, verifications: undefined }
};
function reducer(state, action) {
  const currentAdressData =
    state.verifications[action.ethAddress] || emptyProfileData;
  const test = () => {
    switch (action.type) {
      case ADD_TWITTER_HANDLE_LEGACY:
        return {
          ...state,
          verifications: {
            ...state.verifications,
            [action.ethAddress]: {
              ...currentAdressData,
              twitter: action.data.success
                ? action.data.verification.twitterVerification.handle
                : undefined
            }
          }
        };
      case ADD_3BOX_PROFILE:
        return {
          ...state,
          verifications: {
            ...state.verifications,
            [action.ethAddress]: {
              ...currentAdressData,
              threeBox: {
                ...currentAdressData.threeBox,
                profile: action.data
              }
            }
          }
        };
      case ADD_3BOX_VERIFICATIONS:
        if (!action.data.did) {
          // if it doesn't contain the did, count it as invalid.
          return state;
        }
        return {
          ...state,
          verifications: {
            ...state.verifications,
            [action.ethAddress]: {
              threeBox: {
                ...currentAdressData.threeBox,
                verifications: action.data
              }
            }
          }
        };
      default:
        throw new Error();
    }
  };
  const testResult = test();
  return testResult;
}

export const UserInfoProvider = ({ children }) => {
  const currentUserEthAddress = useCurrentUser();
  const [userProvider, dispatchUserProvider] = useReducer(reducer, {
    verifications: {}
  });

  const updateUserProvider = (currentUserEthAddress, forceReload) => {
    let currentUserEthAddressLower = currentUserEthAddress.toLowerCase();

    // This is a little hack to syncronously make sure that we don't fetch the same details about wildcards twice.
    if (!!window[currentUserEthAddressLower] && !forceReload) {
      return;
    } else {
      window[currentUserEthAddressLower] = true;
    }

    getProfile(currentUserEthAddressLower).then(profile => {
      dispatchUserProvider({
        type: ADD_3BOX_PROFILE,
        ethAddress: currentUserEthAddressLower,
        data: profile
      });
      if (!!profile.name) {
        getVerifiedAccounts(profile).then(verifiedAccounts => {
          console.log("verified accounts!!]", verifiedAccounts);
          dispatchUserProvider({
            type: ADD_3BOX_VERIFICATIONS,
            ethAddress: currentUserEthAddressLower,
            data: verifiedAccounts
          });
        });
      }
    });

    // getSpace(currentUserEthAddressLower, "wildcards").then(profile => {
    //   console.log("wildcards SPACE ", { profile });
    // });

    // TODO: should try to catch any errors here with this request.
    // TODO:: depricate this code ASAP :D
    fetch(`https://wildcards.xyz/verification/${currentUserEthAddress}`).then(
      async result => {
        let resultJson = await result.json();

        dispatchUserProvider({
          type: ADD_TWITTER_HANDLE_LEGACY,
          data: resultJson
        });
      }
    );
  };

  // This code loads the logged in users eth address first.
  useEffect(() => {
    if (!!currentUserEthAddress && currentUserEthAddress !== "") {
      updateUserProvider(currentUserEthAddress);
    }
  }, [currentUserEthAddress]);

  return (
    <UserInfoContext.Provider
      value={{ ...userProvider, update: updateUserProvider }}
    >
      {children}
    </UserInfoContext.Provider>
  );
};

export const useUserInfoContext = () => useContext(UserInfoContext);

export default UserInfoProvider;
