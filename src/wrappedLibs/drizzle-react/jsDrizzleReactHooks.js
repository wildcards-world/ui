import { drizzleReactHooks } from '@drizzle/react-plugin'

export const useGetAvailableDeposit = (userAddress) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'depositAbleToWithdraw', userAddress)
}

export const useGetPrice = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'price', tokenId)
}
export const useGetTotalCollected = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'totalCollected', tokenId)
}
export const useGetPatronageOwed = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'patronageOwed', tokenId)
}
export const useGetForeclosureTime = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'foreclosureTime', tokenId)
}

export const useUserBalance = () => {
  return drizzleReactHooks.useDrizzleState(state =>
    (!!state.accounts && !!state.accounts[0]) ?
      state.accountBalances[state.accounts[0]]
      :
      "0"
  )
}
