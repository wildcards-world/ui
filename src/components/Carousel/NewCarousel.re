%raw
{| import "./Carousel.scss" |};

let create_sides = (numberOfSides, radius) =>
  Array.init(
    numberOfSides - 1,
    index => {
      let css =
        "rotate3d(0, 1, 0,"
        ++ Js.Float.toString(
             360.0 *. float_of_int(index + 1) /. float_of_int(numberOfSides),
           )
        ++ "deg) translate3d(0, 0,"
        ++ Js.Float.toString(radius)
        ++ "vw)";
      <div
        key={string_of_int(index)}
        className={Css.style([
          Css.border(Css.px(1), `dashed, Css.hex("303030")),
          Css.width(`percent(100.)),
        ])}
        style={ReactDOMRe.Style.make(~transform=css, ())}>
        {ReasonReact.string(string_of_int(index + 1))}
        <img
          className={Styles.headerImg(3., 1.)}
          src={Animal.Andy->Animal.getImage}
        />
      </div>;
      // <Dapp.BasicAnimalDisplay animal=Animal.Andy isExplorer=false />
    },
  );

[@react.component]
let make = (~numberOfSides: int, ~radius, ~transform) =>
  <section id="carousel" style={ReactDOMRe.Style.make(~transform, ())}>
    {ReasonReact.array(create_sides(numberOfSides, radius))}
  </section>;
