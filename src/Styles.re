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
    justifyContent(`spaceBetween),
    alignItems(center),
    flexWrap(`wrap),
    paddingTop(px(120)),
    paddingBottom(px(120)),
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
