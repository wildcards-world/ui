import { drizzleReactHooks } from '@drizzle/react-plugin'

export const useGetAvailableDeposit = (userAddress) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("VitalikSteward", 'depositAbleToWithdraw', userAddress)
}

export const useGetPrice = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("VitalikSteward", 'price', tokenId)
}

export const useUserBalance = () => {
  return drizzleReactHooks.useDrizzleState(state =>
    (!!state.accounts && !!state.accounts[0]) ?
      state.accountBalances[state.accounts[0]]
      :
      "0"
  )
}
