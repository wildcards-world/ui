open Globals;

[@react.component]
let make = () => {
  let (increaseIteration, _increaseIterationTxState) =
    AnimalActions.useIncreaseVoteIteration();
  <Rimble.Box className=Styles.topBody>
    <Rimble.Box>
      <Rimble.Button
        onClick={_ => {
          Js.log("clicked");
          increaseIteration();
        }}>
        "Increase iteration"->restr
      </Rimble.Button>
    </Rimble.Box>
  </Rimble.Box>;
};
