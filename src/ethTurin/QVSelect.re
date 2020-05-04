open Globals;

// 10000
// 100000000
[@react.component]
let make =
    (
      ~selectVote: int => unit,
      ~makeCustomVote: float => unit,
      ~maxVote: float,
    ) => {
  Js.log2("Max Vote", maxVote);
  let (voteValue, setVoteValue) = React.useState(_ => 0.);
  let customVote: float => unit =
    voteValue =>
      if (voteValue < 0.) {
        // If negative set to 0
        setVoteValue(_ => 0.);
      } else if (voteValue >= maxVote) {
        // If above max, set to max
        setVoteValue(_ => maxVote);
      } else {
        setVoteValue(_ => voteValue);
      };
  <>
    {ReasonReact.array(
       [|1, 2, 3, 4, 5|]
       ->Array.map(x => {
           let disabled = x->float_of_int >= maxVote;
           Js.log3(x, "<= maxVote", disabled);
           <Rimble.Button
             key={x->string_of_int} onClick={_ => selectVote(x)} disabled>
             //  value={x->string_of_int}

               {(
                  {
                    x;
                  }->string_of_int
                  ++ " Vote = "
                  ++ {
                       x * x;
                     }
                     ->string_of_int
                  ++ " Loyalty Token"
                )
                ->restr}
             </Rimble.Button>;
         }),
     )}
    <CustomVote maxVote voteValue customVote makeVote=makeCustomVote />
  </>;
};