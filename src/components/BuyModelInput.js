import React, { useState, useEffect, Fragment } from "react"
import { Heading, Box, Input, Button, Text, Slider, Flex } from "rimble-ui"

import { useDebouncedCallback } from 'use-debounce';

export default ({
  patronage,
  newPrice,
  deposit,
  depositTimeInSeconds,
  priceSliderInitialMax,
  maxAvailableDeposit,
  setNewPrice,
  updatePatronage,
  setDeposit,
  onSubmitBuy,
  gorillaName
}) => {
  const [debouncedSetDeposit] = useDebouncedCallback(
    // debounced function
    setDeposit,
    // delay in ms
    50
  );

  const [depositSlider, setDepositSlider] = useState(newPrice)

  useEffect(() => {
    setDepositSlider(deposit);
  }, [deposit]);

  const eventToValue = (func) => (event) => func(event.target.value)

  return <Fragment><Flex>
    <Box p={2} mb={2} >
      <Heading>Purchase {gorillaName}</Heading>
    </Box>
    </Flex >
    <Flex >
    <Box p={2} mb={2} width={[1, 0.5]}>
      <Text>
        Set your monthly contribution:
      </Text>
      <Input
        type="number"
        placeholder="Your Initial Sale Price"
        onChange={eventToValue(setNewPrice)}
        value={newPrice}
      />
    </Box>
    <Box p={2} mb={2} width={[1, 0.5]}>
      <Text>
        This will be {gorillaName}'s new for sale price:
      </Text>
      <Input
        type="number"
        placeholder="Monthly Patronage"
        onChange={eventToValue(updatePatronage)}
        value={patronage}
      />
    </Box>
    </Flex >
    <Flex >
    <Box p={2} mb={2} width={[1, 0.5]}>
      <Input
        type="number"
        placeholder="Your Initial Deposit"
        onChange={eventToValue(setDeposit)}
        value={deposit}
      />
    </Box>
          </Flex >
          <Flex >
    <Box p={2} mb={2} width={[1, 0.5]}>
      <Slider value={depositSlider}
        onChange={
          event => {
            let value = event.target.value
            setDepositSlider(value)
            debouncedSetDeposit(value)
          }
        }
        min={"0.0000001"} max={maxAvailableDeposit} step={"0.0000001"} />
      <p>This deposit will last {depositTimeInSeconds} seconds</p>
      <p>This deposit will last {depositTimeInSeconds / 2628000} months</p>
      <br />
      <Button onClick={onSubmitBuy}>
        Buy
    </Button>
    </Box>
  </Flex >
  </Fragment>
}; 
