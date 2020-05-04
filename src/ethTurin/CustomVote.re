open Globals;

[@react.component]
let make =
    (
      ~voteValue: float,
      ~customVote: float => unit,
      ~makeVote: _ => unit,
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
      <button onClick={_ => makeVote()}> "Vote"->restr </button>
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