// module Styles = {
/* Open the Css module, so we can access the style properties below without prefixing them with Css. */
open Css;

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
    zIndex(50),
    display(`flex),
    // flexDirection(`row),
    justifyContent(`spaceBetween),
    // alignItems(center),
    flexWrap(`wrap),
    paddingTop(px(120)),
    paddingLeft(px(15)),
    paddingRight(px(15)),
    paddingBottom(px(120)),
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
    position(relative),
    // width(px(550)),
    maxWidth(px(550)),
    float(`none),
    paddingTop(px(65)),
    // paddingBottom(px(70)),
    width(`calc((`sub, `percent(100.), px(600)))),
    textAlign(center),
    // boxSizing(`borderBox),
    // margin(px(0)),
    // padding(px(0)),
  ]);

let heading = style([fontSize(em(3.))]);

let subHeading = style([fontSize(em(1.8)), fontWeight(`num(200))]);

let wildCardGreen = rgb(107, 173, 62);
let colorGreen = style([color(wildCardGreen)]);

let gorillaColumn = style([position(relative), width(`percent(50.))]);

let gorillas =
  style([
    position(relative),
    width(`percent(30.)),
    float(`right),
    margin(auto),
    display(block),
  ]);

let gorillaBack =
  style([
    position(absolute),
    zIndex(3),
    width(vw(20.)),
    filter([`grayscale(90.)]),
    hover([
      filter([`grayscale(0.)]),
      transition(~duration=1000, ~delay=0, ~timingFunction=ease, "all"),
    ]),
  ]);

let gorillaTransform = (horizontal, vertical) =>
  style([transform(translate(vw(horizontal), vh(vertical)))]);

let headerImg =
  style([
    position(relative),
    zIndex(10),
    maxHeight(px(500)),
    maxWidth(`percent(100.)),
    textAlign(center),
  ]);

let gorillaCentreFront =
  style([position(absolute), zIndex(5), width(vw(20.))]);
