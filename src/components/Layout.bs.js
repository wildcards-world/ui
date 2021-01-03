// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Cn from "re-classnames/src/Cn.bs.js";
import * as Css from "bs-css-emotion/src/Css.bs.js";
import * as Dapp from "./Dapp.bs.js";
import * as FAQs from "./StaticContent/FAQs.bs.js";
import * as Team from "./StaticContent/Team.bs.js";
import * as Curry from "bs-platform/lib/es6/curry.js";
import * as React from "react";
import * as Footer from "./StaticContent/Footer.bs.js";
import * as Header from "./Header.bs.js";
import * as Router from "../helpers/Router.bs.js";
import * as Styles from "../Styles.bs.js";
import * as BuyGrid from "./BuyGrid.bs.js";
import * as Globals from "../harberger-lib/Globals.bs.js";
import * as QlHooks from "../harberger-lib/QlHooks.bs.js";
import * as TokenId from "../harberger-lib/TokenId.bs.js";
import * as Partners from "./StaticContent/Partners.bs.js";
import * as VotePage from "../ethTurin/VotePage.bs.js";
import * as InputHelp from "../harberger-lib/InputHelp.bs.js";
import * as RimbleUi from "rimble-ui";
import * as FeaturedIn from "./StaticContent/FeaturedIn.bs.js";
import * as HowItWorks from "./StaticContent/HowItWorks.bs.js";
import * as OrgProfile from "./OrgProfile.bs.js";
import * as Belt_Option from "bs-platform/lib/es6/belt_Option.js";
import * as EmailSignup from "./StaticContent/EmailSignup.bs.js";
import * as ProfileIcon from "../harberger-lib/components/ProfileIcon.bs.js";
import * as UserProfile from "./UserProfile.bs.js";
import * as Web3Connect from "../harberger-lib/components/Web3Connect.bs.js";
import * as Announcement from "./Announcement.bs.js";
import * as LeaderBoards from "./Leaderboards/LeaderBoards.bs.js";
import * as RootProvider from "../harberger-lib/RootProvider.bs.js";
import ReactSwitch from "react-switch";
import * as ArtistProfiles from "./ArtistProfiles.bs.js";
import * as ReactTranslate from "../helpers/providers/ReactTranslate.bs.js";
import * as CustomerBenefit from "./StaticContent/CustomerBenefit.bs.js";
import * as AnimalAnnouncement from "./AnimalAnnouncement.bs.js";
import * as HomepageLeaderBoard from "./StaticContent/HomepageLeaderBoard.bs.js";

var betaBanner = "/img/beta-banner.png";

function Layout$AnimalFocusDetails(Props) {
  var currentAnimal = Props.currentAnimal;
  var showForwardBackButtons = Props.showForwardBackButtons;
  var clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute(undefined);
  var animalDetails = QlHooks.useDetailsPageNextPrevious(Belt_Option.getWithDefault(currentAnimal, TokenId.fromStringUnsafe("0")));
  return React.createElement("div", {
              className: Cn.fromList({
                    hd: Styles.topBody,
                    tl: {
                      hd: Curry._1(Css.style, {
                            hd: Css.position(Css.relative),
                            tl: /* [] */0
                          }),
                      tl: /* [] */0
                    }
                  })
            }, Belt_Option.mapWithDefault(currentAnimal, false, (function (param) {
                    return true;
                  })) ? React.createElement(RimbleUi.Button.Text, {
                    className: Curry._1(Css.style, {
                          hd: Css.zIndex(10000),
                          tl: /* [] */0
                        }),
                    onClick: (function (param) {
                        return Curry._1(clearAndPush, "#" + (
                                    showForwardBackButtons ? "" : "explorer"
                                  ));
                      }),
                    icononly: true,
                    icon: "Close",
                    color: "black",
                    position: "absolute",
                    top: 0,
                    right: 0,
                    size: "large",
                    m: 1
                  }) : null, React.createElement(RimbleUi.Flex, {
                  children: null,
                  flexWrap: "wrap",
                  alignItems: "center"
                }, showForwardBackButtons ? React.createElement("div", {
                        className: Curry._1(Css.style, {
                              hd: Css.width({
                                    NAME: "percent",
                                    VAL: 5
                                  }),
                              tl: /* [] */0
                            })
                      }, React.createElement("span", {
                            className: Styles.carouselArrow(false, true),
                            onClick: (function (param) {
                                return InputHelp.handleMouseEvent((function (param) {
                                              return Curry._1(clearAndPush, "#details/" + TokenId.toString(animalDetails.prev));
                                            }), param);
                              })
                          }, "◄")) : null, React.createElement("div", {
                      className: Curry._1(Css.style, {
                            hd: Css.width(showForwardBackButtons ? ({
                                      NAME: "percent",
                                      VAL: 90
                                    }) : ({
                                      NAME: "percent",
                                      VAL: 100
                                    })),
                            tl: /* [] */0
                          })
                    }, React.createElement(Dapp.make, {
                          currentAnimal: currentAnimal
                        })), showForwardBackButtons ? React.createElement("div", {
                        className: Curry._1(Css.style, {
                              hd: Css.width({
                                    NAME: "percent",
                                    VAL: 5
                                  }),
                              tl: /* [] */0
                            })
                      }, React.createElement("span", {
                            className: Styles.carouselArrow(false, false) + (" " + Curry._1(Css.style, {
                                    hd: Css.width({
                                          NAME: "percent",
                                          VAL: 3
                                        }),
                                    tl: /* [] */0
                                  })),
                            onClick: (function (param) {
                                return InputHelp.handleMouseEvent((function (param) {
                                              return Curry._1(clearAndPush, "#details/" + TokenId.toString(animalDetails.next));
                                            }), param);
                              })
                          }, "►")) : null));
}

var AnimalFocusDetails = {
  make: Layout$AnimalFocusDetails
};

function Layout(Props) {
  var urlState = Router.useUrlState(undefined);
  var clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute(undefined);
  var isExplorer = Router.useIsExplorer(undefined);
  var isDetails = Router.useIsDetails(undefined);
  var isHome = Router.useIsHome(undefined);
  var translationModeContext = ReactTranslate.useTranslationModeContext(undefined);
  var tmp;
  if (typeof urlState === "number") {
    switch (urlState) {
      case /* Team */0 :
          tmp = React.createElement(Team.make, {});
          break;
      case /* IncreaseVoteIteration */1 :
          tmp = null;
          break;
      case /* VotePage */2 :
          tmp = React.createElement(VotePage.make, {
                chain: /* MainnetQuery */2
              });
          break;
      
    }
  } else {
    switch (urlState.TAG | 0) {
      case /* User */0 :
          tmp = React.createElement(UserProfile.make, {
                chain: /* MainnetQuery */2,
                userAddress: urlState._0
              });
          break;
      case /* Artist */1 :
          tmp = React.createElement(ArtistProfiles.make, {
                artistIdentifier: urlState._0
              });
          break;
      case /* Org */2 :
          tmp = React.createElement(OrgProfile.make, {
                orgId: urlState._0
              });
          break;
      case /* Explorer */3 :
          var subState = urlState._1;
          tmp = subState ? React.createElement(Layout$AnimalFocusDetails, {
                  currentAnimal: subState._0,
                  showForwardBackButtons: false
                }) : React.createElement(BuyGrid.make, {
                  wildcardsEdition: urlState._0
                });
          break;
      case /* Leaderboards */4 :
          tmp = React.createElement(RimbleUi.Flex, {
                children: React.createElement(LeaderBoards.make, {
                      leaderboardType: urlState._0
                    }),
                flexWrap: "wrap",
                alignItems: "center",
                className: Curry._1(Css.style, {
                      hd: Css.padding(Css.em(2)),
                      tl: /* [] */0
                    })
              });
          break;
      case /* Home */5 :
          var animalPageState = urlState._0;
          tmp = animalPageState ? React.createElement(Layout$AnimalFocusDetails, {
                  currentAnimal: animalPageState._0,
                  showForwardBackButtons: true
                }) : React.createElement(React.Fragment, {
                  children: null
                }, React.createElement(Layout$AnimalFocusDetails, {
                      currentAnimal: undefined,
                      showForwardBackButtons: false
                    }), React.createElement(FeaturedIn.make, {}), React.createElement(HomepageLeaderBoard.make, {}), React.createElement(CustomerBenefit.make, {}), React.createElement(HowItWorks.make, {}), React.createElement(EmailSignup.make, {}), React.createElement(FAQs.make, {}), React.createElement(Partners.make, {}));
          break;
      
    }
  }
  return React.createElement("div", {
              className: Styles.app
            }, React.createElement("div", {
                  className: Curry._1(Css.style, {
                        hd: Css.minHeight(Css.vh(88)),
                        tl: /* [] */0
                      })
                }, React.createElement(Announcement.make, {
                      announcementBannerColor: "72D6B5",
                      children: React.createElement("span", undefined, React.createElement("a", {
                                className: AnimalAnnouncement.linkToAnimal,
                                href: "/#explorer/2nd-edition"
                              }, Globals.restr("New wildcards")), " launched on the ", React.createElement("a", {
                                className: AnimalAnnouncement.linkToAnimal,
                                href: "https://matic.network",
                                rel: "noopener noreferrer"
                              }, Globals.restr("MATIC network. ")))
                    }), React.createElement("div", {
                      className: Curry._1(Css.style, {
                            hd: Css.position(Css.relative),
                            tl: /* [] */0
                          })
                    }, React.createElement("img", {
                          className: Styles.betaBanner,
                          src: betaBanner
                        })), React.createElement(Header.make, {
                      navItems: [
                        {
                          shouldDisplay: isHome,
                          shouldDisplayMobile: false,
                          component: (function (param, param$1) {
                              return React.createElement("div", {
                                          className: Styles.navListItemToggle
                                        }, React.createElement("span", {
                                              className: Styles.someMarginRight
                                            }, Globals.restr(translationModeContext.translationModeCrypto ? "EXPERT MODE" : "DEFAULT MODE")), React.createElement(ReactSwitch, {
                                              onChange: translationModeContext.setTranslationModeCrypto,
                                              checked: translationModeContext.translationModeCrypto,
                                              onColor: "#6BAD3F",
                                              onHandleColor: "#346D4C",
                                              offHandleColor: "#aaaaaa",
                                              uncheckedIcon: false,
                                              checkedIcon: false,
                                              height: 16,
                                              width: 30,
                                              handleDiameter: 18,
                                              className: Styles.translationSwitch
                                            }));
                            })
                        },
                        {
                          shouldDisplay: !isHome,
                          shouldDisplayMobile: !isHome,
                          component: (function (closeModal, param) {
                              return React.createElement("a", {
                                          className: Styles.navListText,
                                          href: "",
                                          onClick: (function ($$event) {
                                              Curry._1(closeModal, undefined);
                                              $$event.preventDefault();
                                              return Curry._1(clearAndPush, "#");
                                            })
                                        }, Globals.restr("HOME"));
                            })
                        },
                        {
                          shouldDisplay: true,
                          shouldDisplayMobile: true,
                          component: (function (closeModal, param) {
                              return React.createElement("a", {
                                          className: Styles.navListText,
                                          onClick: (function ($$event) {
                                              Curry._1(closeModal, undefined);
                                              $$event.preventDefault();
                                              return Curry._1(clearAndPush, "/#leaderboards/monthly-contribution");
                                            })
                                        }, Globals.restr("LEADERBOARDS"));
                            })
                        },
                        {
                          shouldDisplay: true,
                          shouldDisplayMobile: true,
                          component: (function (closeModal, param) {
                              return React.createElement("a", {
                                          className: Styles.navListText,
                                          onClick: (function ($$event) {
                                              Curry._1(closeModal, undefined);
                                              $$event.preventDefault();
                                              return Curry._1(clearAndPush, "/#dao");
                                            })
                                        }, Globals.restr("DAO"));
                            })
                        },
                        {
                          shouldDisplay: true,
                          shouldDisplayMobile: true,
                          component: (function (closeModal, param) {
                              return React.createElement("a", {
                                          className: Styles.navListText,
                                          href: "https://blog.wildcards.world/",
                                          rel: "noopener noreferrer",
                                          target: "_blank",
                                          onClick: (function (param) {
                                              return Curry._1(closeModal, undefined);
                                            })
                                        }, Globals.restr("BLOG"));
                            })
                        },
                        {
                          shouldDisplay: !isExplorer || isDetails,
                          shouldDisplayMobile: !isExplorer || isDetails,
                          component: (function (closeModal, param) {
                              return React.createElement("div", undefined, React.createElement(RimbleUi.Button, {
                                              className: Styles.whiteText,
                                              children: Globals.restr("VIEW WILDCARDS"),
                                              onClick: (function ($$event) {
                                                  Curry._1(closeModal, undefined);
                                                  $$event.preventDefault();
                                                  return Curry._1(clearAndPush, "#explorer");
                                                })
                                            }));
                            })
                        },
                        {
                          shouldDisplay: true,
                          shouldDisplayMobile: true,
                          component: (function (clickAction, param) {
                              return React.createElement(Web3Connect.make, {
                                          clickAction: clickAction
                                        });
                            })
                        },
                        {
                          shouldDisplay: true,
                          shouldDisplayMobile: true,
                          component: (function (clickAction, isMobile) {
                              return React.createElement(ProfileIcon.make, {
                                          clickAction: clickAction,
                                          isMobile: isMobile
                                        });
                            })
                        }
                      ]
                    }), tmp), React.createElement(Footer.make, {}));
}

var make = Layout;

export {
  betaBanner ,
  AnimalFocusDetails ,
  make ,
  
}
/* Css Not a pure module */
