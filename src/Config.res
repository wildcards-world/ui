@val
external network: option<string> = "process.env.REACT_APP_NETWORK"

let isTestnet = network->Option.mapWithDefault(false, network => network == "TEST")

let availableNetworkIds = isTestnet ? [4, 5, 80001] : [1, 137]

type withdrawal = {
  txHash: string,
  amount: Ethers.BigNumber.t,
  amountInUsd: float,
  networkId: int,
}
type orgWithdrawals = array<withdrawal>
type allWithdrawals = Js.Dict.t<orgWithdrawals>

let orgWtihdrawalsMapping = Js.Dict.fromArray([
  (
    "careforwild",
    [
      {
        // Care for Wild 1	0xc0eD94f053E7Ee5f24DCebbfd4dcF16d8E767d5F		https://etherscan.io/tx/0x556f75dc9129bbb8d2f9bda68ded0a521e69ad0f6c9589227cbc21bb64077cbc	0.748345411		$3,576.08	2,676.14	
        txHash: "0x556f75dc9129bbb8d2f9bda68ded0a521e69ad0f6c9589227cbc21bb64077cbc",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="0.748345411"),
        amountInUsd: 2676.14,
        networkId: 1,
      },
    ],
  ),
  (
    "careforwild",
    [
      {
        // Care for Wild	Care for wild 2			https://etherscan.io/tx/0xf6b02b69f8f5f734721ed6f38821bfedebcc2873549e5db2e9ec1b6a1a1011a2	0.6317736644		1,368.22	864.4053631																						
        txHash: "0xf6b02b69f8f5f734721ed6f38821bfedebcc2873549e5db2e9ec1b6a1a1011a2",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="0.6317736644"),
        amountInUsd: 864.41,
        networkId: 1,
      },
    ],
  ),
  (
    "darwinanimaldoctors",
    [
      {
        // Darwin Animal Doctors 1	0x233f9bcb02bfAD03aAcCb6cE40b6C4f83C867603		https://etherscan.io/tx/0x218b1469a0c5a19842a1d14778371032f8ef7195d349a3dfbe4d813dad5f3c9d	0.7225148935		1,368.22	988.56	
        txHash: "0x218b1469a0c5a19842a1d14778371032f8ef7195d349a3dfbe4d813dad5f3c9d",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="0.7225148935"),
        amountInUsd: 988.56,
        networkId: 1,
      },
    ],
  ),
  (
    "darwinanimaldoctors",
    [
      {
        // Darwin Animal Doctors 2			https://etherscan.io/tx/0x3d96e4119e084de9a9ee770d3f21fb51c894a646a910b251ee5edd02a3a2a2c3	0.3392833942		3,544.14	1,202.47	
        txHash: "0x3d96e4119e084de9a9ee770d3f21fb51c894a646a910b251ee5edd02a3a2a2c3",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="0.748345411"),
        amountInUsd: 1202.47,
        networkId: 1,
      },
    ],
  ),
  (
    "bdi",
    [
      {
        // BDI 1	0xADad0D21ba0E4b356e2b2769e08CfeF206f83891		https://etherscan.io/tx/0x6582ea27593806e6822425574de60544499d3954bca5078629c9e096678c6ac8	2.544068973		1,368.22	3,480.85	
        txHash: "0x6582ea27593806e6822425574de60544499d3954bca5078629c9e096678c6ac8",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="2.544068973"),
        amountInUsd: 3480.85,
        networkId: 1,
      },
    ],
  ),
  (
    "bdi",
    [
      {
        // BDI 2			https://etherscan.io/tx/0xc3a37a1a25337f631328a6833ac4ec922f7df7f075a7ccddb89e11857f0536e6	2.81152196		3,424.24	9,627.33	
        txHash: "0xc3a37a1a25337f631328a6833ac4ec922f7df7f075a7ccddb89e11857f0536e6",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="2.81152196"),
        amountInUsd: 9627.33,
        networkId: 1,
      },
    ],
  ),
  (
    "pangolinafrica",
    [
      {
        // Pangolin Africa	0x0471C0ADbF27c53FbCf122C829807bd3DE3fec55		https://etherscan.io/tx/0xf5b33c84b77827b775d9dfa516113c596706ad5ad8d0cc6c9a456d43258caaf4	0.2841441781		1,368.22	388.77	
        txHash: "0xf5b33c84b77827b775d9dfa516113c596706ad5ad8d0cc6c9a456d43258caaf4",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="0.2841441781"),
        amountInUsd: 388.77,
        networkId: 1,
      },
    ],
  ),
  (
    "sendaverde",
    [
      {
        // Senda Verda		Manual payment to senda verde (they didn't have an ethereum address initially)	https://etherscan.io/tx/0x17343dfee8fb442535af99ab5c6bf655c8e8eb2a63f706062fa5921231b86cf3	12.1511067	Jan-29-2021 06:46:42 AM +UTC	1,379.13	16757.95578	
        txHash: "0x17343dfee8fb442535af99ab5c6bf655c8e8eb2a63f706062fa5921231b86cf3",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="12.1511067"),
        amountInUsd: 16757.96,
        networkId: 1,
      },
    ],
  ),
  (
    "sendaverde",
    [
      {
        // https://etherscan.io/tx/0x5e749f311768f4f2237a3d47e73ce3626753739c057cf19e153db5bffcd55772	3.634006486	Jan-26-2021 01:14:03 PM +UTC	1368.22	4972.120354	
        txHash: "0x5e749f311768f4f2237a3d47e73ce3626753739c057cf19e153db5bffcd55772",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="3.634006486"),
        amountInUsd: 4972.12,
        networkId: 1,
      },
    ],
  ),
  (
    "wildtomorrow",
    [
      {
        // WTF		Manual Payment from us to WTF	https://etherscan.io/tx/0xefa0c143731e2209a7d5d7ec2dbf0e8a3fef31c4da39c71e4badb4597b8e2bb6	3.516220128		1,330.78	4679.315422	
        txHash: "0xefa0c143731e2209a7d5d7ec2dbf0e8a3fef31c4da39c71e4badb4597b8e2bb6",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="3.516220128"),
        amountInUsd: 4679.32,
        networkId: 1,
      },
    ],
  ),
  (
    "wildtomorrow",
    [
      {
        // Wild Tomorrow Fund				https://etherscan.io/tx/0xc69f5e9dbe2d3db93955c9346d4a8fd39b72cc67f33bc3a5a7b99b9653573363	10.31977989		1,368.22	14119.72923																						
        txHash: "0xc69f5e9dbe2d3db93955c9346d4a8fd39b72cc67f33bc3a5a7b99b9653573363",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="10.31977989"),
        amountInUsd: 14119.73,
        networkId: 1,
      },
    ],
  ),
  (
    "wildtomorrow",
    [
      {
        // https://etherscan.io/tx/0xe4b790d893721e0ec2e6776514200081d658a6e9b09563a84fe0e91d0c447e37	1.605126075		1,964.18	3152.756533	
        txHash: "0xe4b790d893721e0ec2e6776514200081d658a6e9b09563a84fe0e91d0c447e37",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="1.605126075"),
        amountInUsd: 3152.756533,
        networkId: 1,
      },
    ],
  ),
  (
    "greatwhaleconservancy",
    [
      {
        // Great Whale			https://etherscan.io/tx/0x71840a46150b1dc94c8bf4b9d3d886d43f16e5fb8af2933b9ff8d5fc52ac0130	1.984149104		1,368.22	2714.752487	
        txHash: "0xf5b33c84b77827b775d9dfa516113c596706ad5ad8d0cc6c9a456d43258caaf4",
        amount: Ethers.Utils.parseEtherUnsafe(~amount="0.2841441781"),
        amountInUsd: 388.77,
        networkId: 1,
      },
    ],
  ),
])

let filterOutOrgs = ["sasharkconservancy"]
let filterOutWildcards = ["66", "30", "52", "44", "47", "50"]
