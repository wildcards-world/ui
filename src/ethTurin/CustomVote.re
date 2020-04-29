open Globals;

[@react.component]
let make =
    (
      ~redeemedLoyaltyTokenBalance: float,
      ~voteValue: int,
      ~customVote: int => unit,
      ~makeVote: _ => unit,
    ) => {
  <div>
    <form>
      <input
        value={voteValue->Int.toString}
        type_="number"
        min=0
        max={redeemedLoyaltyTokenBalance->Float.toString}
        onChange={event => customVote(ReactEvent.Form.target(event)##value)}
      />
      <button onClick={_ => makeVote()}> "Vote"->restr </button>
    </form>
    <br />
    <small>
      {voteValue->Int.toString->restr}
      " votes = "->restr
      {(voteValue * voteValue)->Int.toString->restr}
      " loyalty tokens"->restr
    </small>
  </div>;
};