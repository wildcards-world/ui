// const { RelayProvider } = require("@opengsn/gsn");

export default (origProvider, signer) => {
  console.log({ origProvider });
  // const configuration = {
  //   relayHubAddress: relayHub.address,
  //   stakeManagerAddress: stakeManager.address,
  // };
  const Gsn = require("@opengsn/gsn");
  const RelayProvider = Gsn.RelayProvider;

  // const configureGSN = require("@opengsn/gsn/dist/src/relayclient/GSNConfigurator")
  const configureGSN = require("@opengsn/gsn").configureGSN;

  const ethers = require("ethers");

  const gsnConfig = configureGSN({
    relayHubAddress: "0x2E0d94754b348D208D64d52d78BcD443aFA9fa52",
    paymasterAddress: "0x38489512d064106f5A7AD3d9e13268Aaf777A41c",
    stakeManagerAddress: "0x0ecf783407C5C80D71CFEa37938C0b60BD255FF8",
    trustedForwarderAddress: "0x6453D37248Ab2C16eBd1A8f782a2CBC65860E60B",
    gasPriceFactorPercent: 70,
    methodSuffix: "_v4",
    jsonStringifyRequest: true,
    chainId: 42,
    relayLookupWindowBlocks: 1e5,
  }); // gsnConfig
  var PrivateKeyProvider = require("truffle-privatekey-provider");
  // "0x4e9F3eaAe986CfD010758367880cd6a21d60Bf02"
  var privateKey = "dd723f2c7dcbf6f4914d2f940d0f06cb3caf352947fd786cd7edb5f4c25c8276";
  var privKeyProv = new PrivateKeyProvider(privateKey, "https://kovan.infura.io/v3/c401b8ee3a324619a453f2b5b2122d7a");
 
  // const gsnProvider = new RelayProvider(privKeyProv, gsnConfig);
  // const gsnProvider = new RelayProvider(privKeyProv, gsnConfig);
  const gsnProvider = privKeyProv;
  /*
  // const origProvider = window.ethereum;
  const gsnProvider = new RelayProvider(origProvider, gsnConfig);
  */
  // const provider = new ethers.providers.Web3Provider(origProvider);
  const provider = new ethers.providers.Web3Provider(gsnProvider);
  return provider.getSigner();
  // return new ethers.Wallet(provider);
  // return signer.connect(provider);
  // return new RelayProvider(provider, configuration);
};

// RelayHub: 0x2167016491369C0820F054FC92A301B97c4C0413
// StakeManager: 0xF9EF5090550E55d3408411d6D23361A1363f13b1
// Penalizer: 0x270d0a520f1921c476FeECba08Ff965a389d49C7
// VersionRegistry: 0xB8bAa0b4bd356b3f643047DF729F62F401F4DB2d
// Forwarder: 0x844849A90479a12FFc549c8Da98E362575FF78d7
// Paymaster (Default): 0x0C00CFE8EbB34fE7C31d4915a43Cde211e9F0F3B

// const conf = {
//   ourContract: "0x23Cd0E36bB4727550bc01Cd3A1E8931b6d7CC796",
//   notOurs: "0x6969Bc71C8f631f6ECE03CE16FdaBE51ae4d66B1",
//   paymaster: "0x0572dc46eb6edc950aa37c12fa9c862d4165cbc5",
//   relayhub: "0x2E0d94754b348D208D64d52d78BcD443aFA9fa52",
//   stakemgr: "0x0ecf783407C5C80D71CFEa37938C0b60BD255FF8",
//   gasPrice: 20000000000, // 20 Gwei
// };

// const flagAddr = conf.ourContract;
