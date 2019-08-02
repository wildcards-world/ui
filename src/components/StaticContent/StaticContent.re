include About;

include HowItWorks;

module ProblemSolution = {
  [@bs.module "./ProblemSolution.js"] [@react.component]
  external make: unit => React.element = "default";
};

include CustomerBenefit;

include CoreConcepts;

module EmailSignup = {
  [@bs.module "./EmailSignup.js"] [@react.component]
  external make: unit => React.element = "default";
};

module FinalNote = {
  [@bs.module "./FinalNote.js"] [@react.component]
  external make: unit => React.element = "default";
};

module Footer = {
  [@bs.module "./Footer.js"] [@react.component]
  external make: unit => React.element = "default";
};
