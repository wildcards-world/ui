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

let textOnlyModalText = style([padding(em(2.))]);
let totalRaisedText = size => style([fontSize(em(size))]);

let copyButton = style([float(`right), zIndex(50)]);

let centerText = style([textAlign(center)]);

// Use flex box to center
let centerItems =
  style([display(`flex), alignItems(center), width(`percent(100.))]);
// Use margin to center
let centerItemsMargin = style([display(block), margin(auto)]);

let translationSwitch = style([padding(rem(0.))]);

let topBody =
  style([
    // zIndex(50),
    // display(`flex),
    // flexDirection(`row),
    justifyContent(`spaceBetween),
    // alignItems(center),
    media("(max-width: 831px)", [textAlign(center)]),
    // flexWrap(`wrap),
    paddingTop(px(120)),
    paddingLeft(px(15)),
    paddingRight(px(15)),
    // paddingBottom(px(120)),
    maxWidth(px(1200)),
    margin(auto),
  ]);

let header = style([position(relative)]);
let noMarginBottom = style([marginBottom(px(0))]);
let noMarginTop = style([marginTop(px(0))]);

let headerLogo = style([media("(max-width: 630px)", [margin(`auto)])]);

let nav =
  style([
    position(absolute),
    maxWidth(px(1200)),
    zIndex(200),
    top(px(0)),
    right(px(0)),
    left(px(0)),
    margin2(~v=em(0.), ~h=auto),
  ]);

let navBox =
  style([
    display(`flex),
    justifyContent(`spaceBetween),
    alignItems(center),
  ]);

let navList =
  style([
    media("(max-width: 630px)", [display(none)]),
    marginLeft(`auto),
    listStyle(`none, `inside, `none),
  ]);

// let center =
//   style([
//     justifyContent(`spaceBetween),
//     media("(max-width: 831px)", [textAlign(center)]),
//     // flexWrap(`wrap),
//     paddingTop(px(120)),
//     paddingLeft(px(15)),
//     paddingRight(px(15)),
//     // paddingBottom(px(120)),
//     maxWidth(px(1200)),
//     margin2(~v=em(0.), ~h=auto),
//   ]);
let navListItem = style([display(inlineBlock)]);

let navListItemToggle =
  style([display(`flex), justifyContent(center), alignItems(center)]);

let someMarginRight = style([marginRight(px(10))]);

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

let rightTopHeader = style([textAlign(center)]);

/// NOTE: the padding of 4% is calculated as 12/3 since there is a right padding of 12% and 3 gorilla images shown.
let dappImagesCounteractOffset = style([marginLeft(`percent(4.))]);

let heading = style([fontSize(em(3.))]);

let subHeading = style([fontSize(em(1.8)), fontWeight(`num(200))]);

let wildCardGreen = rgb(107, 173, 62);
let wildCardBlue = rgb(114, 199, 215);
let colorGreen = style([color(wildCardGreen)]);
let colorBlue = style([color(wildCardBlue)]);

let gorillaBox = style([marginRight(`percent(12.))]);

let ownedGorillaImg =
  style([
    position(relative),
    zIndex(1),
    maxWidth(`percent(100.)),
    textAlign(center),
    // transform(translateX(`percent(-25.))),
  ]);

let clickableLink = style([cursor(`pointer)]);

let headerImg = enlargement =>
  style([
    zIndex(1),
    maxHeight(px(500)),
    maxWidth(`percent(enlargement)),
    textAlign(center),
    hover([
      filter([`saturate(150.), `brightness(110.)]),
      zIndex(2),
      overflow(visible),
      transform(scale(1.1, 1.1)),
      transition(~duration=100, ~delay=0, ~timingFunction=ease, "all"),
    ]),
  ]);

let gorillaText = style([transform(translateX(`percent(25.)))]);

let horizantalBlueTile =
  style([
    width(`percent(100.)),
    padding2(~v=em(2.), ~h=em(0.)),
    backgroundColor(`hex("73c7d7ff")),
    // media("(max-width: 831px)", [padding2(~v=rem(2.), ~h=rem(2.4))]),
    textAlign(`center),
  ]);

let explainerLargeText =
  style([
    fontSize(rem(2.)),
    color(`hex("486269")),
    padding2(~v=em(0.), ~h=em(6.)),
    margin2(~v=em(1.), ~h=auto),
    maxWidth(px(1200)),
    media(
      "(max-width: 760px)",
      [fontSize(rem(1.2)), padding2(~v=em(0.), ~h=em(2.))],
    ),
  ]);
let explainerMediumText =
  style([
    media(
      "(max-width: 760px)",
      [fontSize(rem(1.1)), padding2(~v=em(0.), ~h=em(2.))],
    ),
    padding2(~v=em(0.), ~h=em(3.)),
    margin2(~v=em(0.), ~h=auto),
    maxWidth(px(1200)),
    fontSize(rem(1.6)),
    color(`hex("486269")),
    textAlign(`center),
  ]);

let boldExplainerText = style([color(`hex("303030")), fontWeight(`bold)]);

let infoBackground =
  style([
    backgrounds([
      `linearGradient((
        deg(0.),
        [
          (zero, `rgba((255, 255, 255, 0.2))),
          (zero, `rgba((255, 255, 255, 0.2))),
        ],
      )),
      `url(backgroundImageGorilla),
    ]),
    backgroundSize(`cover),
    backgroundRepeat(`noRepeat),
    height(`percent(100.)),
  ]);

let infoCardContainer = style([padding(`rem(1.))]);

let infoCardStyles =
  style([
    margin(`rem(4.)),
    media("(max-width: 831px)", [margin(`rem(0.)), textAlign(center)]),
  ]);

let gorillaImage = style([media("(max-width: 831px)", [display(none)])]);

let redDisclaimer = style([fontSize(`rem(0.9)), color(`hex("e85723"))]);

let floatingSignupBox =
  style([
    maxWidth(px(900)),
    margin2(~v=em(0.), ~h=auto),
    backgroundColor(`hex("fff")),
    // boxShadow10(px(0), px(2), px(4), px(0), `rgba((136,144,195,0.2)), px(0), px(5), px(15), px(0), `rgba((37,44,97,0.15)))
  ]);
let floatingSignupBoxInner =
  style([
    margin2(~v=em(3.), ~h=auto),
    // padding2(~v=em(3.), ~h=em(3.)),
    padding(em(3.)),
  ]);
let emailSignupHeader =
  style([
    fontSize(px(24)),
    // padding2(~v=em(3.), ~h=em(3.)),
    // padding(em(3.)),
  ]);
let emailTextBox =
  style(
    [] // padding(`auto),
    // padding2(~v=auto, ~h=auto),
    // padding(em(3.)),
  );

let inputElements = style([padding(em(0.1))]);

let extraInfoFooterBox =
  style([
    textAlign(center),
    maxWidth(px(600)),
    backgroundColor(rgb(107, 173, 62)),
    margin2(~v=em(6.), ~h=auto),
    position(relative),
  ]);

let emoticonHeader =
  style([
    position(absolute),
    left(px(0)),
    right(px(0)),
    top(px(0)),
    margin(auto),
    transform(translateY(`percent(-50.))),
    textAlign(center),
  ]);

let emojiStyles =
  style([
    height(px(55)),
    maxHeight(`percent(100.)),
    position(relative),
    verticalAlign(middle),
    bottom(px(1)),
  ]);

let finalNoteContent = style([padding(rem(1.5)), margin(ex(4.))]);

let whiteText = style([color(white)]);

let linkPillBox =
  style([
    marginTop(px(25)),
    boxSizing(borderBox),
    minHeight(px(25)),
    fontSize(px(14)),
    borderRadius(px(25)),
    color(hex("fff")),
    backgroundColor(rgba(48, 48, 48, 0.12)),
  ]);
let linkPillText =
  style([margin(px(10)), marginBottom(px(1)), color(hex("fff"))]);

let footer =
  style([backgroundColor(hex("303030")), padding2(~v=px(40), ~h=px(0))]);

let footerContainer =
  style([
    margin(auto),
    padding2(~v=px(0), ~h=px(15)),
    maxWidth(px(1080)),
  ]);

let footerWrapper =
  style([
    display(`flex),
    justifyContent(spaceBetween),
    alignItems(flexStart),
    alignSelf(center),
    flexWrap(`wrap),
  ]);

let footerText =
  style([marginTop(auto), marginBottom(auto), color(hex("919797"))]);

let footerLink =
  style([textDecoration(none), important(color(hex("919797")))]);

let footerSocialButtons =
  style([
    padding(px(0)),
    marginRight(em(6.)), // This is needed to cater for the 'crisp' chat help.
    listStyle(`none, `inside, `none),
    display(`flex),
    justifyContent(center),
    alignItems(center),
    flexWrap(wrap),
  ]);

let footerSocialButton =
  style([
    padding(px(0)),
    margin(px(0)),
    // listStyle(`none, `inside, `none),
    display(`flex),
    justifyContent(flexEnd),
    alignItems(center),
    flexWrap(wrap),
  ]);

let footerSocailButtonLink =
  style([
    maxWidth(px(32)),
    maxHeight(px(32)),
    // hover([
    //   filter([`grayscale(0.)]),
    //   transition(~duration=2000, ~delay=0, ~timingFunction=ease, "all"),
    // ]),
    // transition(~duration=2000, ~delay=0, ~timingFunction=ease, "opacity"),
    // // -webkit-transition: opacity .2s ease,box-shadow .2s ease,-webkit-transform .2s ease;
    // // transition: opacity .2s ease,box-shadow .2s ease,-webkit-transform .2s ease;
    // // transition: opacity .2s ease,transform .2s ease,box-shadow .2s ease;
    // transition: opacity .2s ease,transform .2s ease,box-shadow .2s ease,-webkit-transform .2s ease;
  ]);

let infoModal = style([padding(rem(2.)), borderRadius(px(5))]);

let betaBanner =
  style([
    position(`absolute),
    left(`px(0)),
    top(`px(0)),
    width(`px(80)),
  ]);

let forwardBackButton = style([width(`percent(100.)), height(`em(5.))]);

// let mapStylesFunction = [%raw (prev, next) => "`${prev} ${next}`"];
let mergeStyles: list(string) => string =
  styles => {
    /* styles->Belt.List.reduce("", mapStylesFunction);*/
    styles->Belt.List.reduce("", (prev, next) => prev ++ " " ++ next);
  };

let wrapText = style([overflowWrap(`breakWord), wordWrap(`breakWord)]);

let fiftyPercentWidth = style([width(`percent(50.))]);
