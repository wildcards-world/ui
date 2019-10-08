include About;

include HowItWorks;

include CustomerBenefit;

include CoreConcepts;

include EmailSignup;

include FinalNote;

include Footer;

// module Partners {

// [@bs.module "rimble-ui"] [@react.component];
// external make: () => React.element = "default"
// }

module Partners = {
  [@bs.module "./Partners.js"] [@react.component]
  external make: unit => React.element = "default";
};
