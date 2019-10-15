type gorilla =
  | Andy
  | Vitalik
  | Simon
  | None;

let getId: gorilla => option(string) =
  aGorilla =>
    switch (aGorilla) {
    | Simon => Some("0")
    | Andy => Some("1")
    // | Vitalik => "42",
    | _ => None
    };

let getName: option(string) => string = gorillaId =>
      switch (gorillaId) {
      | None => "Vitalik"
      | Some(tokenIdSet) => 
          switch (tokenIdSet) {
            | "0" => "Simon"
            | "1" => "Andy"
            | _ => "Unknown"
          }
      };
