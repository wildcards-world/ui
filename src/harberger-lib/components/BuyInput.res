type debounceOptions = {
  @live maxWait: option<int>,
  @live leading: option<bool>,
  @live trailing: option<bool>,
}

type callBack<'callback> = {callback: 'callback, cancel: unit => unit, flush: unit => unit}

@module("use-debounce")
external useDebouncedCallback: (. 'callback, int, debounceOptions) => callBack<'callback> =
  "useDebouncedCallback"

let inputStyle = {
  open Css
  style(list{width(#percent(100.))})
}
let rightAlignText = {
  open Css
  style(list{textAlign(#right)})
}
let infoTooltipStyle = {
  open Css
  style(list{display(#inlineBlock)})
}
let infoIcon =
  <div className=infoTooltipStyle> <Rimble.Icon color="green" name="Info" size="16" /> </div>

@react.component
let make = (
  ~patronage,
  ~onSubmitBuy,
  ~newPrice,
  ~deposit,
  ~depositTimeInSeconds,
  ~maxAvailableDeposit,
  ~setNewPrice,
  ~setDeposit,
  ~tokenIdName,
) => {
  // ~updatePatronage: string => unit,
  // ~priceSliderInitialMax: string,
  // ~depositForAYear,

  let (depositSlider, setDepositSlider) = React.useState(_ => newPrice)

  React.useEffect2(() => {
    setDepositSlider(_ => deposit)
    None
  }, (deposit, setDepositSlider))

  let {callback: debouncedSetDeposit, _} = useDebouncedCallback(.
    setDeposit,
    50,
    {maxWait: Some(500), leading: Some(true), trailing: Some(true)},
  )
  <>
    <Rimble.Flex>
      <Rimble.Box p=2 mb=2>
        <Rimble.Heading> {`Purchase ${tokenIdName}`->React.string} </Rimble.Heading>
      </Rimble.Box>
    </Rimble.Flex>
    <Rimble.Flex>
      <Rimble.Box p=2 mb=2 width=[1., 0.5]>
        <Rimble.Tooltip
          message={`This is the amount of money you will receive if someone purchases ${tokenIdName} from you.`}
          placement="top">
          <Rimble.Text>
            {`Set ${tokenIdName}'s new for sale price:`->React.string} infoIcon
          </Rimble.Text>
        </Rimble.Tooltip>
        <Rimble.Input
          className=inputStyle
          _type="number"
          placeholder="Your Initial Sale Price"
          onChange={event => {
            let value = ReactEvent.Form.target(event)["value"]->Option.getWithDefault("")
            setNewPrice(value)
          }}
          value=newPrice
        />
      </Rimble.Box>
      <Rimble.Box p=2 mb=2 width=[1., 0.5]>
        <Rimble.Tooltip
          message="This is the monthly contribution that will go towards conservation of at risk animals. This will be deducted continuously from your deposit"
          placement="top">
          <Rimble.Text className=rightAlignText>
            {"Your monthly contribution:"->React.string} infoIcon
          </Rimble.Text>
        </Rimble.Tooltip>
        <br />
        <Rimble.Text className=rightAlignText> {patronage->React.string} </Rimble.Text>
      </Rimble.Box>
    </Rimble.Flex>
    <Rimble.Flex alignItems="center" justifyContent="center">
      <Rimble.Box p=2 mb=2 width=[1., 0.3]>
        <Rimble.Tooltip
          message="The deposit is the funds that will be used to cover your monthly contribution."
          placement="top">
          <Rimble.Text> {"Set your deposit:"->React.string} infoIcon </Rimble.Text>
        </Rimble.Tooltip>
        <Rimble.Input
          _type="number"
          className=inputStyle
          placeholder="Your Initial Deposit"
          onChange={event => {
            let value = ReactEvent.Form.target(event)["value"]->Option.getWithDefault("")
            setDeposit(value)
          }}
          value=deposit
        />
      </Rimble.Box>
      <Rimble.Box p=2 mb=2 width=[1., 0.7]>
        <Rimble.Slider
          value=depositSlider
          className=inputStyle
          onChange={event => {
            let value = ReactEvent.Form.target(event)["value"]->Option.getWithDefault("")
            setDepositSlider(_ => value)
            debouncedSetDeposit(value)
          }}
          min="0.0001"
          max=maxAvailableDeposit
          step="0.0000001"
        />
      </Rimble.Box>
    </Rimble.Flex>
    <p>
      {("This deposit will last " ++
      (depositTimeInSeconds->CountDown.displayTimeLeftHours ++
      " for your monthly contribution"))->React.string}
    </p>
    <Rimble.Flex>
      <Rimble.Box p=2 mb=2 width=[1., 0.7]>
        <Rimble.Button onClick={_ => onSubmitBuy()}> {"Buy"->React.string} </Rimble.Button>
      </Rimble.Box>
    </Rimble.Flex>
  </>
}
