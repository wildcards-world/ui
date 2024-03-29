// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Curry from "rescript/lib/es6/curry.js";
import * as React from "react";
import * as QlHooks from "../harberger-lib/QlHooks.bs.js";
import * as TokenId from "../harberger-lib/TokenId.bs.js";
import * as InputHelp from "../harberger-lib/InputHelp.bs.js";
import * as RimbleUi from "rimble-ui";
import * as RootProvider from "../harberger-lib/RootProvider.bs.js";

function ActionButtons$Buy(props) {
  var isExplorer = props.isExplorer;
  var animal = props.animal;
  var isExplorer$1 = isExplorer !== undefined ? isExplorer : false;
  var currentPriceWei = QlHooks.usePrice(props.chain, animal);
  var goToBuy = RootProvider.useGoToBuy(undefined);
  var clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute(undefined);
  var buttonText;
  buttonText = typeof currentPriceWei === "number" ? "loading" : (
      currentPriceWei.TAG === /* Foreclosed */0 ? "Adopt" : "Buy"
    );
  return React.createElement(React.Fragment, {
              children: React.createElement(RimbleUi.Button, {
                    children: buttonText,
                    onClick: (function (_e) {
                        Curry._1(clearAndPush, "#" + (InputHelp.getPagePrefix(isExplorer$1) + ("details/" + TokenId.toString(animal))));
                        Curry._1(goToBuy, animal);
                      })
                  })
            });
}

var Buy = {
  make: ActionButtons$Buy
};

function ActionButtons$Auction(props) {
  var isExplorer = props.isExplorer;
  var animal = props.animal;
  var isExplorer$1 = isExplorer !== undefined ? isExplorer : false;
  var goToBuy = RootProvider.useGoToBuy(undefined);
  var clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute(undefined);
  return React.createElement(React.Fragment, {
              children: React.createElement(RimbleUi.Button, {
                    children: "Auction",
                    onClick: (function (_e) {
                        Curry._1(clearAndPush, "#" + (InputHelp.getPagePrefix(isExplorer$1) + ("details/" + TokenId.toString(animal))));
                        Curry._1(goToBuy, animal);
                      })
                  })
            });
}

var Auction = {
  make: ActionButtons$Auction
};

function ActionButtons$UpdateDeposit(props) {
  var goToDepositUpdate = RootProvider.useGoToDepositUpdate(undefined);
  return React.createElement(React.Fragment, {
              children: React.createElement(RimbleUi.Button, {
                    children: "Deposit",
                    onClick: (function (_e) {
                        Curry._1(goToDepositUpdate, undefined);
                      })
                  })
            });
}

var UpdateDeposit = {
  make: ActionButtons$UpdateDeposit
};

function ActionButtons$UpdatePrice(props) {
  var animal = props.animal;
  var goToPriceUpdate = RootProvider.useGoToPriceUpdate(undefined);
  return React.createElement(React.Fragment, {
              children: React.createElement(RimbleUi.Box, {
                    p: 1,
                    children: React.createElement(RimbleUi.Button, {
                          children: "Price",
                          onClick: (function (_e) {
                              Curry._1(goToPriceUpdate, animal);
                            })
                        })
                  })
            });
}

var UpdatePrice = {
  make: ActionButtons$UpdatePrice
};

export {
  Buy ,
  Auction ,
  UpdateDeposit ,
  UpdatePrice ,
}
/* react Not a pure module */
