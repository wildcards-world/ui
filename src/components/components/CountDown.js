import React, { useState, useEffect } from "react";

const nullTime = {
  years: 0,
  months: 0,
  days: 0,
  hours: 0,
  minutes: 0,
  seconds: 0
  // millisec: 0,
};

export const getCountDisplay = (
  countDown,
  includeWords = true,
  leadingZeros = false
) => {
  const addLeadingZeros = value => {
    let valueS = String(value);
    while (valueS.length < 2) {
      valueS = "0" + valueS;
    }
    return valueS;
  };
  const displayTime = (unit, appendage = true) =>
    countDown[unit] > 0
      ? leadingZeros
        ? `${addLeadingZeros(countDown[unit])} ${
            includeWords ? unit : appendage ? ":" : ""
          } `
        : `${countDown[unit]} ${includeWords ? unit : appendage ? ":" : ""} `
      : "";

  return `${displayTime("years")}${displayTime("months")}${displayTime(
    "days"
  )}${displayTime("hours")}${displayTime("minutes")}${displayTime(
    "seconds",
    false
  )}`;
};

export const getCountDisplayToHours = (
  countDown,
  includeWords = true,
  leadingZeros = false
) => {
  const addLeadingZeros = value => {
    let valueS = String(value);
    while (valueS.length < 2) {
      valueS = "0" + valueS;
    }
    return valueS;
  };
  const displayTime = (unit, appendage = true) =>
    countDown[unit] > 0
      ? leadingZeros
        ? `${addLeadingZeros(countDown[unit])} ${
            includeWords ? unit : appendage ? ":" : ""
          } `
        : `${countDown[unit]} ${
            includeWords
              ? countDown[unit] == 1
                ? unit.substring(0, unit.length - 1)
                : unit
              : appendage
              ? ":"
              : ""
          } `
      : "";

  return `${displayTime("years")}${displayTime("months")}${displayTime(
    "days"
  )}${displayTime("hours")}`;
};

export const calculateTimeRemainingFromSeconds = numSeconds => {
  // clear countdown when date is reached
  if (numSeconds <= 0) return { ...nullTime };

  const timeLeft = { ...nullTime };

  // calculate time numSecondserence between now and expected date
  // NOTE: this rounds off the solar year to the nearest day (ie it is off by 5 hours 48 minutes 46 seconds)
  if (numSeconds >= 31536000) {
    // 365 * 24 * 60 * 60
    timeLeft.years = Math.floor(numSeconds / 31536000);
    numSeconds -= timeLeft.years * 31536000;
  }
  if (numSeconds >= 2628000) {
    // (365 * 24 * 60 * 60) / 12
    timeLeft.months = Math.floor(numSeconds / 2628000);
    numSeconds -= timeLeft.months * 2628000;
  }
  if (numSeconds >= 86400) {
    // 24 * 60 * 60
    timeLeft.days = Math.floor(numSeconds / 86400);
    numSeconds -= timeLeft.days * 86400;
  }
  if (numSeconds >= 3600) {
    // 60 * 60
    timeLeft.hours = Math.floor(numSeconds / 3600);
    numSeconds -= timeLeft.hours * 3600;
  }
  if (numSeconds >= 60) {
    timeLeft.minutes = Math.floor(numSeconds / 60);
    numSeconds -= timeLeft.minutes * 60;
  }
  timeLeft.seconds = numSeconds;

  return timeLeft;
};

const calculateCountdown = endDateMoment => {
  const endDate = endDateMoment.toDate();
  let numSeconds =
    (Date.parse(new Date(endDate).toUTCString()) -
      Date.parse(new Date().toUTCString())) /
    1000;

  return calculateTimeRemainingFromSeconds(numSeconds);
};

export default ({ endDateMoment, includeWords, leadingZeros }) => {
  const [countDown, setCountdown] = useState(nullTime);

  useEffect(() => {
    const date = calculateCountdown(endDateMoment);
    setCountdown(date);
    const interval = setInterval(() => {
      const date = calculateCountdown(endDateMoment);
      setCountdown(date);
    }, 1000);
    return () => clearTimeout(interval);
  }, [endDateMoment]);

  return (
    <React.Fragment>
      {getCountDisplay(countDown, includeWords, leadingZeros)}
    </React.Fragment>
  );
};
