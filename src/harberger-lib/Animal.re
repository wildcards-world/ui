open Globals;

let getAnimal: string => option(TokenId.t) =
  // TODO: add a lookup to the `deprecated_id` field (or just let old links be broken...)
  animal => TokenId.make(animal);

let cdnBase = "https://dd2wadt5nc0o7.cloudfront.net";

let useAvatar = animal => {
  QlHooks.useWildcardAvatar(animal)->Option.map(a => cdnBase ++ a)
  |||| "./img/animals/comingsoon.png"; // TODO: use the loading gif as default (but with better resolution! As svg?)
                                     // |||| "./img/loading.gif";
};

// let getAlternateImage: t => array(string) = // TODO: I want to turn this into an array in the future, show a carousel of images instead.
let useAlternateImage: TokenId.t => option(string) =
  animal => {
    QlHooks.useRealImages(animal)
    ->Option.map(animalImages => animalImages->Array.map(info => info##image))
    ->Option.flatMap(images => images[0]);
  };

let useGetOrgImage: string => string =
  org =>
    QlHooks.useLoadOrganisationLogo(org)->Option.map(path => cdnBase ++ path)
    |||| "https://dd2wadt5nc0o7.cloudfront.net/conservations/OGBage.png";

let useGetOrgBadge: string => string =
  org =>
    QlHooks.useLoadOrganisationLogo(org)->Option.map(path => cdnBase ++ path)
    |||| "https://dd2wadt5nc0o7.cloudfront.net/conservations/OGBage.png";

let useGetOrgBadgeImage: TokenId.t => string =
  animal => {
    let org = QlHooks.useWildcardOrgId(animal) |||| "";
    useGetOrgBadge(org);
  };

type launchStatus =
  | Launched
  | LaunchDate(MomentRe.Moment.t);

let nextLaunchDate = MomentRe.momentUtcDefaultFormat("2020-07-30T17:00:00");

let isLaunched: TokenId.t => launchStatus =
  animal =>
    switch (animal->TokenId.toString) {
    // | "25" => LaunchDate(nextLaunchDate)
    | _ => Launched
    };

let useIsOnAuction: TokenId.t => bool =
  animal =>
    switch (animal->TokenId.toString) {
    // | "5" => true
    | _ => false
    };
