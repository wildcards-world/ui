import React, { useState, useEffect } from "react";

const nullTime = {
  years: 0,
  days: 0,
  hours: 0,
  minutes: 0,
  seconds: 0,
  // millisec: 0,
}

const Countdown = ({ countDown }) => {
  const addLeadingZeros = (value) => {
    let valueS = String(value);
    while (valueS.length < 2) {
      valueS = '0' + valueS;
    }
    return valueS;
  }
  const displayTime = (unit) => countDown[unit] > 0 && `${addLeadingZeros(countDown[unit])} ${unit} `

  return (
    <span>
      {displayTime("years")}{displayTime("days")}{displayTime("hours")}{displayTime("minutes")}{displayTime("seconds")}
    </span >
  );
}

const calculateCountdown = (endDateMoment) => {
  const endDate = endDateMoment.toDate()
  // let diff = (new Date(endDate)).
  let diff = (Date.parse((new Date(endDate)).toUTCString()) - Date.parse((new Date()).toUTCString())) / 1000;

  // clear countdown when date is reached
  if (diff <= 0) return nullTime;

  const timeLeft = { ...nullTime };

  // calculate time difference between now and expected date
  if (diff >= (365.25 * 86400)) { // 365.25 * 24 * 60 * 60
    timeLeft.years = Math.floor(diff / (365.25 * 86400));
    diff -= timeLeft.years * 365.25 * 86400;
  }
  if (diff >= 86400) { // 24 * 60 * 60
    timeLeft.days = Math.floor(diff / 86400);
    diff -= timeLeft.days * 86400;
  }
  if (diff >= 3600) { // 60 * 60
    timeLeft.hours = Math.floor(diff / 3600);
    diff -= timeLeft.hours * 3600;
  }
  if (diff >= 60) {
    timeLeft.minutes = Math.floor(diff / 60);
    diff -= timeLeft.minutes * 60;
  }
  timeLeft.seconds = diff;

  return timeLeft;
}

// Functional component for sake of reactHooks...
export default ({ endDateMoment }) => {

  const [countDown, setCountdown] = useState(nullTime);
  // const [, forceUpdate] = React.useState(0);

  useEffect(() => {
    const date = calculateCountdown(endDateMoment);
    setCountdown(date);
    const interval = setInterval(() => {

      const date = calculateCountdown(endDateMoment);
      setCountdown(date);
      // forceUpdate(n => !n)

    }, 1000);
    return () => clearTimeout(interval);
  }, [endDateMoment]);

  return <Countdown countDown={countDown} />
}
