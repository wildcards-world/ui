[@react.component]
let make = (~leaderboardType) => {
  Js.log(leaderboardType);
  // React.null;
  <ReactTabs>
    <ReactTabs.TabList>
      <ReactTabs.Tab> "Monthly Contribution"->React.string </ReactTabs.Tab>
      <ReactTabs.Tab> "Total Contribution"->React.string </ReactTabs.Tab>
      <ReactTabs.Tab> "Days Held"->React.string </ReactTabs.Tab>
    </ReactTabs.TabList>
    <ReactTabs.TabPanel>
      <MonthlyContribution numberOfLeaders=10 />
    </ReactTabs.TabPanel>
    <ReactTabs.TabPanel>
      <TotalContribution numberOfLeaders=10 />
    </ReactTabs.TabPanel>
    <ReactTabs.TabPanel>
      <TotalDaysHeld numberOfLeaders=10 />
    </ReactTabs.TabPanel>
  </ReactTabs>;
  // switch (leaderboardType) {
  // | Router.TotalContribution => <TotalContribution numberOfLeaders=10 />
  // | Router.MonthlyContribution => <MonthlyContribution numberOfLeaders=10 />
  // | Router.Unknown => <p> "Not found"->React.string </p>
  // };
};
