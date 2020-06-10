open Globals;

let orderedArray = [|
  TokenId.fromStringUnsafe("0"),
  TokenId.fromStringUnsafe("1"),
  TokenId.fromStringUnsafe("2"),
  TokenId.fromStringUnsafe("3"),
  TokenId.fromStringUnsafe("4"),
  TokenId.fromStringUnsafe("5"),
  TokenId.fromStringUnsafe("6"),
  TokenId.fromStringUnsafe("7"),
  TokenId.fromStringUnsafe("8"),
  TokenId.fromStringUnsafe("9"),
  TokenId.fromStringUnsafe("10"),
|];

let getName: TokenId.t => string = _ => "NEET TO IMPLEMENT";

let getNameFromId: string => string =
  animalId =>
    switch (animalId) {
    | "0" => "Simon"
    | "1" => "Andy"
    | "2" => "Apthapi"
    | "3" => "Aruma"
    | "4" => "CatStevens"
    | "5" => "Cubai"
    | "6" => "Llajuita"
    | "7" => "Pancho"
    | "8" => "Espumita"
    | "9" => "Verano"
    | "10" => "Nonhlanhla"
    | "11" => "Dlala"
    | "12" => "Isisa"
    | "13" => "Glen"
    | "14" => "Ucok"
    | "15" => "Tarkus"
    | "42" => "Vitalik"
    | _ => "Unknown"
    };

let lookupAnimalIdFromName = id =>
  switch (id) {
  | "simon" => Some(TokenId.fromStringUnsafe("0"))
  | "andy" => Some(TokenId.fromStringUnsafe("1"))
  | "apthapi" => Some(TokenId.fromStringUnsafe("2"))
  | "aruma" => Some(TokenId.fromStringUnsafe("3"))
  | "catstevens" => Some(TokenId.fromStringUnsafe("4"))
  | "cubai" => Some(TokenId.fromStringUnsafe("5"))
  | "llajuita" => Some(TokenId.fromStringUnsafe("6"))
  | "pancho" => Some(TokenId.fromStringUnsafe("7"))
  | "espumita" => Some(TokenId.fromStringUnsafe("8"))
  | "verano" => Some(TokenId.fromStringUnsafe("9"))
  | "nonhlanhla" => Some(TokenId.fromStringUnsafe("10"))
  | "dlala" => Some(TokenId.fromStringUnsafe("11"))
  | "isisa" => Some(TokenId.fromStringUnsafe("12"))
  | "glen" => Some(TokenId.fromStringUnsafe("13"))
  | "ucok" => Some(TokenId.fromStringUnsafe("14"))
  | "tarkus" => Some(TokenId.fromStringUnsafe("15"))
  | "vitalik" => Some(TokenId.fromStringUnsafe("42"))
  | _ => None
  };
let getAnimal: string => option(TokenId.t) =
  animal =>
    TokenId.make(animal)
    ->mapd(lookupAnimalIdFromName(animal), a => Some(a));

let getNextPrev = _animal => (
  TokenId.makeFromInt(0),
  TokenId.makeFromInt(1),
);

let getImage = animal => {
  switch (animal->TokenId.toString) {
  | "1"
  | "3"
  | "5"
  | "7"
  | "9" => "https://dd2wadt5nc0o7.cloudfront.net/7-pancho.png"
  | "10"
  | "8"
  | "6"
  | "2"
  | "4"
  | _ => "https://dd2wadt5nc0o7.cloudfront.net/7-pancho.svg"
  };
};

// let getAlternateImage: t => array(string) = // TODO: I want to turn this into an array in the future, show a carousel of images instead.
let getAlternateImage: TokenId.t => option(string) =
  // _animal => Some("/img/animals/Ucokreal.jpg");
  _animal => None;

let getOrgBadgeImage: TokenId.t => string =
  _animal => "/img/badges/DarwinAnimalDoctors.svg";

type launchStatus =
  | Launched
  | LaunchDate(MomentRe.Moment.t);

let nextLaunchDate = MomentRe.momentUtcDefaultFormat("2020-05-21T17:00:00");

let isLaunched: TokenId.t => launchStatus = _anAnimal => Launched;
