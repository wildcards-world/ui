// module Styles = {
/* Open the Css module, so we can access the style properties below without prefixing them with Css. */
open Css;

let backgroundImageGorilla = [%bs.raw
  {|require("../src/img/wildcardsimages/wild-jungle-background.png")|}
];

Css.(global("body", [fontFamily("Lato, Helvetica, sans-serif")]));
Css.(global("a", [textDecoration(none)]));

let app =
  style([
    boxSizing(`borderBox),
    // Make a global default font size
    fontSize(px(16)),
  ]);

let topBody =
  style([
    // zIndex(50),
    // display(`flex),
    // flexDirection(`row),
    justifyContent(`spaceBetween),
    // alignItems(center),
    // flexWrap(`wrap),
    paddingTop(px(120)),
    paddingLeft(px(15)),
    paddingRight(px(15)),
    // paddingBottom(px(120)),
    maxWidth(px(1200)),
    marginRight(auto),
    marginLeft(auto),
  ]);

let header = style([position(relative)]);

let nav =
  style([
    position(absolute),
    maxWidth(px(1200)),
    zIndex(200),
    top(px(0)),
    right(px(0)),
    left(px(0)),
    marginRight(auto),
    marginLeft(auto),
  ]);

let navBox =
  style([
    display(`flex),
    justifyContent(`spaceBetween),
    alignItems(center),
  ]);

let navList = style([marginLeft(`auto), listStyle(`none, `inside, `none)]);

let navListItem = style([display(inlineBlock)]);

let navListText =
  style([color(rgb(136, 136, 136)), padding(rem(1.)), fontWeight(bold)]);

let leftTopHeader =
  style([
    position(relative),
    width(px(550)),
    maxWidth(px(550)),
    paddingTop(px(70)),
    paddingBottom(px(70)),
    // boxSizing(`borderBox),
    // margin(px(0)),
    // padding(px(0)),
  ]);

let rightTopHeader =
  style([
    // position(relative),
    // width(px(550)),
    // maxWidth(px(550)),
    // float(`none),
    // paddingTop(px(65)),
    // paddingBottom(px(70)),
    // width(`calc((`sub, `percent(100.), px(600)))),
    textAlign(center),
    // boxSizing(`borderBox),
    // margin(px(0)),
    // padding(px(0)),
  ]);

let heading = style([fontSize(em(3.))]);

let subHeading = style([fontSize(em(1.8)), fontWeight(`num(200))]);

let wildCardGreen = rgb(107, 173, 62);
let colorGreen = style([color(wildCardGreen)]);

let gorillaBack =
  style([
    filter([`grayscale(90.)]),
    hover([
      filter([`grayscale(0.)]),
      transition(~duration=1000, ~delay=0, ~timingFunction=ease, "all"),
    ]),
  ]);

let gorillaBox =
  style([marginLeft(`percent(12.)), marginLeft(`percent(12.))]);

let headerImg =
  style([
    position(relative),
    zIndex(10),
    maxHeight(px(500)),
    maxWidth(`percent(150.)),
    textAlign(center),
    transform(translateX(`percent(-25.))),
  ]);

let horizantalBlueTile =
  style([
    width(`percent(100.)),
    padding2(~v=em(2.), ~h=em(10.)),
    backgroundColor(`hex("73c7d7ff")),
  ]);

let explainerLargeText =
  style([fontSize(rem(2.)), color(`hex("486269")), textAlign(`center)]);

let boldExplainerLargeText =
  style([color(`hex("303030")), fontWeight(`bold)]);

let infoBackground =
  style([
    backgroundSize(`cover),
    backgroundRepeat(`noRepeat),
    backgroundImage(`url(backgroundImageGorilla)),
  ]);

let infoCardStyles = style([margin(`rem(6.))]);
