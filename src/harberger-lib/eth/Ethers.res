type txResult = {
  @dead("txResult.blockHash") blockHash: string,
  @dead("txResult.blockNumber") blockNumber: int,
  @dead("txResult.byzantium") byzantium: bool,
  @dead("txResult.confirmations") confirmations: int,
  // contractAddress: null,
  // cumulativeGasUsed: Object { _hex: "0x26063", … },
  // events: Array(4) [ {…}, {…}, {…}, … ],
  @dead("txResult.from") from: Web3.ethAddress,
  // gasUsed: Object { _hex: "0x26063", … },
  // logs: Array(4) [ {…}, {…}, {…}, … ],
  // logsBloom: "0x00200000000000008000000000000000000020000001000000000000400020000000000000002000000000000000000000000002800010000000008000000000000000000000000000000008000000000040000000000000000000000000000000000000020000014000000000000800024000000000000000000010000000000000000000000000000000000000000000008000000000000000000000000200000008000000000000000000000000000000000800000000000000000000000000001002000000000000000000000000000000000000000020000000040020000000000000000080000000000000000000000000000000080000000000200000"
  @dead("txResult.status") status: int,
  @dead("txResult._to") _to: Web3.ethAddress,
  transactionHash: string,
  @dead("txResult.transactionIndex") transactionIndex: int,
}
type txError = {
  @dead("txError.code") code: int, // -32000 = always failing tx ;  4001 = Rejected by signer.
  message: string,
  @dead("txError.stack") stack: option<string>,
}
@new @module("ethers") @scope("providers")
external makeProvider: string => Web3.rawProvider = "JsonRpcProvider"

@send
external waitForTransaction: (Web3.rawProvider, string) => Promise.Js.t<txResult, txError> =
  "waitForTransaction"