// Generated by ReScript, PLEASE EDIT WITH CARE

import * as Curry from "rescript/lib/es6/curry.js";
import * as React from "react";
import * as ReactTabs from "react-tabs";
import * as RootProvider from "../../harberger-lib/RootProvider.bs.js";
import * as TotalDaysHeld from "./TotalDaysHeld.bs.js";
import * as TotalContribution from "./TotalContribution.bs.js";
import * as MonthlyContribution from "./MonthlyContribution.bs.js";

function indexToType(tabIndex) {
  switch (tabIndex) {
    case 0 :
        return "monthly-contribution";
    case 1 :
        return "total-contribution";
    case 2 :
        return "days-held";
    default:
      return "unknown";
  }
}

function LeaderBoards(props) {
  var clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute(undefined);
  var index;
  switch (props.leaderboardType) {
    case /* TotalContribution */0 :
        index = 1;
        break;
    case /* TotalDaysHeld */1 :
        index = 2;
        break;
    case /* Unknown */2 :
        index = -1;
        break;
    case /* MonthlyContribution */3 :
        index = 0;
        break;
    
  }
  var selectLeaderBoard = function (newIndex, _oldIndex) {
    Curry._1(clearAndPush, "#leaderboards/" + indexToType(newIndex));
    return true;
  };
  return React.createElement(ReactTabs.Tabs, {
              selectedIndex: index,
              onSelect: selectLeaderBoard,
              children: null
            }, React.createElement(ReactTabs.TabList, {
                  children: null
                }, React.createElement(ReactTabs.Tab, {
                      children: "Monthly Contribution"
                    }), React.createElement(ReactTabs.Tab, {
                      children: "Total Contribution"
                    }), React.createElement(ReactTabs.Tab, {
                      children: "Days Held"
                    })), React.createElement(ReactTabs.TabPanel, {
                  children: React.createElement(MonthlyContribution.make, {
                        numberOfLeaders: 10
                      })
                }), React.createElement(ReactTabs.TabPanel, {
                  children: React.createElement(TotalContribution.make, {
                        numberOfLeaders: 10
                      })
                }), React.createElement(ReactTabs.TabPanel, {
                  children: React.createElement(TotalDaysHeld.make, {
                        numberOfLeaders: 10
                      })
                }));
}

var make = LeaderBoards;

export {
  indexToType ,
  make ,
}
/* react Not a pure module */
