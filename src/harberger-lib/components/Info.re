open Globals;

[@react.component]
let make = (~chain, ~tokenId: TokenId.t) => {
  let daysHeld = QlHooks.useDaysHeld(~chain, tokenId);

  let currentPatron = QlHooks.usePatron(~chain, tokenId) |||| "Loading";
  let userId = UserProvider.useDisplayName(currentPatron);
  let displayName = UserProvider.useDisplayName(currentPatron);
  let displayNameStr = UserProvider.displayNameToString(displayName);
  let tokenName = tokenId->QlHooks.useWildcardName |||| "loading name";
  let userIdType =
    switch (userId) {
    | EthAddress(_) => "public address"
    | TwitterHandle(_) => "verified twitter account"
    | ThreeBoxName(_) => "3box name"
    };
  let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();

  let currentUsdEthPrice = UsdPriceProvider.useUsdPrice();
  let (depositAvailableToWithdrawEth, depositAvailableToWithdrawUsd) =
    QlHooks.useRemainingDepositEth(~chain, currentPatron)
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
    QlHooks.useAmountRaisedToken(~chain, tokenId)
    ->mapd(("Loading", "Loading"), a =>
        (
          (a->Eth.get(Eth.Eth(`ether))->Float.fromString |||| 0.0)
          ->toFixedWithPrecisionNoTrailingZeros(~digits=9),
          currentUsdEthPrice->mapd("Loading", usdEthRate =>
            a->Eth.get(Eth.Usd(usdEthRate, 2))
          ),
        )
      );
  let foreclosureTime = QlHooks.useForeclosureTime(~chain, currentPatron);
  let definiteTime = foreclosureTime->mapd(None, a => Some(a));

  let ratio = QlHooks.usePledgeRate(~chain, tokenId);

  let optCurrentPrice = PriceDisplay.usePrice(~chain, tokenId);

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

  let showEthWithUsdConversion =
    switch (chain) {
    | Client.MaticQuery => false
    | Client.Neither
    | Client.MainnetQuery => true
    };
  let unit = showEthWithUsdConversion ? "ETH" : "USD";

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
              "This is the monthly percentage contribution of "
              ++ tokenName
              ++ "'s sale price that will go towards conservation of at risk animals. This is deducted continuously from the deposit and paid by the owner of the animal"
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
           monthlyPledgeEth ++ " " ++ unit;
         }
         ->restr
       | None => <Rimble.Loader />
       }}
      <br />
      {switch (showEthWithUsdConversion, optMonthlyPledgeUsd) {
       | (true, Some(monthlyPledgeUsd)) =>
         <small>
           {{
              "(" ++ monthlyPledgeUsd ++ " USD)";
            }
            ->restr}
         </small>
       | _ => React.null
       }}
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
         depositAvailableToWithdrawEth ++ " " ++ unit;
       }
       ->restr}
      <br />
      {showEthWithUsdConversion
         ? <small>
             {{
                "(" ++ depositAvailableToWithdrawUsd ++ " USD)";
              }
              ->restr}
           </small>
         : React.null}
    </p>
    <p>
      <small>
        <strong>
          {{
             tokenName ++ "'s Patronage: ";
           }
           ->restr}
          <Rimble.Tooltip
            message={
              "This is the total contribution that has been raised thanks to the wildcard, "
              ++ tokenName
            }
            placement="top">
            <span> {js|ⓘ|js}->restr </span>
          </Rimble.Tooltip>
        </strong>
      </small>
      <br />
      {{
         totalPatronage ++ " " ++ unit;
       }
       ->restr}
      <br />
      {showEthWithUsdConversion
         ? <small>
             {{
                "(" ++ totalPatronageUsd ++ " USD)";
              }
              ->restr}
           </small>
         : React.null}
    </p>
    {switch (definiteTime) {
     | Some(date) =>
       <p>
         <small>
           <strong>
             "Foreclosure date: "->restr
             <Rimble.Tooltip
               message={
                 "This is the date the deposit will run out and the current owner will lose guardianship of "
                 ++ tokenName
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
                 "This is the amount of time "
                 ++ tokenName
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

module Auction = {
  [@react.component]
  let make =
      (~chain, ~tokenId: TokenId.t, ~abandoned: bool, ~auctionStartTime) => {
    let currentPatron = QlHooks.usePatron(~chain, tokenId) |||| "Loading";
    let displayName = UserProvider.useDisplayName(currentPatron);
    let displayNameStr = UserProvider.displayNameToString(displayName);

    let tokenName = tokenId->QlHooks.useWildcardName |||| "loading name";

    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();

    let currentUsdEthPrice = UsdPriceProvider.useUsdPrice();
    let (totalPatronage, totalPatronageUsd) =
      QlHooks.useAmountRaisedToken(~chain, tokenId)
      ->mapd(("Loading", "Loading"), a =>
          (
            (a->Eth.get(Eth.Eth(`ether))->Float.fromString |||| 0.0)
            ->toFixedWithPrecisionNoTrailingZeros(~digits=9),
            currentUsdEthPrice->mapd("Loading", usdEthRate =>
              a->Eth.get(Eth.Usd(usdEthRate, 2))
            ),
          )
        );

    let ratio = QlHooks.usePledgeRate(~chain, tokenId);

    let monthlyRate = Js.Float.toString(ratio *. 100.);

    let showEthWithUsdConversion =
      switch (chain) {
      | Client.MaticQuery => false
      | Client.Neither
      | Client.MainnetQuery => true
      };
    let unit = showEthWithUsdConversion ? "ETH" : "USD";

    <React.Fragment>
      <div>
        {if (ratio == 0.) {
           <p>
             "The monthly pledge rate will be revealed at launch."->restr
           </p>;
         } else {
           <>
             <small>
               <strong>
                 "Monthly Pledge Rate:"->restr
                 <Rimble.Tooltip
                   message={
                     "This is the monthly percentage contribution of "
                     ++ tokenName
                     ++ "'s sale price that will go towards conservation of at risk animals. This is deducted continuously from the deposit and paid by the guardian of the animal"
                   }
                   placement="top">
                   <span> {js|ⓘ|js}->restr </span>
                 </Rimble.Tooltip>
               </strong>
             </small>
             <br />
             {(monthlyRate ++ " %")->restr}
           </>;
         }}
      </div>
      {abandoned
         ? <p>
             <strong>
               "The previous guardian was "->restr
               <a
                 onClick={e => {
                   ReactEvent.Mouse.preventDefault(e);
                   clearAndPush({j|/#user/$currentPatron|j});
                 }}>
                 displayNameStr->restr
               </a>
               <Rimble.Tooltip
                 message={j|This happens when the user's deposit runs out for the wildcard.|j}
                 placement="top">
                 <span> {js|ⓘ|js}->restr </span>
               </Rimble.Tooltip>
             </strong>
             <br />
           </p>
         : <p>
             {(
                tokenName ++ " has never had a guardian - you can be the first."
              )
              ->React.string}
           </p>}
      <p>
        <small>
          <strong>
            {{
               tokenName ++ "'s Patronage: ";
             }
             ->restr}
            <Rimble.Tooltip
              message={
                "This is the total contribution that has been raised thanks to "
                ++ tokenName
              }
              placement="top">
              <span> {js|ⓘ|js}->restr </span>
            </Rimble.Tooltip>
          </strong>
        </small>
        <br />
        {{
           totalPatronage ++ " " ++ unit;
         }
         ->restr}
        <br />
        {showEthWithUsdConversion
           ? <small>
               {{
                  "(" ++ totalPatronageUsd ++ " USD)";
                }
                ->restr}
             </small>
           : React.null}
      </p>
      {abandoned
         ? <>
             <p>
               <small>
                 <strong>
                   "Abandoned since: "->restr
                   <Rimble.Tooltip
                     message={
                       "This is the date the deposit ran out and the current guardian will lose guardianship of "
                       ++ tokenName
                     }
                     placement="top">
                     <span> {js|ⓘ|js}->restr </span>
                   </Rimble.Tooltip>
                 </strong>
               </small>
               <br />
               {{
                  MomentRe.Moment.format("LLLL", auctionStartTime);
                }
                ->restr}
               <br />
             </p>
             // TODO: show how long it was held by the previous patron
             //  {switch (daysHeld) {
             //   | Some((daysHeldFloat, timeAquired)) =>
             //     let timeAcquiredString =
             //       timeAquired->MomentRe.Moment.toISOString;
             //     <p>
             //       <small>
             //         <strong>
             //           "Days Held: "->restr
             //           <Rimble.Tooltip
             //             message={
             //               "This is the amount of time "
             //               ++ tokenName
             //               ++ " has been held. It was acquired on the "
             //               ++ timeAcquiredString
             //               ++ "."
             //             }
             //             placement="top">
             //             <span> {js|ⓘ|js}->restr </span>
             //           </Rimble.Tooltip>
             //         </strong>
             //       </small>
             //       <br />
             //       {daysHeldFloat->Js.Float.toFixed->restr}
             //       <br />
             //     </p>;
             //   | None => React.null
             //   }}
           </>
         : React.null}
    </React.Fragment>;
  };
};
