type t = {countUp: int}

type options = {
  start: int,
  end: int,
  delay: int,
  duration: int,
  redraw: bool,
  preserveValue: bool,
}

@module("react-countup") external use: options => t = "useCountUp"
@send external start: (t, unit) => unit = "start"
@send external update: (t, int) => unit = "update"

module Basic = {
  @react.component
  let make = (~decimals, ~prefixString="", ~styleOnCountUp="") => {
    let countUpObj = use({
      start: decimals,
      end: decimals,
      delay: 1000,
      duration: 1,
      redraw: false,
      preserveValue: true, // This line is extremely important, otherwise it always starts from zero!!
    })
    let startCountup = countUpObj->start
    let updateCountup = countUpObj->update
    let (showCountupStyles, setShowCountupStyles) = React.useState(_ => false)

    let ref = React.useRef(0)

    React.useEffect3(() => {
      if ref.current != decimals {
        if ref.current == 0 {
          startCountup()
        } else {
          updateCountup(decimals)
          setShowCountupStyles(_ => true)
          let _timeout = Js.Global.setTimeout(() => setShowCountupStyles(_ => false), 1000)

          // NOTE: Due to how fast this component re-renders it is better not to cancel this timeout.
          // Some(() => Js.Global.clearTimeout(_timeout))
        }
        ref.current = decimals
      }
      None
    }, (decimals, startCountup, updateCountup))

    <>
      <span className={Cn.on(styleOnCountUp, showCountupStyles)}>
        {prefixString->React.string} {countUpObj.countUp->string_of_int->React.string}
      </span>
    </>
  }
}

module StringFloat = {
  @react.component
  let make = (~stringFloat, ~styleOnCountUp="") => {
    let res = stringFloat->Js.String2.split(".")
    let decimalsString = res[1]
    let optDecimals =
      decimalsString->Option.flatMap(decimals =>
        decimals->Js.String2.substring(~from=0, ~to_=9)->Int.fromString
      )

    let mainLeadingZeros = switch res[0] {
    | Some(initialString) when initialString != "0" =>
      Js.String.match_(%re("/^0*/g"), initialString)
      ->Option.flatMap(matches => matches[0])
      ->Option.getWithDefault("")
    | Some(_)
    | None => ""
    }

    let decimalLeadingZeros = switch decimalsString {
    | Some(decimalsString) =>
      "." ++
      Js.String.match_(%re("/^0*/g"), decimalsString)
      ->Option.flatMap(matches => matches[0])
      ->Option.getWithDefault("")
    | None => "."
    }

    <>
      {switch res[0]->Option.flatMap(Int.fromString) {
      | Some(initialStringPart) =>
        <Basic decimals=initialStringPart prefixString=mainLeadingZeros styleOnCountUp />
      | None => React.null
      }}
      {switch optDecimals {
      | Some(decimals) => <Basic decimals prefixString=decimalLeadingZeros styleOnCountUp />
      | None => React.null
      }}
    </>
  }
}
module TotalRaised = {
  @react.component
  let make = (~totalRaised) => {
    let styleOnCountUp = {
      open Css
      style(list{color(green), fontWeight(#bold)})
    }

    <StringFloat stringFloat=totalRaised styleOnCountUp />
  }
}
