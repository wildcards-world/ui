module Countup = {
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
}

@react.component
let make = (~totalRaised) => {
  let res = totalRaised->Js.String2.split(".")
  let initialStringPart = res[0]->Option.getWithDefault("")
  let decimalsString = res[1]
  let decimals =
    decimalsString
    ->Option.flatMap(decimals => decimals->Js.String2.substring(~from=0, ~to_=9)->Int.fromString)
    ->Option.getWithDefault(0)
  %log.info(
    "INfo"
    ("res", res)
  )

  let leadingZeros = switch decimalsString {
  | Some(decimalsString) =>
    "." ++
    Js.String.match_(%re("/^0*/g"), decimalsString)
    ->Option.flatMap(matches => matches[0])
    ->Option.getWithDefault("")
  | None => ""
  }

  let countUpObj = Countup.use({
    start: decimals,
    end: decimals,
    delay: 1000,
    duration: 1,
    redraw: false,
    preserveValue: true, // This line is extremely important, otherwise it always starts from zero!!
  })
  let startCountup = countUpObj->Countup.start
  let updateCountup = countUpObj->Countup.update
  let (showCountupStyles, setShowCountupStyles) = React.useState(_ => false)

  let ref = React.useRef(0)

  React.useEffect4(() => {
    if ref.current != decimals {
      ref.current = decimals
      if ref.current == 0 {
        startCountup()
        None
      } else {
        updateCountup(decimals)
        setShowCountupStyles(_ => true)
        let timeout = Js.Global.setTimeout(() => setShowCountupStyles(_ => false), 1000)
        Some(() => Js.Global.clearTimeout(timeout))
      }
    } else {
      None
    }
  }, (totalRaised, decimals, startCountup, updateCountup))

  <>
    <span
      className={
        open Css
        Cn.on(style(list{color(green), fontWeight(#bold)}), showCountupStyles)
      }>
      {initialStringPart->React.string}
      {leadingZeros->React.string}
      {countUpObj.countUp->string_of_int->React.string}
    </span>
  </>
}
