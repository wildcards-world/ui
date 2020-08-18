open Globals;
open Css;

[@react.component]
let make = (~announcementBannerColor: string, ~children) => {
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
        fontSize(px(14)),
        // textTransform(uppercase),
        letterSpacing(px(2)),
      ]);

  // let link =
  //   style([
  //     color(white),
  //     textDecoration(underline),
  //     selector(":visited", [color(hex("ffffff"))]),
  //     selector(":hover", [important(color(hex("6CAD3D")))]),
  //   ]);

  let closeButton = style([position(absolute), right(px(10))]);

  let (showAnnouncement, setShowAnnouncement) = React.useState(() => `block);

  <div className={announcement(showAnnouncement)}>
    children
    <span className=closeButton onClick={_ => setShowAnnouncement(_ => `none)}>
      {js|×|js}->restr
    </span>
  </div>;
};
