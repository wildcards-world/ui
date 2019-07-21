open Hooks;
open Providers.UsdPriceProvider;
open Providers.DrizzleProvider;
open Belt.Option;

// TODO: there must be a better way of importing images in reason react...
let gorilla1 = [%bs.raw {|require('../img/gorillas/gorilla1.png')|}];
let gorilla2 = [%bs.raw {|require('../img/gorillas/gorilla2.png')|}];
let gorilla3 = [%bs.raw {|require('../img/gorillas/gorilla3.png')|}];

// [@bs.module "../../web3ProvideSwitcher"]
// external toWei: unit => bool = "toWei";
let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));

module DefaultLook = {
  [@react.component]
  let make = () => {
    <div className=Styles.rightTopHeader>
      <div className=Styles.gorillaColumn>
        <div className=Styles.gorillas>
          <div
            className={
              Styles.gorillaBack ++ " " ++ Styles.gorillaTransform(-10., 3.)
            }>
            <img className=Styles.headerImg src=gorilla2 />
            <div className={Styles.gorillaTransform(-5., 0.)}>
              <h2> {React.string("Simon")} </h2>
              <h3 className=Styles.colorGreen>
                {React.string("COMING SOON")}
              </h3>
            </div>
          </div>
          <div className=Styles.gorillaCentreFront>
            <img className=Styles.headerImg src=gorilla1 />
            <div>
              <div className="gorilla-purchase-container">
                <h2> {React.string("Vitalik")} </h2>
              </div>
              <Providers.UsdPriceProvider>
                <Offline
                  requireSmartContractsLoaded=true
                  >
                  <BuyModal />
                </Offline>
              </Providers.UsdPriceProvider>
            </div>
          </div>
          <div
            className={
              Styles.gorillaBack ++ " " ++ Styles.gorillaTransform(10., 1.)
            }>
            <img className=Styles.headerImg src=gorilla3 />
            <div className={Styles.gorillaTransform(5., -1.)}>
              <h2> {React.string("Andy")} </h2>
              <h3 className=Styles.colorGreen>
                {React.string("COMING SOON")}
              </h3>
            </div>
          </div>
        </div>
      </div>
    </div>;
  };
};
[@react.component]
let make = () => {
        <DefaultLook/>;
};
