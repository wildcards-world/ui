@react.component
let make = () => {
  <div width="100%">
    <div className=Partners.blueBackground>
      <Rimble.Flex flexWrap="wrap" justifyContent="space-around" alignItems="stretch" pt=50 px=50>
        <h1> {"Our artists"->React.string} </h1>
      </Rimble.Flex>
    </div>
  </div>
}
