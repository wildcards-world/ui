// This 
import React from "react"
import {
  useTotalPatronageWei,
  useTotalPatronageEth,
  useTotalPatronageUsd,
  useDepositAbleToWithdrawWei,
  useDepositAbleToWithdrawEth,
  useDepositAbleToWithdrawUsd,
  useForeclosureTime,
  useCurrentPriceWei,
  useCurrentPriceEth,
  useCurrentPriceUsd,
  useCurrentPatron,
  useAvailableDeposit,
  useBuyTransaction,
} from "../../helpers/hooks/Hooks.bs"

export default () => {
  const TotalPatronageWei = useTotalPatronageWei()
  const TotalPatronageEth = useTotalPatronageEth()
  const TotalPatronageUsd = useTotalPatronageUsd()
  const DepositAbleToWithdrawWei = useDepositAbleToWithdrawWei()
  const DepositAbleToWithdrawEth = useDepositAbleToWithdrawEth()
  const DepositAbleToWithdrawUsd = useDepositAbleToWithdrawUsd()
  const ForeclosureTime = useForeclosureTime()
  const CurrentPriceWei = useCurrentPriceWei()
  const CurrentPriceEth = useCurrentPriceEth()
  const CurrentPriceUsd = useCurrentPriceUsd()
  const CurrentPatron = useCurrentPatron()
  const buyThing = useBuyTransaction()

  if (!window.buyThing) {
    window.buyThing = buyThing
  }
  // const AvailableDeposit = useAvailableDeposit()

  return <div>
    <h1> This is a demo (to show how nice it is working with the refined architecture) [which is definitely also still a work in progress!] </h1>
    <p>TotalPatronageWei: {TotalPatronageWei}</p>
    <p>TotalPatronageEth: {TotalPatronageEth}</p>
    <p>TotalPatronageUsd: {TotalPatronageUsd}</p>
    <p>DepositAbleToWithdrawWei: {DepositAbleToWithdrawWei}</p>
    <p>DepositAbleToWithdrawEth: {DepositAbleToWithdrawEth}</p>
    <p>DepositAbleToWithdrawUsd: {DepositAbleToWithdrawUsd}</p>
    {/* <p>ForeclosureTime: {ForeclosureTime.toString()}</p> */}
    <p>CurrentPriceWei: {CurrentPriceWei}</p>
    <p>CurrentPriceEth: {CurrentPriceEth}</p>
    <p>CurrentPriceUsd: {CurrentPriceUsd}</p>
    <p>CurrentPatron: {CurrentPatron}</p>
    {/*<p>AvailableDeposit: {AvailableDeposit}</p>*/}
  </div>
}
