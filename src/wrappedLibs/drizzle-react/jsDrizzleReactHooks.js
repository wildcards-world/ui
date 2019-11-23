import { drizzleReactHooks } from '@drizzle/react-plugin'

export const useGetAvailableDeposit = (userAddress) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'depositAbleToWithdraw', userAddress) || undefined
}

export const useGetPrice = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'price', tokenId) || undefined
}
export const useGetTotalCollected = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'totalCollected', tokenId) || undefined
}
export const useGetPatronageOwed = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'patronageOwed', tokenId) || undefined
}
export const useGetForeclosureTime = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'foreclosureTime', tokenId) || undefined
}

export const useUserBalance = () => {
  return drizzleReactHooks.useDrizzleState(state =>
    (!!state.accounts && !!state.accounts[0]) ?
      state.accountBalances[state.accounts[0]]
      :
      "0"
  )
}
