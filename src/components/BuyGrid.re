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
module Heading = [%styled.h1 {|
  padding-top: 5rem;
  text-align: center;
|}];

[@react.component]
let make = () => {
  let isGqlLoaded = QlStateManager.useIsInitialized();

  <div className=backgroundStyle>
    <div> <Heading> "Wildcards Kingdom"->restr </Heading> </div>
    <Rimble.Flex
      flexWrap="wrap" justifyContent="space-around" alignItems="stretch" px=50>
      {Animal.orderedArray
       ->Array.mapWithIndex((i, animal) =>
           <Rimble.Box
             key={i->string_of_int} fontSize=4 p=3 width=[|1., 1., 0.3|]>
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
