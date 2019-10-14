import { drizzleReactHooks } from '@drizzle/react-plugin'

export const useGetAvailableDeposit = (userAddress) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("VitalikSteward", 'depositAbleToWithdraw', userAddress)
}

export const useGetPrice = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("VitalikSteward", 'price', tokenId)
}

export const useUserBalance = (tokenId) => {
  const accountBalance = drizzleReactHooks.useDrizzleState(a => {
    console.log(a.accounts)
    console.log(a)
    console.log(a.balances)
  })
  return 5;
}
