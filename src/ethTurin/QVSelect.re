open Globals;

// module CustomVote = {
//   [@react.component]
//   let make =
//       (
//         ~voteValue: float,
//         ~customVote: float => unit,
//         ~makeVote: float => unit,
//         ~maxVote: float,
//       ) => {
//   };
// };

let hasDecimals = value => value -. value->Float.toInt->Float.fromInt > 0.;

[@react.component]
let make = (~submitVoteFunction: float => unit, ~maxVote: float) => {
  let (voteValue, setVoteValue) = React.useState(_ => 0.);
  let (voteText, setVoteText) = React.useState(_ => "0.5");
  let customVote: string => unit =
    voteValueString => {
      let optVoteValue = Float.fromString(voteValueString);

      switch (optVoteValue) {
      | None =>
        if (voteValueString == "") {
          setVoteText(_ => voteValueString);
        } else {
          ();
            // If the text doesn't parse as a float, don't save it
        }
      | Some(voteValue) =>
        setVoteText(_ => voteValueString);
        if (voteValue < 0.) {
          // If negative set to 0
          setVoteValue(_ => 0.);
        } else if (voteValue >= maxVote) {
          // If above max, set to max
          setVoteValue(_ => maxVote);
        } else {
          setVoteValue(_ => voteValue);
        };
      };
    };

  <>
    {ReasonReact.array(
       [|1., 2., 3., 4., 5.|]
       ->Array.map(x => {
           let disabled = x >= maxVote;
           Js.log3(x, "<= maxVote", disabled);
           <Rimble.Button
             key={x->Float.toString}
             onClick={_ => submitVoteFunction(x)}
             disabled>
             {(
                {
                  x;
                }->Float.toString
                ++ " Vote = "
                ++ {
                     x *. x;
                   }
                   ->Float.toString
                ++ " Loyalty Token"
              )
              ->restr}
           </Rimble.Button>;
         }),
     )}
    <div>
      <form>
        <input
          value=voteText
          type_="number"
          min=0
          max={maxVote->Float.toString}
          onChange={event => {
            let voteString = ReactEvent.Form.target(event)##value;
            customVote(voteString);
          }}
        />
        <button onClick={_ => submitVoteFunction(voteValue)}>
          "Vote"->restr
        </button>
      </form>
      <br />
      <small>
        {voteValue->Float.toString->restr}
        " votes = "->restr
        {(voteValue *. voteValue)->Float.toString->restr}
        " loyalty tokens"->restr
      </small>
    </div>
  </>;
};
