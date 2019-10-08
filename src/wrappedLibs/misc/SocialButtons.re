[@bs.module "react-social-icons"] [@react.component]
external make:
  (
    ~url: string=?,
    ~bgColor: string=?,
    ~fgColor: string=?,
    ~target: string=?,
    ~className: string=?
  ) =>
  React.element =
  "SocialIcon";
