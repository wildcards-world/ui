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

type tokenStatus =
  | Loading
  | WaitingForLaunch(MomentRe.Moment.t)
  | Launched(MomentRe.Moment.t)
  | Owned(Eth.t) // TODO put the owner and price as a parameter here
  | Foreclosed(MomentRe.Moment.t);

let useTokenStatus: TokenId.t => tokenStatus =
  animal => {
    let optLaunchTime = QlHooks.useLaunchTimeBN(animal);
    let currentTime = QlHooks.useCurrentTimestampBn();
    let currentPriceWei = QlHooks.usePrice(animal);

    switch (optLaunchTime) {
    | Some(launchTime) =>
      if (launchTime->BN.gtGet(. currentTime)) {
        WaitingForLaunch(launchTime->BN.toNumber->MomentRe.momentWithUnix);
      } else {
        switch (currentPriceWei) {
        | Price(price) =>
          if (price->BN.gtGet(. BN.new_("0"))) {
            Owned(price);
          } else {
            Launched(launchTime->BN.toNumber->MomentRe.momentWithUnix);
          }
        | Foreclosed(foreclosureTime) =>
          Foreclosed(foreclosureTime->Helper.bnToMoment)
        | Loading => Loading
        };
      }
    | None => Loading
    };
  };

let useIsOnAuction: TokenId.t => bool =
  animal => {
    let tokenStatus = useTokenStatus(animal);

    switch (tokenStatus) {
    | Loading
    | WaitingForLaunch(_)
    | Owned(_) => false
    | Launched(_)
    | Foreclosed(_) => true
    };
  };

let useAuctionPriceWei = (animal, launchTime) => {
  let tokenStatus = useTokenStatus(animal);
  let auctionStartPrice = QlHooks.useAuctionStartPrice(animal);
  let auctionEndPrice = QlHooks.useAuctionEndPrice(animal);
  let auctionLength = QlHooks.useAuctioLength(animal);
  let currentTime = QlHooks.useCurrentTime();

  // disable warning #4
  [@ocaml.warning "-4"]
  (
    switch (tokenStatus) {
    | Foreclosed(foreclosureTime) =>
      let auctionStartTime =
        foreclosureTime->MomentRe.Moment.toUnix->BN.newInt_;

      if (BN.new_(currentTime) |<| (auctionStartTime |+| auctionLength)) {
        auctionStartPrice
        |-| (
          auctionStartPrice
          |-| auctionEndPrice
          |*| (BN.new_(currentTime) |-| auctionStartTime)
          |/| auctionLength
        );
      } else {
        auctionEndPrice;
      };
    | Launched(_) =>
      if (BN.new_(currentTime) |<| (launchTime |+| auctionLength)) {
        auctionStartPrice
        |-| (
          auctionStartPrice
          |-| auctionEndPrice
          |*| (BN.new_(currentTime) |-| launchTime)
          |/| auctionLength
        );
      } else {
        auctionEndPrice;
      }
    | _ => auctionEndPrice
    }
  );
};
