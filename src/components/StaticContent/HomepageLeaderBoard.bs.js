// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Curry from "rescript/lib/es6/curry.js";
import * as React from "react";
import * as Styles from "../../Styles.bs.js";
import * as RimbleUi from "rimble-ui";
import * as RootProvider from "../../harberger-lib/RootProvider.bs.js";
import * as MonthlyContribution from "../Leaderboards/MonthlyContribution.bs.js";

var gorillaOnVine = "/img/wildcardsimages/gorilla-on-vine.png";

function HomepageLeaderBoard(props) {
  var clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute(undefined);
  return React.createElement(RimbleUi.Box, {
              children: React.createElement(RimbleUi.Flex, {
                    children: null,
                    flexWrap: "wrap"
                  }, React.createElement(RimbleUi.Box, {
                        children: React.createElement("img", {
                              src: gorillaOnVine,
                              width: "100%"
                            }),
                        width: [0.28],
                        className: Styles.animalImage
                      }), React.createElement(RimbleUi.Box, {
                        children: React.createElement(RimbleUi.Card, {
                              className: Styles.infoCardStyles,
                              children: null
                            }, React.createElement(MonthlyContribution.make, {
                                  numberOfLeaders: 6
                                }), React.createElement("br", undefined), React.createElement(RimbleUi.Box, {
                                  children: React.createElement(RimbleUi.Button, {
                                        className: Styles.centerItemsMargin,
                                        children: "View Other Leaderboards",
                                        onClick: (function (e) {
                                            e.preventDefault();
                                            Curry._1(clearAndPush, "/#leaderboards/monthly-contribution");
                                          })
                                      }),
                                  width: [1]
                                })),
                        width: [
                          1,
                          1,
                          0.67
                        ],
                        className: Styles.infoCardContainer
                      })),
              className: Styles.infoBackground
            });
}

var make = HomepageLeaderBoard;

export {
  gorillaOnVine ,
  make ,
}
/* react Not a pure module */
