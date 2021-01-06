let stringToArray = str => str->Js.String.castToArrayLike->Js.Array.from
let toDollarCentsFixedNoRounding = aFloat =>
  aFloat
  ->Float.toString
  ->Js.String2.match_(%re("/^\d+[.]\d\d/g"))
  ->Option.getWithDefault([""])
  ->Array.getUnsafe(0)

module AmountRaised = {
  @react.component
  let make = (~mainnetEth, ~maticDai) => {
    let currentUsdEthPrice = UsdPriceProvider.useUsdPrice()
    let (prevUsdRaisedStrArray, setPrevUsdRaisedStrArray) = React.useState(_ => [])
    let (
      (raisedMainUnchanged, raisedMainChanged, raisedSubUnchanged, raisedSubChanged),
      setRaisedDisplay,
    ) = React.useState(_ => ("", "", "", ""))

    let usdRaisedFloat =
      currentUsdEthPrice->Option.mapWithDefault(0., usdEthRate =>
        mainnetEth->Eth.getFloat(Eth.Usd(usdEthRate, 2))
      ) +. maticDai->Eth.getFloat(Eth.Eth(#ether))

    let usdRaisedStr = usdRaisedFloat->Js.Float.toFixedWithPrecision(~digits=6)
    let usdRaised2Precision = usdRaisedFloat->toDollarCentsFixedNoRounding

    React.useEffect1(() => {
      let usdRaisedDigitArray = usdRaisedStr->stringToArray

      let indexOfTheFirstChangedDigit = Js.Array.findIndexi((newDigit, index) =>
        switch prevUsdRaisedStrArray[index] {
        | Some(digit) => digit != newDigit
        | None => false
        }
      , usdRaisedDigitArray)

      switch (indexOfTheFirstChangedDigit, usdRaised2Precision->String.length) {
      | (index, length) when index < 0 =>
        setRaisedDisplay(_ => (
          usdRaised2Precision,
          "",
          Js.String.sliceToEnd(~from=length, usdRaisedStr),
          "",
        ))
      | (index, length) when index > length =>
        setRaisedDisplay(_ => (
          usdRaised2Precision,
          "",
          Js.String.slice(~from=length, ~to_=index, usdRaisedStr),
          Js.String.sliceToEnd(~from=index, usdRaisedStr),
        ))
      | (index, length) =>
        setRaisedDisplay(_ => (
          Js.String.slice(~from=0, ~to_=index, usdRaised2Precision),
          Js.String.sliceToEnd(~from=index, usdRaised2Precision),
          "",
          Js.String.sliceToEnd(~from=length, usdRaisedStr),
        ))
      }

      setPrevUsdRaisedStrArray(_ => usdRaisedDigitArray)
      None
    }, [usdRaisedStr])

    let optExplainerString = switch (
      mainnetEth->Web3Utils.fromWeiBNToEthPrecision(~digits=4),
      maticDai->Web3Utils.fromWeiBNToEthPrecision(~digits=2),
    ) {
    | ("0", "0") => None
    | (ethAmount, "0") => Some("(" ++ ethAmount ++ " ETH)")
    | ("0", _) => None
    | (ethAmount, daiAmount) => Some("(" ++ ethAmount ++ " ETH + " ++ daiAmount ++ "DAI)")
    }

    let styleOnCountUp = {
      open Css
      style(list{color(green), fontWeight(#bold)})
    }

    <>
      <p>
        <span> {raisedMainUnchanged->React.string} </span>
        <Countup.StringFloat stringFloat={raisedMainChanged} styleOnCountUp />
        <span
          className={
            open CssJs
            style(.[fontSize(em(0.75))])
          }>
          <span> {raisedSubUnchanged->React.string} </span>
          {switch raisedSubChanged {
          | "" => React.null
          | stringFloat => <Countup.StringFloat stringFloat styleOnCountUp />
          }}
        </span>
        {" USD"->React.string}
        {switch optExplainerString {
        | Some(explainerString) => <> <br /> <small> {explainerString->React.string} </small> </>
        | None => React.null
        }}
      </p>
    </>
  }
}