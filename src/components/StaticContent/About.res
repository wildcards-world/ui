@react.component @dead("+make")
let make = () =>
  <Rimble.Box className=Styles.horizantalBlueTile>
    <p className=Styles.explainerMediumText>
      <span className=Styles.boldExplainerText> {"Wildcards"->React.string} </span>
      {" is an idea born out of the "->React.string}
      <a href="https://ethcapetown.com/"> {"#ETHCapeTown2019"->React.string} </a>
      {" hackathon which saw team Wildcards as overall "->React.string}
      <a href="https://devpost.com/software/ethcapetown_wildcards"> {"winners"->React.string} </a>
      {". The focus of the project is raising funds for at risk animals."->React.string}
    </p>
  </Rimble.Box>
