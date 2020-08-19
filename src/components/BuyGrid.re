open Globals;

let backgroundStyle =
  Css.(
    style([
      paddingTop(rem(1.)),
      unsafe(
        "backgroundImage",
        "linear-gradient(to top, #74C7D7 0%, white 100%)",
      ),
    ])
  );
let headingStyle = Css.(style([paddingTop(rem(5.)), textAlign(`center)]));

[@react.component]
let make = () => {
  let isGqlLoaded = QlStateManager.useIsInitialized();
  let allAnimals = QlHooks.useAnimalList();

  <div className=backgroundStyle>
    <div> <h1 className=headingStyle> "Wildcards Kingdom"->restr </h1> </div>
    <Rimble.Flex
      flexWrap="wrap" justifyContent="space-around" alignItems="stretch" px=50>
      {allAnimals
       ->Array.map(animal =>
           <Rimble.Box
             key={animal->TokenId.toString}
             fontSize=4
             p=3
             width=[|1., 1., 0.3|]>
             <Rimble.Card>
               <Dapp.CarouselAnimal animal isGqlLoaded scalar=1. />
             </Rimble.Card>
           </Rimble.Box>
         )
       ->React.array}
      ///* Always empty to ensure grid structure */}
      <Rimble.Box fontSize=4 p=3 width=[|1., 1., 0.3|]>
        React.null
      </Rimble.Box>
      <Rimble.Box fontSize=4 p=3 width=[|1., 1., 0.3|]>
        React.null
      </Rimble.Box>
    </Rimble.Flex>
  </div>;
};
