type t = string;

// TODO: check that the string is the correct type
let make: string => option(t) =
  tokenId => Helper.isStringInteger(tokenId) ? Some(tokenId) : None;
let makeWithDefault: (string, int) => t =
  (tokenId, default) =>
    switch (make(tokenId)) {
    | Some(id) => id
    | None => default->Belt.Int.toString
    };
let makeFromInt: int => t = tokenId => tokenId->Belt.Int.toString;
