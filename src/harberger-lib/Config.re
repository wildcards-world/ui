[@bs.val] [@bs.scope ("window", "location")]
external windowLocation: string = "origin";

module Transak = {
  let getConfig = (~chain, web3Context: RootProvider.web3reactContext) => {
    let (defaultCryptoCurrency, defaultNetwork) =
      switch (chain) {
      | Client.Neither
      | Client.MainnetQuery => ("ETH", "mainnet")
      | Client.MaticQuery => ("DAI", "matic")
      };
    let (environment, apiKey) =
      switch (web3Context.chainId) {
      | Some(1) => ("PRODUCTION", "e7f543f7-e12e-4257-ad04-682679f0404c")
      | Some(_)
      | None => ("STAGING", "e2c87df4-4d03-49a2-8b1c-899a8bcf13eb")
      };

    let transakConfig: Transak.initParams = {
      environment,
      apiKey,
      // apiKey: "e7f543f7-e12e-4257-ad04-682679f0404c",
      hostURL: windowLocation,
      defaultCryptoCurrency: Some(defaultCryptoCurrency),
      walletAddress: web3Context.account,
      defaultNetwork: Some(defaultNetwork),
      themeColor: Some("#6BAD3E"),
      exchangeScreenTitle: Some("Top up your balance for Wildcards"),
      isDisableCrypto: Some(true), // This could be interesting... Doesn't work unless other parameters are correct though.
      cryptoCurrencyCode: None,
      cryptoCurrencyList: None,
      networks: None,
      walletAddressesData: None,
      fiatCurrency: None, // TODO
      countryCode: None,
      fiatAmount: None,
      defaultFiatAmount: None,
      paymentMethod: None,
      defaultPaymentMethod: None,
      disablePaymentMethods: None,
      email: None,
      userData: None,
      partnerOrderId: None,
      partnerCustomerId: None,
      accessToken: None,
      redirectURL: None,
      disableWalletAddressForm: None,
      isAutoFillUserData: None,
      height: None,
      width: None,
      hideMenu: None,
      hideExchangeScreen: None,
      isFeeCalculationHidden: None,
    };

    transakConfig;
  };
};
