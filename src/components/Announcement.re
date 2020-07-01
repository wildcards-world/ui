open Globals;
open Css;

[@react.component]
let make =
    (
      ~nextReleasedAnimals: array(TokenId.t),
      ~announcementBannerColor: string,
    ) => {
  let announcement: 'displayType => string =
    displayVal =>
      style([
        display(displayVal),
        position(relative),
        padding2(~v=rem(0.4), ~h=rem(1.)),
        color(white),
        backgroundColor(hex(announcementBannerColor)),
        textAlign(`center),
        zIndex(2),
        fontSize(px(18)),
        textTransform(uppercase),
        letterSpacing(px(2)),
      ]);

  let linkToAnimal =
    style([
      color(white),
      textDecoration(underline),
      selector(":visited", [color(hex("ffffff"))]),
      selector(":hover", [important(color(hex("6CAD3D")))]),
    ]);

  let nextLaunchDate = Animal.nextLaunchDate;

  let isLaunched =
    MomentRe.diff(nextLaunchDate, MomentRe.momentNow(), `seconds) < 0.;

  let closeButton = style([position(absolute), right(px(10))]);

  let (showAnnouncement, setShowAnnouncement) = React.useState(() => `block);
  let numberOfAnimalsToLaunch = nextReleasedAnimals->Array.length;
  let isPlural = numberOfAnimalsToLaunch > 1;
  <div className={announcement(showAnnouncement)}>
    {(
       "New Wildcard"
       ++ {
         isPlural ? "s" : "";
       }
       ++ " "
     )
     ->restr}
    {React.array(
       nextReleasedAnimals->Array.mapWithIndex((index, animal) => {
         let name = QlHooks.useWildcardName(animal) |||| "Loading";
         <span key={animal->TokenId.toString}>
           <a
             href={"/#details/" ++ animal->TokenId.toString}
             className=linkToAnimal>
             name->restr
           </a>
           {index == numberOfAnimalsToLaunch - 1
              ? " " : index == numberOfAnimalsToLaunch - 2 ? " and " : ", "}
           ->restr
         </span>;
       }),
     )}
    {isLaunched
       ? (
           {
             isPlural ? "have" : "has";
           }
           ++ " just been launched!"
         )
         ->restr
       : <>
           "coming in "->restr
           <CountDown endDateMoment=nextLaunchDate displayUnits=true />
           "!"->restr
         </>}
    <span className=closeButton onClick={_ => setShowAnnouncement(_ => `none)}>
      {js|×|js}->restr
    </span>
  </div>;
};
