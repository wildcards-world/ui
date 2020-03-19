import React, { useState, useEffect, Fragment } from "react";
import {
  Heading,
  Box,
  Input,
  Button,
  Text,
  Slider,
  Flex,
  Tooltip,
  Icon
} from "rimble-ui";
import moment from "moment";
import { useDebouncedCallback } from "use-debounce";
import {
  getCountDisplayToHours,
  calculateTimeRemainingFromSeconds
} from "./components/CountDown";
const inputStyle = {
  width: "100%"
};
const rightAlignedText = {
  textAlign: "right"
};
const infoTooltipStyle = {
  display: "inline-block"
};
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
  animalName,
  depositForAYear
}) => {
  const [debouncedSetDeposit] = useDebouncedCallback(
    // debounced function
    setDeposit,
    // delay in ms
    50
  );

  const [depositSlider, setDepositSlider] = useState(newPrice);

  useEffect(() => {
    setDepositSlider(deposit);
  }, [deposit]);

  const eventToValue = func => event => func(event.target.value);

  return (
    <Fragment>
      <Flex>
        <Box p={2} mb={2}>
          <Heading>Purchase {animalName}</Heading>
        </Box>
      </Flex>
      <Flex>
        <Box p={2} mb={2} width={[1, 0.5]}>
          <Tooltip
            message={`This is the amount of money you will receive if someone purchases ${animalName} from you.`}
            placement="top"
          >
            <Text>
              Set {animalName}'s new for sale price:
              <div style={infoTooltipStyle}>
                <Icon color="green" name="Info" size="16" />
              </div>
            </Text>
          </Tooltip>
          <Input
            style={inputStyle}
            type="number"
            placeholder="Your Initial Sale Price"
            onChange={eventToValue(setNewPrice)}
            value={newPrice}
          />
        </Box>
        <Box p={2} mb={2} width={[1, 0.5]}>
          <Tooltip
            message="This is the monthly contribution that will go towards conservation of endangered animals. This will be deducted continuously from your deposit"
            placement="top"
          >
            <Text style={rightAlignedText}>
              Your monthly contribution:
              <div style={infoTooltipStyle}>
                <Icon color="green" name="Info" size="16" />
              </div>
            </Text>
          </Tooltip>
          <br />
          <Text style={rightAlignedText}>{patronage}</Text>
        </Box>
      </Flex>
      <Flex
        alignItems="center"
        justifyContent="center"
        alignContent="center"
        verticalAlign
      >
        <Box p={2} mb={2} width={[1, 0.3]}>
          <Tooltip
            message="The deposit is the funds that will be used to cover your monthly contribution."
            placement="top"
          >
            <Text>
              Set your deposit:
              <div style={infoTooltipStyle}>
                <Icon color="green" name="Info" size="16" />
              </div>
            </Text>
          </Tooltip>
          <Input
            type="number"
            style={inputStyle}
            placeholder="Your Initial Deposit"
            onChange={eventToValue(setDeposit)}
            value={deposit}
          />
        </Box>{" "}
        <Box p={2} mb={2} width={[1, 0.7]}>
          <Slider
            value={depositSlider}
            style={inputStyle}
            onChange={event => {
              let value = event.target.value;
              setDepositSlider(value);
              debouncedSetDeposit(value);
            }}
            min={"0.0001"}
            max={maxAvailableDeposit}
            step={"0.0000001"}
          />
        </Box>
      </Flex>

      {/* <p>{depositTimeInSeconds} , {moment.duration(depositTimeInSeconds, 'seconds').humanize(true)}</p> */}
      <p>
        This deposit will last{" "}
        {getCountDisplayToHours(
          calculateTimeRemainingFromSeconds(depositTimeInSeconds)
        )}{" "}
        for your monthly contribution
      </p>
      <Flex>
        <Box p={2} mb={2} width={[1, 0.7]}>
          <Button onClick={onSubmitBuy}>Buy</Button>
        </Box>
      </Flex>
    </Fragment>
  );
};
