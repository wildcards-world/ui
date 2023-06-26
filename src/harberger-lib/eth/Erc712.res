type structProperty = {
  @dead("structProperty.name") name: string,
  @dead("structProperty._type") @as("type")
  _type: string,
}
type typedStruct = array<structProperty>

let permit = [
  {name: "holder", _type: "address"},
  {name: "spender", _type: "address"},
  {name: "nonce", _type: "uint256"},
  {name: "expiry", _type: "uint256"},
  {name: "allowed", _type: "bool"},
]

let eip712Domain = [
  {name: "name", _type: "string"},
  {name: "version", _type: "string"},
  // {name: "chainId", _type: "uint256"},
  {name: "verifyingContract", _type: "address"},
  {name: "salt", _type: "bytes32"},
]
type domain = {
  @dead("domain.name") name: string,
  @dead("domain.version") version: string,
  // chainId: string,
  @dead("domain.verifyingContract") verifyingContract: string,
  @dead("domain.salt") salt: string,
}
