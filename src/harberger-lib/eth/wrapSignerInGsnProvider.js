const { RelayProvider, configureGSN } = require("@opengsn/gsn");
const Web3HttpProvider = require("web3-providers-http");
var PrivateKeyProvider = require("truffle-privatekey-provider");
const ethers = require("ethers");

const useGsn = true;

export default (origProvider, externalSigner, address) => {
  // console.log({ origProvider });

  // const web3provider = new Web3HttpProvider("http://localhost:8545");
  const web3provider = new Web3HttpProvider(
    "https://kovan.infura.io/v3/c401b8ee3a324619a453f2b5b2122d7a"
  );

  // // Local:
  // const gsnConfig = configureGSN({
  //   relayHubAddress: "0xb068cfc28733c9b396559efaa3d4d90690b65fd7",
  //   paymasterAddress: "0x4af46ecdc266a58f0c0b49bc3930e5619b37e82e",
  //   stakeManagerAddress: "0xe3331b7f0e5f030fe46ea41ed85e29ada5b661df",
  //   trustedForwarderAddress: "0x97c588bf0e178cd5c56b26a6a26d483e1d94f93c",
  //   versionRegistry: "0x0c83e5937010c8946bca89eb6a2185006243acfb",
  //   gasPriceFactorPercent: 70,
  //   // methodSuffix: "_v4",
  //   // jsonStringifyRequest: true,
  //   // chainId: 42,
  //   relayLookupWindowBlocks: 1e5,
  //   verbose: true,
  // }); // gsnConfig

  // Kovan
  const gsnConfig = configureGSN({
    relayHubAddress: "0xE9dcD2CccEcD77a92BA48933cb626e04214Edb92",
    paymasterAddress: "0x083082b7Eada37dbD8f263050570B31448E61c94",
    stakeManagerAddress: "0x93e4F8d0904a52F8a7304066499a6B2B77260ce1",
    trustedForwarderAddress: "0x0842Ad6B8cb64364761C7c170D0002CC56b1c498",
    versionRegistry: "0x570EFB87a19367cDbD715039b4aC38554b308896",
    // gasPriceFactorPercent: 70,
    // methodSuffix: "_v4",
    // jsonStringifyRequest: true,
    chainId: 42,
    // relayLookupWindowBlocks: 1e5,
    verbose: true,
  }); // gsnConfig

  // console.log({ from });
  // console.log(from);
  var privateKey =
    "dd723f2c7dcbf6f4914d2f940d0f06cb3caf352947fd786cd7edb5f4c25c8276";

  let etherProvider;
  if (useGsn) {
    // let gsnProvider = new RelayProvider(web3provider, gsnConfig);
    let gsnProvider = new RelayProvider(web3provider, gsnConfig);

    // const account = new ethers.Wallet(Buffer.from(privateKey, "hex"));
    // // // const account = new ethers.Wallet(Buffer.from("1".repeat(64), "hex"));
    // gsnProvider.addAccount({
    //   address: account.address,
    //   privateKey: Buffer.from(account.privateKey.replace("0x", ""), "hex"),
    // });

    // const from = account.address;

    etherProvider = new ethers.providers.Web3Provider(gsnProvider);

    // let signer = etherProvider.getSigner(address);
    // signer
    //   .signMessage("test message")
    //   .then((message, a) => console.log("SIGNER - signed message", message, a));
    // // console.log("SIGNER");
    // // console.log(signer._address);
    // // console.log(signer.address);
    // // console.log(signer.privateKey);
    // // signer.connect(etherProvider);
    let newSigner = externalSigner.connect(etherProvider);
    newSigner
      .signMessage("test message")
      .then((message, a) => console.log("SIGNER - signed message", message, a));

    return newSigner;
  } else {
    var privKeyProv = new PrivateKeyProvider(
      privateKey,
      "https://kovan.infura.io/v3/c401b8ee3a324619a453f2b5b2122d7a"
    );
    etherProvider = new ethers.providers.Web3Provider(privKeyProv);
  }
  // "0x4e9F3eaAe986CfD010758367880cd6a21d60Bf02"
  // var privKeyProv = new PrivateKeyProvider(privateKey, "https://kovan.infura.io/v3/c401b8ee3a324619a453f2b5b2122d7a");
  // var privKeyProv = new PrivateKeyProvider(privateKey, "http://localhost:8545");

  // const gsnProvider = new RelayProvider(privKeyProv, gsnConfig);
  // const gsnProvider = new RelayProvider(privKeyProv, gsnConfig);
  // const gsnProvider = privKeyProv;
  /*
  // const origProvider = window.ethereum;
  const gsnProvider = new RelayProvider(origProvider, gsnConfig);
  */
  // const provider = new ethers.providers.Web3Provider(origProvider);
  // const provider = new ethers.providers.Web3Provider(gsnProvider);
  // return etherProvider.getSigner("0xd3Cbce59318B2E570883719c8165F9390A12BdD6");
  // return new ethers.Wallet(provider);
  // return signer.connect(provider);
  // return new RelayProvider(provider, configuration);
};

// const conf = {
//   ourContract: "0x23Cd0E36bB4727550bc01Cd3A1E8931b6d7CC796",
//   notOurs: "0x6969Bc71C8f631f6ECE03CE16FdaBE51ae4d66B1",
//   paymaster: "0x0572dc46eb6edc950aa37c12fa9c862d4165cbc5",
//   relayhub: "0x2E0d94754b348D208D64d52d78BcD443aFA9fa52",
//   stakemgr: "0x0ecf783407C5C80D71CFEa37938C0b60BD255FF8",
//   gasPrice: 20000000000, // 20 Gwei
// };

// const flagAddr = conf.ourContract;
