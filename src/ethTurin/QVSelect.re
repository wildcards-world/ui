open Globals;

[@react.component]
let make = (~selectVote: int => unit, ~redeemedLoyaltyTokenBalance: float) => {
  Js.log(redeemedLoyaltyTokenBalance);
  <select
    onChange={event => {
      let value: string = ReactEvent.Form.target(event)##value;
      selectVote(value->Int.fromString |||| (-1));
    }}>
    <option value="none-selected"> "Select an option"->restr </option>
    {ReasonReact.array(
       [|1, 2, 3, 4, 5, 6, 7, 8, 9, 10|]
       ->Array.map(x =>
           <option
             key={x->string_of_int}
             value={x->string_of_int}
             disabled={(x * x)->Float.fromInt > redeemedLoyaltyTokenBalance}>
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
           </option>
         ),
     )}
    <option value={redeemedLoyaltyTokenBalance->Float.toString}>
      {redeemedLoyaltyTokenBalance->Float.toString->restr}
    </option>
    <option value="-1"> "custom"->restr </option>
  </select>;
};