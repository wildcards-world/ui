import { drizzleReactHooks as dHooks } from "drizzle-react"
import { Drizzle } from "drizzle"

let getWeb3Status = (state: any) => state.web3.status
export const useWeb3Status = () => dHooks.useDrizzleState(getWeb3Status)

class DrizzleSingleton {
  public static instance: DrizzleSingleton
  public drizzle: any = null

  constructor() {
    if (DrizzleSingleton.instance) { return DrizzleSingleton.instance }
  }

  createDrizzle(options: any, drizzleStore: any) {
    if (!this.drizzle) {
      this.drizzle = new Drizzle(options, drizzleStore)
    }
    return this.drizzle
  }
}

export default new DrizzleSingleton()
