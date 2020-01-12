open Belt.Option;

// TODO: use 3box and deprecate this functionality.
module Input = {
  [@react.component]
  let make = () => {
    <Rimble.Box p=1>
      <Rimble.Heading>
        {React.string("TODO: 3 Box will be here!")}
      </Rimble.Heading>
    </Rimble.Box>;
  };
};

[@react.component]
let make = () => {
  let goToVerifyUser = RootProvider.useVerifyUser();

  <React.Fragment>
    <Rimble.Box p=1>
      <Rimble.Button onClick={_e => {goToVerifyUser()}}>
        {React.string("Verify")}
      </Rimble.Button>
    </Rimble.Box>
  </React.Fragment>;
};
