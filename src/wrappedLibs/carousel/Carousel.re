[@bs.module "@brainhubeu/react-carousel"] [@react.component]
external make:
  (
    ~children: React.element,
    ~slidesPerPage: int=?,
    ~centered: bool=?,
    ~value: int=?,
    ~animationSpeed: int=?,
    ~className: string=?,
    ~onChange: int => unit=?,
    ~infinite: bool=?,
    ~autoPlay: int=?
  ) =>
  React.element =
  "default";

module Dots = {
  [@bs.module "@brainhubeu/react-carousel"] [@react.component]
  external make:
    (~onChange: int => unit, ~value: int, ~number: int) => React.element =
    "Dots";
};
