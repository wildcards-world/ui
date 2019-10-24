open Hooks;
open Providers.DrizzleProvider;
open Belt.Option;

// TODO: there must be a better way of importing images in reason react...
let gorilla1 = [%bs.raw {|require('../img/gorillas/gorilla1.png')|}];
let gorilla2 = [%bs.raw {|require('../img/gorillas/gorilla2.png')|}];
let gorilla3 = [%bs.raw {|require('../img/gorillas/gorilla3.png')|}];

module ShareSocial = {
  [@bs.module "./components/shareSocialMedia"] [@react.component]
  external make: unit => React.element = "default";
};

module DefaultLook = {
  open Gorilla;

  [@react.component]
  let make = (~areRequirementsLoaded: bool=false) => {
    open Components;
    let setProvider = useSetProvider();
    React.useEffect0(() => {
      open Web3connect.Core;
      let core = getCore(None); // TOGGLE THE ABOVE LINE OUT BEFORE PRODUCTION!!
      core->setOnConnect(setProvider);
      None;
    });

    let (ownVitalik, ownSimon, ownAndy) =
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

        (
          currentAccount == currentPatronVitalik,
          currentAccount == currentPatronSimon,
          currentAccount == currentPatronAndy,
        );
      } else {
        (false, false, false);
      };
    let url = ReasonReactRouter.useUrl();

    <div className=Styles.rightTopHeader>
      {switch (Js.String.split("/", url.hash)) {
       // switch (url.hash) {
       | [|"details", gorillaStr|] =>
         let gorilla = getGorilla(gorillaStr);
         let tokenId = getId(gorilla);
         switch (gorilla) {
         | NoGorilla =>
           <div>
             <h1>
               {React.string(
                  "We are unable to find a gorilla by the name of "
                  ++ gorillaStr
                  ++ " in our system.",
                )}
             </h1>
             <p> <S> "Please check the spelling and try again." </S> </p>
           </div>
         | _ =>
           <React.Fragment>
             <img className=Styles.ownedGorillaImg src={getImage(gorilla)} />
             <h2> <S> {getName(gorilla)} </S> </h2>
             <Offline requireSmartContractsLoaded=true>
               {
                 let isYours =
                   switch (gorilla) {
                   | Vitalik => ownVitalik
                   | Simon => ownSimon
                   | Andy => ownAndy
                   | _ => false
                   };

                 if (isYours) {
                   <React.Fragment>
                     <UpdatePriceModal gorilla />
                     <br />
                     <UpdateDeposit gorilla />
                     <br />
                     <ShareSocial />
                   </React.Fragment>;
                 } else {
                   <React.Fragment>
                     <PriceDisplay tokenId />
                     <BuyModal tokenId />
                   </React.Fragment>;
                 };
               }
             </Offline>
           </React.Fragment>
         };
       | _ =>
         <React.Fragment>
           <Rimble.Flex className=Styles.gorillaBox>
             <Rimble.Box
               onClick={() =>
                 ReasonReactRouter.push("#details/" ++ getName(Simon))
               }>
               <div className=Styles.gorillaBack>
                 <img className={Styles.headerImg(140.)} src=gorilla2 />
                 <div className=Styles.gorillaText>
                   <h2> {React.string("Simon")} </h2>
                   <Offline requireSmartContractsLoaded=true>
                     {if (ownSimon) {
                        <React.Fragment>
                          <UpdatePriceModal gorilla=Simon />
                          <br />
                          <UpdateDeposit gorilla=Simon />
                          <br />
                          <ShareSocial />
                        </React.Fragment>;
                      } else {
                        <React.Fragment>
                          <PriceDisplay tokenId={Some("0")} />
                          <BuyModal tokenId={Some("0")} />
                        </React.Fragment>;
                      }}
                   </Offline>
                   <p>
                     <small> <S> "Harberger Tax" </S> </small>
                     <br />
                     <small> <S> "20% per month" </S> </small>
                   </p>
                 </div>
               </div>
             </Rimble.Box>
             <Rimble.Box
               onClick={() =>
                 ReasonReactRouter.push("#details/" ++ getName(Vitalik))
               }>
               <img className={Styles.headerImg(155.)} src=gorilla1 />
               <div>
                 <div className=Styles.gorillaText>
                   <h2> {React.string("Vitalik")} </h2>
                   <Offline requireSmartContractsLoaded=true>
                     {if (ownVitalik) {
                        <React.Fragment>
                          <UpdatePriceModal gorilla=Vitalik />
                          <br />
                          <UpdateDeposit gorilla=Vitalik />
                          <br />
                          <ShareSocial />
                        </React.Fragment>;
                      } else {
                        <React.Fragment>
                          <PriceDisplay tokenId=None />
                          <BuyModal tokenId=None />
                        </React.Fragment>;
                      }}
                   </Offline>
                   <p>
                     <small> <S> "Harberger Tax" </S> </small>
                     <br />
                     <small> <S> "2.5% per month" </S> </small>
                   </p>
                 </div>
               </div>
             </Rimble.Box>
             <Rimble.Box
               onClick={() =>
                 ReasonReactRouter.push("#details/" ++ getName(Andy))
               }>
               <div className=Styles.gorillaBack>
                 <img className={Styles.headerImg(140.)} src=gorilla3 />
                 <div className=Styles.gorillaText>
                   <h2> {React.string("Andy")} </h2>
                   <Offline requireSmartContractsLoaded=true>
                     {if (ownAndy) {
                        <React.Fragment>
                          <UpdatePriceModal gorilla=Andy />
                          <br />
                          <UpdateDeposit gorilla=Andy />
                          <br />
                          <ShareSocial />
                        </React.Fragment>;
                      } else {
                        <React.Fragment>
                          <PriceDisplay tokenId={Some("1")} />
                          <BuyModal tokenId={Some("1")} />
                        </React.Fragment>;
                      }}
                   </Offline>
                   <p>
                     <small> <S> "Harberger Tax" </S> </small>
                     <br />
                     <small> <S> "20% per month" </S> </small>
                   </p>
                 </div>
               </div>
             </Rimble.Box>
           </Rimble.Flex>
           <Rimble.Box className=Styles.dappImagesCounteractOffset>
             <Offline requireSmartContractsLoaded=true>
               <TotalRaised />
             </Offline>
           </Rimble.Box>
           <Rimble.Box className=Styles.dappImagesCounteractOffset>
             <p>
               <small>
                 <S> "Having problems buying? " </S>
                 <a
                   href="https://blog.wildcards.world/how-to-buy-a-wildcard-web3-ethereum/"
                   target="_blank">
                   <S> "Read our guide" </S>
                 </a>
               </small>
             </p>
           </Rimble.Box>
         </React.Fragment>
       }}
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
