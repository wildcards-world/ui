import React from "react"
import { drizzleReactHooks as dHooks } from "drizzle-react"
import DrizzleSingleton from "./drizzle"
import { diff } from "deep-object-diff"

// let drizzle = null
let getWeb3FromState = fullState => fullState
// let getTotalCollected = state => {
//   const { VitalikSteward } = state.contracts
//   return VitalikSteward
//   // console.log(VitalikSteward)
//   // DrizzleSingleton.drizzle.contracts.VitalikSteward.methods.getTotalCollected.callCache()
//   // if (!VitalikSteward.initialized) {
//   //   return "waaa"
//   // } else {
//   //   // console.log(drizzle)
//   //   // console.log('diff(drizzle, DrizzleSingleton.drizzle)')
//   //   // console.log(diff(drizzle, DrizzleSingleton.drizzle))
//   //   // console.log(DrizzleSingleton.drizzle.contracts.VitalikSteward)
//   //   // console.log(drizzle.drizzle.contracts.VitalikSteward.methods.totalCollected.callCache())
//   //   // DrizzleSingleton.drizzle.contracts.VitalikSteward.methods.getTotalCollected.callCache()
//   //   return "woooo"
//   // }
// }

export default () => {
  // let drizzle = dHooks.useDrizzleState(getWeb3FromState).status
  let { drizzle,
    useCacheCall,
    useCacheEvents,
    useCacheSend } = dHooks.useDrizzle()

  let state = dHooks.useDrizzleState(getWeb3FromState)
  // let VitalikSteward = dHooks.useDrizzleState(getTotalCollected)
  // if (!!VitalikSteward.initialized) {
  //   console.log(state.contracts.VitalikSteward.totalCollected)
  console.log(state.contracts.VitalikSteward.patronageOwed)

  const getTotalCollected = call => (!!state.contracts.VitalikSteward.initialized) ? call('VitalikSteward', 'totalCollected') : 'N/A'
  const getPatronageOwed = call => (!!state.contracts.VitalikSteward.initialized) ? call('VitalikSteward', 'patronageOwed') : 'N/A'
  let totalCollected = useCacheCall(['VitalikSteward'], getTotalCollected)
  let patronageOwed = useCacheCall(['VitalikSteward'], getPatronageOwed)
  console.log(totalCollected + patronageOwed)
  // } else {
  //   let status = dHooks.useDrizzleState(getWeb3FromState).status
  //   let boom = dHooks.useDrizzleState(getWeb3FromState).status
  // }

  return <h1> ...use the web3 object...  </h1>
}
