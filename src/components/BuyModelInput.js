import React, { useState, useEffect } from "react"
import { Heading, Box, Input, Button, Text, Slider } from "rimble-ui"

import { useDebouncedCallback } from 'use-debounce';

export default ({
  patronage,
  newPrice,
  deposit,
  foreclosureTime, // in seconds?
  patronageMax,
  priceSliderInitialMax,
  newPriceMax,
  depositMax,
  foreclosureTimeMax, // in seconds?
  setNewPrice,
  updatePatronage,
  setDeposit,
  setForeclosureTime,
  onSubmitBuy,
}) => {
  // Debounce callback
  const [debouncedCallback] = useDebouncedCallback(
    // debounced function
    setNewPrice,
    // delay in ms
    50
  );
  const [priceSlider, setPriceSlider] = useState(newPrice)
  const [priceSliderMax, setPriceSliderMax] = useState(priceSliderInitialMax)
  const [priceSliderEnabled, setPriceSliderEnabled] = useState(true)
  // This will create about 200 steps by default in the slider.
  const [priceSliderStep, setPriceSliderStep] = useState((parseFloat(newPrice) / 100).toString())
  useEffect(() => {
    setPriceSlider(newPrice);
    let newPriceFloat = parseFloat(newPrice)
    let priceSliderMaxFloat = parseFloat(priceSliderMax)
    let sliderRatio = newPriceFloat / priceSliderMaxFloat
    // If the slider is in the top 1% OR the bottom 5% make the slider longer or shorter respectively.
    if (sliderRatio > 0.99 || sliderRatio < 0.05) {
      setPriceSliderMax((newPriceFloat * 2).toString())
      setPriceSliderStep(newPriceFloat / 100)

      // Disable the price slider for half a second after the scaling is adjusted to prevent weird jitters
      setPriceSliderEnabled(false)
      setTimeout(() => setPriceSliderEnabled(true), 500)
    }
  }, [newPrice]);

  const eventToValue = (func) => (event) => func(event.target.value)

  return <Box p={4} mb={3} >
    <Heading>Purchase</Heading>
    <Text>
      Enter the desired values for the transaction.
      </Text>
    <Input
      type="number"
      placeholder="Your Initial Sale Price"
      onChange={eventToValue(setNewPrice)}
      value={newPrice}
    />
    <Slider value={priceSlider}
      onChange={
        event => {
          if (!priceSliderEnabled) return
          let value = event.target.value
          setPriceSlider(value)
          debouncedCallback(value)
        }
      }
      min={"0.00000000001"} max={priceSliderMax} step={priceSliderStep} />
    <Input
      type="number"
      placeholder="Monthly Patronage"
      onChange={updatePatronage}
      value={patronage}
    />
    <Input
      type="number"
      placeholder="Your Initial Deposit"
      onChange={event => {
        let value = event.target.value
        setDeposit(value)
      }}
      value={deposit}
    />
    <br />
    <Button onClick={onSubmitBuy}>
      Buy
    </Button>
  </Box >
}; 
