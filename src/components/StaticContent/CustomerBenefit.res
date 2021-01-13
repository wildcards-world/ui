@react.component
let make = () =>
  <Rimble.Box className=Styles.horizantalBlueTile>
    <p className=Styles.explainerLargeText>
      <span className=Styles.boldExplainerText> {"Wildcards"->React.string} </span>
      {" connects global conservation organisations to users. "->React.string}
    </p>
    <p className=Styles.explainerMediumText>
      {"Social, gamified and transparent giving makes wildcards the perfect platform to start making your difference."->React.string}
    </p>
  </Rimble.Box>
