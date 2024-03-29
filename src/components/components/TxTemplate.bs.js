// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Curry from "rescript/lib/es6/curry.js";
import * as React from "react";
import * as Styles from "../../Styles.bs.js";
import * as Web3Utils from "../../harberger-lib/Web3Utils.bs.js";
import * as RimbleUi from "rimble-ui";
import * as RootProvider from "../../harberger-lib/RootProvider.bs.js";
import * as WildcardsLoader from "../StaticContent/WildcardsLoader.bs.js";

function TxTemplate(props) {
  var txState = props.txState;
  var children = props.children;
  var etherscanUrl = RootProvider.useEtherscanUrl(undefined);
  var sidechainUrl = RootProvider.useSidechainEtherscanUrl(undefined);
  var clearNonUrlState = RootProvider.useClearNonUrlState(undefined);
  var isSideChainTx = props.chain === 1;
  var txExplererUrl = isSideChainTx ? sidechainUrl : etherscanUrl;
  if (typeof txState === "number") {
    switch (txState) {
      case /* UnInitialised */0 :
          return children;
      case /* SignMetaTx */1 :
          return React.createElement(React.Fragment, {
                      children: null
                    }, React.createElement(RimbleUi.Heading, {
                          children: null
                        }, "Please sign the message to submit this transaction.", React.createElement(WildcardsLoader.make, {})), React.createElement(RimbleUi.Flex, {
                          children: React.createElement(RimbleUi.Loader, {
                                size: "80px"
                              }),
                          justifyContent: "center"
                        }));
      case /* Created */2 :
          return React.createElement(React.Fragment, {
                      children: null
                    }, React.createElement(RimbleUi.Heading, {
                          children: null
                        }, "Processing Transaction ", React.createElement(WildcardsLoader.make, {})), React.createElement(RimbleUi.Text, {
                          children: "Tx created."
                        }), React.createElement(RimbleUi.Flex, {
                          children: React.createElement(RimbleUi.Loader, {
                                size: "80px"
                              }),
                          justifyContent: "center"
                        }));
      case /* SubmittedMetaTx */3 :
          return React.createElement(React.Fragment, {
                      children: null
                    }, React.createElement(RimbleUi.Heading, {
                          children: null
                        }, "Transaction Submitted ", React.createElement(WildcardsLoader.make, {})), React.createElement(RimbleUi.Text, {
                          children: "Awaiting transaction details."
                        }), React.createElement(RimbleUi.Flex, {
                          children: React.createElement(RimbleUi.Loader, {
                                size: "80px"
                              }),
                          justifyContent: "center"
                        }));
      case /* Failed */4 :
          return React.createElement(React.Fragment, {
                      children: null
                    }, React.createElement(RimbleUi.Heading, {
                          children: null
                        }, "The transaction failed.", React.createElement(WildcardsLoader.make, {})), React.createElement(RimbleUi.Text, {
                          children: "It is possible that someone else bought the token before you, or the price changed. If you are unsure please feel free to contact our support."
                        }), children);
      
    }
  } else {
    switch (txState.TAG | 0) {
      case /* DaiPermit */0 :
          return React.createElement(React.Fragment, {
                      children: null
                    }, React.createElement(RimbleUi.Heading, {
                          children: null
                        }, "Please sign the message to allow use of " + (Web3Utils.fromWeiBNToEthPrecision(txState._0, 2) + " DAI."), React.createElement(WildcardsLoader.make, {})), React.createElement(RimbleUi.Flex, {
                          children: React.createElement(RimbleUi.Loader, {
                                size: "80px"
                              }),
                          justifyContent: "center"
                        }));
      case /* SignedAndSubmitted */1 :
          return React.createElement(React.Fragment, {
                      children: null
                    }, React.createElement(RimbleUi.Heading, {
                          children: null
                        }, "Processing Transaction ", React.createElement(WildcardsLoader.make, {})), React.createElement(RimbleUi.Text, {
                          children: React.createElement("a", {
                                href: "https://$txExplererUrl/tx/$txHash",
                                rel: "noopener noreferrer",
                                target: "_blank"
                              }, "View the transaction on " + txExplererUrl)
                        }), React.createElement(RimbleUi.Loader, {
                          className: Styles.centerItems,
                          size: "80px"
                        }));
      case /* Declined */2 :
          return React.createElement(React.Fragment, {
                      children: null
                    }, React.createElement(RimbleUi.Heading, {
                          children: "The transaction was declined by your wallet, please try again."
                        }), React.createElement("p", undefined, "Failure reason: " + txState._0), children);
      case /* ServerError */3 :
          return React.createElement(React.Fragment, {
                      children: null
                    }, React.createElement(RimbleUi.Heading, {
                          children: "There was a server error when submitting your transaction."
                        }), React.createElement("p", undefined, "Failure reason: " + txState._0), children);
      case /* Complete */4 :
          return React.createElement(React.Fragment, {
                      children: null
                    }, React.createElement(RimbleUi.Heading, {
                          children: null
                        }, "Transaction Complete ", React.createElement(WildcardsLoader.make, {})), React.createElement(RimbleUi.Text, {
                          children: React.createElement("a", {
                                href: "https://$txExplererUrl/tx/$txHash",
                                rel: "noopener noreferrer",
                                target: "_blank"
                              }, "View the transaction on " + txExplererUrl)
                        }), React.createElement(RimbleUi.Button, {
                          children: props.closeButtonText,
                          onClick: (function (_e) {
                              Curry._1(clearNonUrlState, undefined);
                            })
                        }));
      
    }
  }
}

var make = TxTemplate;

export {
  make ,
}
/* react Not a pure module */
