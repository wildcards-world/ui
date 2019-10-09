import React from "react"
import { Heading, Box, Input, Button, Text } from "rimble-ui"

export default ({ deposit, newPrice, setNewPrice, setDeposit, onSubmitBuy }) =>
  <Box p={4} mb={3} >
    <Heading> "Purchase" </Heading>
    <Text>
      Enter the desired values for the transaction.
      </Text>
    <Input
      type="number"
      placeholder="Your Initial Sale Price"
      onChange={setNewPrice}
      value={newPrice}
    />
    <Input
      type="number"
      placeholder="Your Initial Deposit"
      onChange={setDeposit}
      value={deposit}
    />
    <br />
    <Button onClick={onSubmitBuy}>
      Buy
    </Button>
  </Box >; 
