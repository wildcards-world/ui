import { drizzleReactHooks } from '@drizzle/react-plugin'

export const useGetPrice = (tokenId) => {
  const { useCacheCall } = drizzleReactHooks.useDrizzle()
  return useCacheCall("WildcardSteward_v0", 'price', tokenId)
}
