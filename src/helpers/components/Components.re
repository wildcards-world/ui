module S = {
  [@react.component]
  let make = (~children: string) => {
    React.string(children);
  };
};
