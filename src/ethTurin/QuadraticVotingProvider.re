let approveLoyaltyToken: _ => unit =
  () => {
    Js.log("approve loyalty token");
  };

let submitVoteToContract = (conservationSelection, numberOfVotes) => {
  Js.log("submit vote");
  Js.log2("conservation Selection: ", conservationSelection);
  Js.log2("votes: ", numberOfVotes);
};

let getVoteResults: _ => unit =
  () => {
    Js.log("get vote results");
  };