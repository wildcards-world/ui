[@bs.module "profile-hover"] [@react.component]
external make: (~address: string=?) => React.element = "default";
module Test = {
  [@bs.module "@wildcards/profile-hover"] [@react.component]
  external make: (~address: string=?) => React.element = "default";
};
