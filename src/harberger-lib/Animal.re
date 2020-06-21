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
  TokenId.fromStringUnsafe("11"),
  TokenId.fromStringUnsafe("12"),
  TokenId.fromStringUnsafe("13"),
  TokenId.fromStringUnsafe("14"),
  TokenId.fromStringUnsafe("15"),
  TokenId.fromStringUnsafe("16"),
  TokenId.fromStringUnsafe("17"),
  TokenId.fromStringUnsafe("18"),
  TokenId.fromStringUnsafe("42"),
|];

let useGetName: TokenId.t => option(string) =
  tokenId => QlHooks.useWildcardName(tokenId);

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
    | "16" => "Hook"
    | "17" => "Mijungla"
    | "18" => "Ajayu"
    | "42" => "Vitalik"
    | _ => "Unknown"
    };
// http://localhost:3000/#details/15
// http://localhost:3000/#details/simon
// http://localhost:3000/#details/tarkus
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

// TODO: Move to the backend!
let getImage = animal => {
  switch (animal->TokenId.toString) {
  | "0" => "https://dd2wadt5nc0o7.cloudfront.net/0-simon.svg"
  | "1" => "https://dd2wadt5nc0o7.cloudfront.net/1-andy.svg"
  | "2" => "https://dd2wadt5nc0o7.cloudfront.net/2-apthapi.svg"
  | "3" => "https://dd2wadt5nc0o7.cloudfront.net/3-aruma.svg"
  | "4" => "https://dd2wadt5nc0o7.cloudfront.net/4-catstevens.svg"
  | "5" => "https://dd2wadt5nc0o7.cloudfront.net/5-cubai.svg"
  | "6" => "https://dd2wadt5nc0o7.cloudfront.net/6-llajuita.svg"
  | "7" => "https://dd2wadt5nc0o7.cloudfront.net/7-pancho.svg"
  | "8" => "https://dd2wadt5nc0o7.cloudfront.net/8-espumita.svg"
  | "9" => "https://dd2wadt5nc0o7.cloudfront.net/9-verano.svg"
  | "10" => "https://dd2wadt5nc0o7.cloudfront.net/10-nonhlanhla.svg"
  | "11" => "https://dd2wadt5nc0o7.cloudfront.net/11-dlala.svg"
  | "12" => "https://dd2wadt5nc0o7.cloudfront.net/12-isisa.svg"
  | "13" => "https://dd2wadt5nc0o7.cloudfront.net/13-glen.svg"
  | "14" => "https://dd2wadt5nc0o7.cloudfront.net/14-ucok.svg"
  | "15" => "https://dd2wadt5nc0o7.cloudfront.net/15-tarkus.svg"
  | "16" => "https://dd2wadt5nc0o7.cloudfront.net/16-hook.jpg"
  | "17" => "https://dd2wadt5nc0o7.cloudfront.net/17-mijungla.svg"
  | "18" => "https://dd2wadt5nc0o7.cloudfront.net/18-ajayu.svg"
  | "42" => "https://dd2wadt5nc0o7.cloudfront.net/42-vitalik.svg"
  | _ => "./img/animals/comingsoon.png"
  };
};

// let getAlternateImage: t => array(string) = // TODO: I want to turn this into an array in the future, show a carousel of images instead.
let getAlternateImage: TokenId.t => option(string) =
  animal =>
    switch (animal->TokenId.toString) {
    | "2" => Some("https://dd2wadt5nc0o7.cloudfront.net/2-apthapi-real.jpg")
    | "3" => Some("https://dd2wadt5nc0o7.cloudfront.net/3-aruma-real.jpg")
    | "4" =>
      Some("https://dd2wadt5nc0o7.cloudfront.net/4-catstevens-real.jpg")
    | "5" => Some("https://dd2wadt5nc0o7.cloudfront.net/5-cubai-real.jpg")
    | "6" => Some("https://dd2wadt5nc0o7.cloudfront.net/6-llajuita-real.jpg")
    | "7" => Some("https://dd2wadt5nc0o7.cloudfront.net/7-pancho-real.jpg")
    | "8" => Some("https://dd2wadt5nc0o7.cloudfront.net/8-espumita-real.jpg")
    | "9" => Some("https://dd2wadt5nc0o7.cloudfront.net/9-verano-real.jpg")
    | "10" =>
      Some("https://dd2wadt5nc0o7.cloudfront.net/10-nonhlanhla-real.jpg")
    | "11" => Some("https://dd2wadt5nc0o7.cloudfront.net/11-dlala-real.jpg")
    | "12" => Some("https://dd2wadt5nc0o7.cloudfront.net/12-isisa-real.jpg")
    | "14" => Some("https://dd2wadt5nc0o7.cloudfront.net/14-ucok-real.jpg")
    | "15" => Some("https://dd2wadt5nc0o7.cloudfront.net/15-tarkus-real.jpg")
    | "16" => Some("https://dd2wadt5nc0o7.cloudfront.net/16-hook-real.jpg")
    | "17" =>
      Some("https://dd2wadt5nc0o7.cloudfront.net/17-mijungla-real.jpg")
    | "18" => Some("https://dd2wadt5nc0o7.cloudfront.net/18-ajayu-real.jpg")
    | "13"
    | "0"
    | "1"
    | "42"
    | _ => None
    };

// TODO: move this to the server!
let useGetOrgBadgeImage: TokenId.t => string =
  animal => {
    let org = QlHooks.useWildcardOrgId(animal) |||| "";
    switch (org) {
    | "sendaverde" => "/img/badges/SendaVerdeBadge.png"
    | "wildtomorrow" => "/img/badges/WildTomorrowBadge.png"
    | "wildcards" => "/img/logos/wildcards-logo.svg" // TODO: find cropped version of this logo
    | "darwinanimaldoctors" => "/img/badges/DarwinAnimalDoctors.svg"
    | "greatwhaleconservancy" => "/img/badges/great-whale-conservancy-small.png"
    | _ => "/img/badges/OGBage.png"
    };
  };
// TODO: this will come from the backend too. But it will be any alternative badge for the UI
let useGetAltBadge: TokenId.t => option(string) =
  animal => {
    switch (animal->TokenId.toString) {
    | "0"
    | "1"
    | "42" => Some("/img/badges/OGBage.png")
    | _ => None
    };
  };
type launchStatus =
  | Launched
  | LaunchDate(MomentRe.Moment.t);

let nextLaunchDate = MomentRe.momentUtcDefaultFormat("2020-05-21T17:00:00");

let isLaunched: TokenId.t => launchStatus = _anAnimal => Launched;
