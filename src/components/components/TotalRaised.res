type patronageRaised =
  | Loaded(option<string>, string, string)
  | Loading

// let uesTotalPatronage = () => {
// let optTotalPatronageWei = QlHooks.useAmountRaised(~chain=Client.MainnetQuery)
// let optTotalPatronageDai = QlHooks.useAmountRaised(~chain=Client.MaticQuery)
//   let optCurrentUsdEthPrice = useUsdPrice()

//   switch (optTotalPatronageWei, optTotalPatronageDai) {
//   | (Some(totalPatronageWei), Some(totalPatronageDai)) =>
//     let totalPatronageEth = totalPatronageWei->BN.toString->Web3Utils.fromWeiToEth
//     let totalPatronageDaiUsd = totalPatronageDai->BN.toString->Web3Utils.fromWeiToEth
//     let totalEthFloat = Float.fromString(totalPatronageEth)->Option.getWithDefault(0.)
//     let totalEthStr = totalEthFloat->Js.Float.toFixedWithPrecision(~digits=2)
//     let totalDaiFloat = Float.fromString(totalPatronageDaiUsd)->Option.getWithDefault(0.)
//     let totalDaiStr = totalDaiFloat->Js.Float.toFixedWithPrecision(~digits=2)

//     Js.log2(totalEthStr, totalDaiStr)

//     let optTotaPatronageUsd =
//       optCurrentUsdEthPrice
//       ->Option.map(currentUsdEthPrice => totalEthFloat *. currentUsdEthPrice +. totalDaiFloat)
//       ->Option.map(totaPatronageUsd => Js.Float.toFixedWithPrecision(totaPatronageUsd, ~digits=5))

//     Loaded(optTotaPatronageUsd, totalEthStr, totalDaiStr)
//   | _ => Loading
//   }
// }

@react.component
let make = () => {
  let optTotalPatronageWei = QlHooks.useAmountRaised(~chain=Client.MainnetQuery)
  let optTotalPatronageDai = QlHooks.useAmountRaised(~chain=Client.MaticQuery)

  switch (optTotalPatronageWei, optTotalPatronageDai) {
  | (Some(mainnetEth), Some(maticDai)) =>
    <div
      className={
        open Css
        style(list{
          // These styles make the total raised counter always display in the centre.
          display(#flex),
          alignItems(#center),
          justifyContent(#center),
          flexDirection(#column),
        })
      }>
      <p
        className={
          open Css
          style(list{display(#table)})
        }>
        {React.string("Wildcards has currently raised ")}
        <br />
        <Amounts.AmountRaised
          mainnetEth
          maticDai
          populateElement={(~bigTextComponent, ~smallTextComponent, ~optCommentTextComponent) => {
            <>
              <span className={Styles.totalRaisedText(4.)}>
                {bigTextComponent}
                <span className={Styles.totalRaisedText(0.75)}> {smallTextComponent} </span>
                <strong> {React.string(" USD")} </strong>
              </span>
              <br />
              {switch optCommentTextComponent {
              | Some(commentTextComponent) =>
                <React.Fragment>
                  <span className={Styles.totalRaisedText(2.)}> {commentTextComponent} </span>
                </React.Fragment>
              | None => React.null
              }}
              <br />
              <span className={Styles.totalRaisedText(1.5)}>
                {React.string(" for conservation.")}
              </span>
            </>
          }}
        />
      </p>
    </div>
  | _ => <Rimble.Loader />
  }
}
