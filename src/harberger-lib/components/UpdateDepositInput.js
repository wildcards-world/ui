import React from "react";
import { Heading, Box, Input, Button, Text, Radio } from "rimble-ui";

export default ({
  depositChange,
  updateDepositChange,
  isAddDeposit,
  updateIsAddDeposit,
  onSubmitDepositChange
}) => (
  <Box p={4} mb={3}>
    <Heading>Update Deposit</Heading>
    <Text>How do you want to change your deposit?</Text>
    <Radio
      checked={isAddDeposit}
      onChange={() => updateIsAddDeposit(true)}
      label="Deposit"
      my={2}
    />
    <Radio
      checked={!isAddDeposit}
      onChange={() => updateIsAddDeposit(false)}
      label="Withdrawl"
      my={2}
    />
    <Input
      type="number"
      name="value"
      value={depositChange}
      placeholder={isAddDeposit ? "deposit to add" : "deposit to remove"}
      onChange={updateDepositChange}
    />
    <br />
    <Button onClick={onSubmitDepositChange}>
      {isAddDeposit ? "Add" : "Withdraw"}
    </Button>
  </Box>
);
