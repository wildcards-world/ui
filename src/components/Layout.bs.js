// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE

import * as Cn from "re-classnames/src/Cn.bs.js";
import * as Css from "bs-css-emotion/src/Css.js";
import * as Curry from "bs-platform/lib/es6/curry.js";
import * as React from "react";
import * as RimbleUi from "rimble-ui";
import * as ReactSwitch from "react-switch";
import * as Dapp$WildCards from "./Dapp.bs.js";
import * as Footer$WildCards from "./StaticContent/Footer.bs.js";
import * as Header$WildCards from "./Header.bs.js";
import * as Router$WildCards from "../helpers/Router.bs.js";
import * as Styles$WildCards from "../Styles.bs.js";
import * as GSNTest$WildCards from "../GSNTest.bs.js";
import * as Globals$WildCards from "../harberger-lib/Globals.bs.js";
import * as QlHooks$WildCards from "../harberger-lib/QlHooks.bs.js";
import * as TokenId$WildCards from "../harberger-lib/TokenId.bs.js";
import * as InputHelp$WildCards from "../harberger-lib/InputHelp.bs.js";
import * as ProfileIcon$WildCards from "../harberger-lib/components/ProfileIcon.bs.js";
import * as Web3Connect$WildCards from "../harberger-lib/components/Web3Connect.bs.js";
import * as Announcement$WildCards from "./Announcement.bs.js";
import * as RootProvider$WildCards from "../harberger-lib/RootProvider.bs.js";
import * as ReactTranslate$WildCards from "../helpers/providers/ReactTranslate.bs.js";

var betaBanner = "/img/beta-banner.png";

function Layout$AnimalFocusDetails(Props) {
  var currentAnimal = Props.currentAnimal;
  var showForwardBackButtons = Props.showForwardBackButtons;
  var clearAndPush = RootProvider$WildCards.useClearNonUrlStateAndPushRoute(undefined);
  var animalDetails = QlHooks$WildCards.useDetailsPageNextPrevious(Globals$WildCards.$pipe$pipe$pipe$pipe(currentAnimal, TokenId$WildCards.fromStringUnsafe("0")));
  return React.createElement("div", {
              className: Cn.make(/* :: */[
                    Styles$WildCards.topBody,
                    /* :: */[
                      Curry._1(Css.style, /* :: */[
                            Css.position(Css.relative),
                            /* [] */0
                          ]),
                      /* [] */0
                    ]
                  ])
            }, Globals$WildCards.mapd(currentAnimal, false, (function (param) {
                    return true;
                  })) ? React.createElement(RimbleUi.Button.Text, {
                    className: Curry._1(Css.style, /* :: */[
                          Css.zIndex(10000),
                          /* [] */0
                        ]),
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
                        className: Curry._1(Css.style, /* :: */[
                              Css.width(/* `percent */[
                                    -119887163,
                                    5
                                  ]),
                              /* [] */0
                            ])
                      }, React.createElement("span", {
                            className: Styles$WildCards.carouselArrow(false, true),
                            onClick: (function (param) {
                                return InputHelp$WildCards.handleMouseEvent((function (param) {
                                              return Curry._1(clearAndPush, "#details/" + TokenId$WildCards.toString(animalDetails.prev));
                                            }), param);
                              })
                          }, "◄")) : null, React.createElement("div", {
                      className: Curry._1(Css.style, /* :: */[
                            Css.width(showForwardBackButtons ? /* `percent */[
                                    -119887163,
                                    90
                                  ] : /* `percent */[
                                    -119887163,
                                    100
                                  ]),
                            /* [] */0
                          ])
                    }, React.createElement(Dapp$WildCards.make, { })), showForwardBackButtons ? React.createElement("div", {
                        className: Curry._1(Css.style, /* :: */[
                              Css.width(/* `percent */[
                                    -119887163,
                                    5
                                  ]),
                              /* [] */0
                            ])
                      }, React.createElement("span", {
                            className: Styles$WildCards.carouselArrow(false, false) + (" " + Curry._1(Css.style, /* :: */[
                                    Css.width(/* `percent */[
                                          -119887163,
                                          3
                                        ]),
                                    /* [] */0
                                  ])),
                            onClick: (function (param) {
                                return InputHelp$WildCards.handleMouseEvent((function (param) {
                                              return Curry._1(clearAndPush, "#details/" + TokenId$WildCards.toString(animalDetails.next));
                                            }), param);
                              })
                          }, "►")) : null));
}

var AnimalFocusDetails = {
  make: Layout$AnimalFocusDetails
};

function Layout(Props) {
  Router$WildCards.useUrlState(undefined);
  var clearAndPush = RootProvider$WildCards.useClearNonUrlStateAndPushRoute(undefined);
  var isExplorer = Router$WildCards.useIsExplorer(undefined);
  var isDetails = Router$WildCards.useIsDetails(undefined);
  var isHome = Router$WildCards.useIsHome(undefined);
  var translationModeContext = ReactTranslate$WildCards.useTranslationModeContext(undefined);
  return React.createElement("div", {
              className: Styles$WildCards.app
            }, React.createElement("div", {
                  className: Curry._1(Css.style, /* :: */[
                        Css.minHeight(Css.vh(88)),
                        /* [] */0
                      ])
                }, React.createElement(Announcement$WildCards.make, {
                      announcementBannerColor: "72D6B5",
                      children: React.createElement("span", undefined, "Due to the congestion on the Ethereum network, the next wildcards release will be delayed")
                    }), React.createElement("div", {
                      className: Curry._1(Css.style, /* :: */[
                            Css.position(Css.relative),
                            /* [] */0
                          ])
                    }, React.createElement("img", {
                          className: Styles$WildCards.betaBanner,
                          src: betaBanner
                        })), React.createElement(Header$WildCards.make, {
                      navItems: [
                        {
                          shouldDisplay: isHome,
                          shouldDisplayMobile: false,
                          component: (function (param, param$1) {
                              return React.createElement("div", {
                                          className: Styles$WildCards.navListItemToggle
                                        }, React.createElement("span", {
                                              className: Styles$WildCards.someMarginRight
                                            }, Globals$WildCards.restr(translationModeContext.translationModeCrypto ? "EXPERT MODE" : "DEFAULT MODE")), React.createElement(ReactSwitch.default, {
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
                                              className: Styles$WildCards.translationSwitch
                                            }));
                            })
                        },
                        {
                          shouldDisplay: !isHome,
                          shouldDisplayMobile: !isHome,
                          component: (function (closeModal, param) {
                              return React.createElement("a", {
                                          className: Styles$WildCards.navListText,
                                          href: "",
                                          onClick: (function ($$event) {
                                              Curry._1(closeModal, undefined);
                                              $$event.preventDefault();
                                              return Curry._1(clearAndPush, "#");
                                            })
                                        }, Globals$WildCards.restr("HOME"));
                            })
                        },
                        {
                          shouldDisplay: true,
                          shouldDisplayMobile: true,
                          component: (function (closeModal, param) {
                              return React.createElement("a", {
                                          className: Styles$WildCards.navListText,
                                          onClick: (function ($$event) {
                                              Curry._1(closeModal, undefined);
                                              $$event.preventDefault();
                                              return Curry._1(clearAndPush, "/#leaderboards/monthly-contribution");
                                            })
                                        }, Globals$WildCards.restr("LEADERBOARDS"));
                            })
                        },
                        {
                          shouldDisplay: true,
                          shouldDisplayMobile: true,
                          component: (function (closeModal, param) {
                              return React.createElement("a", {
                                          className: Styles$WildCards.navListText,
                                          onClick: (function ($$event) {
                                              Curry._1(closeModal, undefined);
                                              $$event.preventDefault();
                                              return Curry._1(clearAndPush, "/#dao");
                                            })
                                        }, Globals$WildCards.restr("DAO"));
                            })
                        },
                        {
                          shouldDisplay: true,
                          shouldDisplayMobile: true,
                          component: (function (closeModal, param) {
                              return React.createElement("a", {
                                          className: Styles$WildCards.navListText,
                                          href: "https://blog.wildcards.world/",
                                          rel: "noopener noreferrer",
                                          target: "_blank",
                                          onClick: (function (param) {
                                              return Curry._1(closeModal, undefined);
                                            })
                                        }, Globals$WildCards.restr("BLOG"));
                            })
                        },
                        {
                          shouldDisplay: !isExplorer || isDetails,
                          shouldDisplayMobile: !isExplorer || isDetails,
                          component: (function (closeModal, param) {
                              return React.createElement("div", undefined, React.createElement(RimbleUi.Button, {
                                              className: Styles$WildCards.whiteText,
                                              children: Globals$WildCards.restr("VIEW WILDCARDS"),
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
                              return React.createElement(Web3Connect$WildCards.make, {
                                          clickAction: clickAction
                                        });
                            })
                        },
                        {
                          shouldDisplay: true,
                          shouldDisplayMobile: true,
                          component: (function (clickAction, isMobile) {
                              return React.createElement(ProfileIcon$WildCards.make, {
                                          clickAction: clickAction,
                                          isMobile: isMobile
                                        });
                            })
                        }
                      ]
                    }), React.createElement(GSNTest$WildCards.make, { }), React.createElement(Layout$AnimalFocusDetails, {
                      currentAnimal: undefined,
                      showForwardBackButtons: false
                    })), React.createElement(Footer$WildCards.make, { }));
}

var make = Layout;

export {
  betaBanner ,
  AnimalFocusDetails ,
  make ,
  
}
/* Css Not a pure module */
