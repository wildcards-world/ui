import { drizzleReactHooks } from '@drizzle/react-plugin'

export const useGetAvailableDeposit = (userAddress) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'depositAbleToWithdraw', userAddress)
}

export const useGetPrice = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'price', tokenId)
}

export const useUserBalance = (tokenId) => {
  const accountBalance = drizzleReactHooks.useDrizzleState(a => {
    console.log(a.accounts)
    console.log(a)
    console.log(a.balances)
  })
  return 5;
}
