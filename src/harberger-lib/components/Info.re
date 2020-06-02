open Globals;

[@gentype]
[@react.component]
let make = (~tokenId: TokenId.t) => {
  let daysHeld = QlHooks.useDaysHeld(tokenId);

  let currentPatron = QlHooks.usePatron(tokenId) |||| "Loading";
  let userId = UserProvider.useDisplayName(currentPatron);
  let displayName = UserProvider.useDisplayName(currentPatron);
  let displayNameStr = UserProvider.displayNameToString(displayName);
  let userIdType =
    switch (userId) {
    | EthAddress(_) => "public address"
    | TwitterHandle(_) => "verified twitter account"
    | ThreeBoxName(_) => "3box name"
    };
  let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();

  let currentUsdEthPrice = UsdPriceProvider.useUsdPrice();
  let (depositAvailableToWithdrawEth, depositAvailableToWithdrawUsd) =
    QlHooks.useRemainingDepositEth(currentPatron)
    ->mapd(("Loading", "Loading"), a =>
        (
          (a->Eth.get(Eth.Eth(`ether))->Float.fromString |||| 0.0)
          ->toFixedWithPrecisionNoTrailingZeros(~digits=9),
          currentUsdEthPrice->mapd("Loading", usdEthRate =>
            a->Eth.get(Eth.Usd(usdEthRate, 2))
          ),
        )
      );

  let (totalPatronage, totalPatronageUsd) =
    QlHooks.useAmountRaisedToken(tokenId)
    ->mapd(("Loading", "Loading"), a =>
        (
          (a->Eth.get(Eth.Eth(`ether))->Float.fromString |||| 0.0)
          ->toFixedWithPrecisionNoTrailingZeros(~digits=9),
          currentUsdEthPrice->mapd("Loading", usdEthRate =>
            a->Eth.get(Eth.Usd(usdEthRate, 2))
          ),
        )
      );
  let foreclosureTime = QlHooks.useForeclosureTime(currentPatron);
  let definiteTime = foreclosureTime->mapd(None, a => Some(a));

  let ratio = QlHooks.usePledgeRate(tokenId);

  let optCurrentPrice = PriceDisplay.usePrice(tokenId);

  let (optMonthlyPledgeEth, optMonthlyPledgeUsd) =
    switch (optCurrentPrice) {
    | Some((priceEth, optPriceUsd)) => (
        Some(
          toFixedWithPrecisionNoTrailingZeros(
            Float.fromString(priceEth)->Accounting.defaultZeroF *. ratio,
            ~digits=4,
          ),
        ),
        switch (optPriceUsd) {
        | Some(_priceUsd) => None

        | None => None
        },
      )
    | None => (None, None)
    };

  let monthlyRate = Js.Float.toString(ratio *. 100.);

  <React.Fragment>
    <div>
      <small>
        <strong>
          {{
             "Monthly Pledge (at " ++ monthlyRate ++ "%): ";
           }
           ->restr}
          <Rimble.Tooltip
            message={
              "This is the monthly percentage contribution of token#"
              ++ tokenId->TokenId.toString
              ++ "'s sale price that will go towards conservation of endangered animals. This is deducted continuously from the deposit and paid by the owner of the animal"
            }
            placement="top">
            <span> {js|ⓘ|js}->restr </span>
          </Rimble.Tooltip>
        </strong>
      </small>
      <br />
      {switch (optMonthlyPledgeEth) {
       | Some(monthlyPledgeEth) =>
         {
           monthlyPledgeEth ++ " ETH";
         }
         ->restr
       | None => <Rimble.Loader />
       }}
      <br />
      <small>
        {switch (optMonthlyPledgeUsd) {
         | Some(monthlyPledgeUsd) =>
           {
             "(" ++ monthlyPledgeUsd ++ " USD)";
           }
           ->restr
         | None => React.null
         }}
      </small>
    </div>
    <p>
      <small>
        <strong>
          "Current Patron: "->restr
          <Rimble.Tooltip
            message={j|This is the $userIdType of the current owner|j}
            placement="top">
            <span> {js|ⓘ|js}->restr </span>
          </Rimble.Tooltip>
        </strong>
      </small>
      <br />
      <a
        onClick={e => {
          ReactEvent.Mouse.preventDefault(e);
          clearAndPush({j|/#user/$currentPatron|j});
        }}>
        displayNameStr->restr
      </a>
    </p>
    <p>
      <small>
        <strong>
          "Available Deposit: "->restr
          <Rimble.Tooltip
            message="This is the amount the owner has deposited to pay their monthly contribution"
            placement="top">
            <span> {js|ⓘ|js}->restr </span>
          </Rimble.Tooltip>
        </strong>
      </small>
      <br />
      {{
         depositAvailableToWithdrawEth ++ " ETH";
       }
       ->restr}
      <br />
      <small>
        {{
           "(" ++ depositAvailableToWithdrawUsd ++ " USD)";
         }
         ->restr}
      </small>
    </p>
    <p>
      <small>
        <strong>
          {{
             "token#" ++ tokenId->TokenId.toString ++ "'s Patronage: ";
           }
           ->restr}
          <Rimble.Tooltip
            message={
              "This is the total contribution that has been raised thanks to the wildcard, token#"
              ++ tokenId->TokenId.toString
            }
            placement="top">
            <span> {js|ⓘ|js}->restr </span>
          </Rimble.Tooltip>
        </strong>
      </small>
      <br />
      {{
         totalPatronage ++ " ETH";
       }
       ->restr}
      <br />
      <small>
        {{
           "(" ++ totalPatronageUsd ++ " USD)";
         }
         ->restr}
      </small>
    </p>
    {switch (definiteTime) {
     | Some(date) =>
       <p>
         <small>
           <strong>
             "Foreclosure date: "->restr
             <Rimble.Tooltip
               message={
                 "This is the date the deposit will run out and the current owner will lose ownership of token#"
                 ++ tokenId->TokenId.toString
               }
               placement="top">
               <span> {js|ⓘ|js}->restr </span>
             </Rimble.Tooltip>
           </strong>
         </small>
         <br />
         {{
            MomentRe.Moment.format("LLLL", date);
          }
          ->restr}
         <br />
         <small>
           "( "->restr
           <CountDown endDateMoment=date />
           ")"->restr
         </small>
       </p>
     | None => React.null
     }}
    {switch (daysHeld) {
     | Some((daysHeldFloat, timeAquired)) =>
       let timeAcquiredString = timeAquired->MomentRe.Moment.toISOString;
       <p>
         <small>
           <strong>
             "Days Held: "->restr
             <Rimble.Tooltip
               message={
                 "This is the amount of time token#"
                 ++ tokenId->TokenId.toString
                 ++ " has been held. It was acquired on the "
                 ++ timeAcquiredString
                 ++ "."
               }
               placement="top">
               <span> {js|ⓘ|js}->restr </span>
             </Rimble.Tooltip>
           </strong>
         </small>
         <br />
         {daysHeldFloat->Js.Float.toFixed->restr}
         <br />
       </p>;
     | None => React.null
     }}
  </React.Fragment>;
};
