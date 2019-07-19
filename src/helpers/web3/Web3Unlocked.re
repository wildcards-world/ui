[@bs.module "./web3ProvideSwitcher"] external default: 'providerSwitcher = "";

let isUnlocked: unit => bool = () => default##providerIsUnlocked;
let unlock: unit => bool = default##switchToInjectedWeb3;

[@react.component]
let make = () => {
  let isConnected: bool = default##providerIsUnlocked;
  <div>
    <p>
      {if (isConnected) {
         React.string("connected!");
       } else {
         React.string("NOOOT CONNECTED YET :/");
       }}
    </p>
  </div>;
};
