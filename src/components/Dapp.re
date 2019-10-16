open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;
open Components;

// TODO: there must be a better way of importing images in reason react...
let gorilla1 = [%bs.raw {|require('../img/gorillas/gorilla1.png')|}];
let gorilla2 = [%bs.raw {|require('../img/gorillas/gorilla2.png')|}];
let gorilla3 = [%bs.raw {|require('../img/gorillas/gorilla3.png')|}];

module DefaultLook = {
  open Gorilla;

  [@react.component]
  let make = (~areRequirementsLoaded: bool=false) => {
    let setProvider = useSetProvider();
    let isProviderSelected = useIsProviderSelected();
    React.useEffect0(() => {
      // Setup the Web3connect component

      open Web3connect.Core;
      // let core = getCore(Some("goerli"));
      let core = getCore(None); // TOGGLE THE ABOVE LINE OUT BEFORE PRODUCTION!!
      core->setOnConnect(setProvider);
      None;
    });

    let owned =
      if (areRequirementsLoaded) {
        // NOTE/TODO: this doesn't take into account token ownership
        let currentPatronVitalik =
          useCurrentPatron()->mapWithDefault("no-patron-defined", a => a);
        let currentPatronSimon =
          useCurrentPatronNew(0)->mapWithDefault("no-patron-defined", a => a);
        let currentPatronAndy =
          useCurrentPatronNew(1)->mapWithDefault("no-patron-defined", a => a);
        let currentAccount =
          useCurrentUser()->mapWithDefault("no-current-account", a => a);

        if (currentAccount == currentPatronVitalik) {
          Vitalik;
        } else if (currentAccount == currentPatronSimon) {
          Simon;
        } else if (currentAccount == currentPatronAndy) {
          Andy;
        } else {
          None;
        };
      } else {
        None;
      };

    <div className=Styles.rightTopHeader>
      {switch (owned) {
       | Simon =>
         <React.Fragment>
           <img className=Styles.ownedGorillaImg src=gorilla2 />
           <h2> {React.string("Simon")} </h2>
           <PriceDisplay tokenId={Some("0")} />
           <UpdatePriceModal gorilla=owned />
           <UpdateDeposit gorilla=owned />
         </React.Fragment>
       | Vitalik =>
         <React.Fragment>
           <img className=Styles.ownedGorillaImg src=gorilla1 />
           <h2> {React.string("Vitalik")} </h2>
           <PriceDisplay tokenId=None />
           <UpdatePriceModal gorilla=owned />
           <UpdateDeposit gorilla=owned />
         </React.Fragment>
       | Andy =>
         <React.Fragment>
           <img className=Styles.ownedGorillaImg src=gorilla1 />
           <h2> {React.string("Andy")} </h2>
           <PriceDisplay tokenId={Some("1")} />
           <UpdatePriceModal gorilla=owned />
           <UpdateDeposit gorilla=owned />
         </React.Fragment>
       | None =>
         <Rimble.Flex className=Styles.gorillaBox>
           <Rimble.Box>
             <div className=Styles.gorillaBack>
               <img className={Styles.headerImg(140.)} src=gorilla2 />
               <div className=Styles.gorillaText>
                 <h2> {React.string("Simon")} </h2>
                 <Offline requireSmartContractsLoaded=true>
                   <PriceDisplay tokenId={Some("0")} />
                   <BuyModal tokenId={Some("0")} />
                 </Offline>
               </div>
             </div>
           </Rimble.Box>
           <Rimble.Box>
             <img className={Styles.headerImg(155.)} src=gorilla1 />
             <div>
               <div className=Styles.gorillaText>
                 <h2> {React.string("Vitalik")} </h2>
                 <Offline requireSmartContractsLoaded=true>
                   <PriceDisplay tokenId=None />
                   <BuyModal tokenId=None />
                 </Offline>
               </div>
             </div>
           </Rimble.Box>
           <Rimble.Box>
             <div className=Styles.gorillaBack>
               <img className={Styles.headerImg(140.)} src=gorilla3 />
               <div className=Styles.gorillaText>
                 <h2> {React.string("Andy")} </h2>
                 <Offline requireSmartContractsLoaded=true>
                   <PriceDisplay tokenId={Some("1")} />
                   <BuyModal tokenId={Some("1")} />
                 </Offline>
               </div>
             </div>
           </Rimble.Box>
         </Rimble.Flex>
       }}
      <Rimble.Box className=Styles.dappImagesCounteractOffset>
        <Offline requireSmartContractsLoaded=true> <TotalRaised /> </Offline>
      </Rimble.Box>
    </div>;
  };
};

[@react.component]
// The Offline container here shows the website, but without loading the requirements
let make = () => {
  <Providers.UsdPriceProvider>

      <Offline
        requireSmartContractsLoaded=true
        alturnateLoaderWeb3={<DefaultLook key="a" />}
        alturnateLoaderSmartContracts={<DefaultLook key="b" />}>
        <DefaultLook areRequirementsLoaded=true key="c" />
      </Offline>
    </Providers.UsdPriceProvider>; //alturnativeNoWeb3=DefaultLook
};
