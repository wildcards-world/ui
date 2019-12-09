open Hooks;
open Belt.Option;

let getToDisplay = (label, value) =>
  React.string(label ++ ": " ++ value->mapWithDefault("loading", a => a));
module Transaction = {
  [@react.component]
  let make = () => {
    let currentPrice = useCurrentPriceWei();
    let currentUser = useCurrentUser();
    let currentPatron = useCurrentPatron();
    let availableDeposit = Hooks.useDepositAbleToWithdrawWei();
    Js.log("vitalik --- ");
    Js.log(currentPrice);
    Js.log(currentUser);
    Js.log(currentPatron);
    Js.log(availableDeposit);

    let exitFunc = {
      let exitObj = useExitTransaction();
      txObject => {
        Js.log(txObject);
        exitObj##send(. txObject);
      };
    };

    let (buyFunc, txObjects) = {
      let buyObj = useBuyTransactionNew();
      (
        (newPrice, txObject) =>
          buyObj##send(. "42", newPrice->Web3Utils.toWeiFromEth, txObject),
        buyObj##_TXObjects,
      );
    };

    let onUpgrade = event => {
      ReactEvent.Form.preventDefault(event);

      // TODO: Abstract this better into a utility library of sorts.
      let setFunctionObj = [%bs.raw {| (from) => ({ from }) |}];
      exitFunc(setFunctionObj(. currentUser));

      let setFunctionValueObj = [%bs.raw
        {| (value, from) => ({ value, from }) |}
      ];
      buyFunc(
        currentPrice->Belt.Option.mapWithDefault("0", a => a),
        setFunctionValueObj(.
          "7500000000000000",
          // availableDeposit->Belt.Option.mapWithDefault("0", a => a),
          currentUser,
        ),
      );
      ();
    };

    <TxTemplate txObjects>
      <Rimble.Box p=4 mb=3>
        <Rimble.HeadingS> "Migrate Vitalik" </Rimble.HeadingS>
        <Rimble.TextS>
          "Clicking below will generate 2 transactions."
        </Rimble.TextS>
        <Rimble.TextS>
          "1. withdraw your full deposit from the legacy contract."
        </Rimble.TextS>
        <Rimble.TextS>
          "2. buy the new token with the same deposit, and price as the old token."
        </Rimble.TextS>
        <br />
        {switch (currentUser, currentPrice, availableDeposit, currentPatron) {
         | (Some(user), Some(_), Some(_), Some(patron)) =>
           if (user == patron) {
             <Rimble.Button onClick=onUpgrade>
               {React.string("Upgrade")}
             </Rimble.Button>;
           } else {
             <p>
               "you don't own vitalik, only the owner of vitalik can migrate it."
               ->React.string
             </p>;
           }
         | _ => <p> "Loading the details of the token"->React.string </p>
         }}
      </Rimble.Box>
    </TxTemplate>;
  };
};

[@react.component]
let make = () => {
  let (isModalOpen, setModalOpen) = React.useState(() => false);
  let isProviderSelected = Providers.DrizzleProvider.useIsProviderSelected();

  let onUnlockMetamaskAndOpenModal = () => {
    setModalOpen(_ => true);
  };
  let onOpenModal = event => {
    ReactEvent.Form.preventDefault(event);
    ReactEvent.Form.stopPropagation(event);
    setModalOpen(_ => true);
  };

  <React.Fragment>
    {if (isProviderSelected) {
       <Rimble.Button onClick=onOpenModal>
         {React.string("Migrate Vitalik")}
       </Rimble.Button>;
     } else {
       <Web3connect.CustomButton afterConnect=onUnlockMetamaskAndOpenModal>
         {React.string("Connect Web 3 and Migrate Vitalik")}
       </Web3connect.CustomButton>;
     }}
    <Rimble.Modal isOpen=isModalOpen>
      <Rimble.Card width={Rimble.AnyStr("420px")} p=0>
        <Rimble.Button.Text
          icononly=true
          icon="Close"
          color="moon-gray"
          position="absolute"
          top=0
          right=0
          m=3
          onClick={_ => setModalOpen(_ => false)}
        />
        <Transaction />
      </Rimble.Card>
    </Rimble.Modal>
  </React.Fragment>;
};
