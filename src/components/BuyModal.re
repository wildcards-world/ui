open Globals;
open Accounting;

[@react.component]
let make = (~animal: TokenId.t, ~isExplorer: bool=false) => {
  let currentPriceWei = QlHooks.usePrice(animal);

  let goToBuy = RootProvider.useGoToBuy();
  let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute();

  let buttonText =
    switch (currentPriceWei) {
    | Price(_price) => "Buy"
    | Foreclosed => "Adopt"
    | Loading => "loading"
    };

  <React.Fragment>
    <Rimble.Button
      onClick={_e => {
        clearAndPush(
          "#"
          ++ InputHelp.getPagePrefix(isExplorer)
          ++ "details/"
          ++ animal->TokenId.toString,
        );
        goToBuy(animal);
      }}>
      {React.string(buttonText)}
    </Rimble.Button>
  </React.Fragment>;
};
