open Globals;

module CustomVote = {
  [@react.component]
  let make =
      (
        ~voteValue: float,
        ~customVote: float => unit,
        ~makeVote: float => unit,
        ~maxVote: float,
      ) => {
    <div>
      <form>
        <input
          value={voteValue->Float.toString}
          // type_="number"
          min=0
          max={maxVote->Float.toString}
          onChange={event => {
            let voteString = ReactEvent.Form.target(event)##value;
            let voteFloat = Float.fromString(voteString) |||| 0.;
            customVote(voteFloat);
          }}
        />
        <button onClick={_ => makeVote(voteValue)}> "Vote"->restr </button>
      </form>
      <br />
      <small>
        {voteValue->Float.toString->restr}
        " votes = "->restr
        {(voteValue *. voteValue)->Float.toString->restr}
        " loyalty tokens"->restr
      </small>
    </div>;
  };
};
[@react.component]
let make =
    (
      ~selectVote: int => unit,
      ~makeCustomVote: float => unit,
      ~maxVote: float,
    ) => {
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
