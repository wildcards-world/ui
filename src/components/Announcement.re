open Globals;
open Css;

[@react.component]
let make = () => {
  let announcement: 'displayType => string =
    displayVal =>
      style([
        display(displayVal),
        position(relative),
        padding2(~v=rem(0.4), ~h=rem(1.)),
        color(white),
        backgroundColor(hex("DBC0FF")),
        textAlign(`center),
        zIndex(2),
        fontSize(px(18)),
        textTransform(uppercase),
        letterSpacing(px(2)),
      ]);

  let closeButton = style([position(absolute), right(px(10))]);

  let (showAnnouncement, setShowAnnouncement) = React.useState(() => `block);
  <div className={announcement(showAnnouncement)}>
    "New Wildcard "->restr
    <a href="/#details/Glen"> "Glen The Dragon"->restr </a>
    " Coming in "->restr
    <CountDown endDateMoment=123456. displayUnits=false />
    " Days!"->restr
    <span className=closeButton onClick={_ => setShowAnnouncement(_ => `none)}>
      "X"->restr
    </span>
  </div>;
};