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
  [@react.component]
  let make = (~areRequirementsLoaded: bool=false) => {
    let isCurrentOwner =
      if (areRequirementsLoaded) {
        let currentPatron =
          useCurrentPatron()->mapWithDefault("no-patron-defined", a => a);
        let currentAccount =
          useCurrentUser()->mapWithDefault("no-current-account", a => a);
        currentAccount == currentPatron;
      } else {
        false;
      };

    <div className=Styles.rightTopHeader>
      {if (isCurrentOwner) {
         <React.Fragment>
           <img className=Styles.headerImg src=gorilla1 />
           <UpdatePriceModal />
           <Rimble.Button>
             {React.string("Add/Remove Deposit")}
           </Rimble.Button>
         </React.Fragment>;
       } else {
         <Rimble.Flex className=Styles.gorillaBox>
           <Rimble.Box>
             <div className=Styles.gorillaBack>
               <img className=Styles.headerImg src=gorilla1 />
               <div className=Styles.gorillaText>
                 <h2> {React.string("Simon")} </h2>
                 <h3 className=Styles.colorGreen>
                   {React.string("COMING SOON")}
                 </h3>
               </div>
             </div>
           </Rimble.Box>
           <Rimble.Box>
             <img className=Styles.headerImg src=gorilla2 />
             <div>
               <div className=Styles.gorillaText>
                 <h2> {React.string("Vitalik")} </h2>
                 <Offline requireSmartContractsLoaded=true>
                   <BuyModal />
                 </Offline>
               </div>
             </div>
           </Rimble.Box>
           <Rimble.Box>
             <div className=Styles.gorillaBack>
               <img className=Styles.headerImg src=gorilla3 />
               <div className=Styles.gorillaText>
                 <h2> {React.string("Andy")} </h2>
                 <h3 className=Styles.colorGreen>
                   {React.string("COMING SOON")}
                 </h3>
               </div>
             </div>
           </Rimble.Box>
         </Rimble.Flex>;
       }}
      <Offline requireSmartContractsLoaded=true> <TotalRaised /> </Offline>
    </div>;
  };
};
[@react.component]
// The Offline container here shows the website, but without loading the requirements
let make = () => {
  <Providers.UsdPriceProvider>

      <Offline
        requireSmartContractsLoaded=true
        alturnateLoaderWeb3={<DefaultLook />}
        alturnateLoaderSmartContracts={<DefaultLook />}>
        <DefaultLook areRequirementsLoaded=true />
      </Offline>
    </Providers.UsdPriceProvider>; //alturnativeNoWeb3=DefaultLook
};
