import React, { useState, useEffect, useRef } from "react";
import { useCountUp } from "react-countup";

const Counter = ({ from, till }) => {
  const ref = useRef(null);
  const [styles, setStyles] = useState({})
  const { countUp, start, pauseResume, reset, update } = useCountUp({
    start: till,
    end: till,
    delay: 1000,
    duration: 6,
    redraw: false,
    preserveValue: true, // This line is extremely important, otherwise it always starts from zero!!
    // onReset: () => console.log("Resetted!"),
    // onUpdate: () => console.log("Updated!"),
    // onPauseResume: () => console.log("Paused or resumed!"),
    // onStart: ({ pauseResume }) => console.log("pauseResume"),
    // onEnd: ({ pauseResume }) => console.log("pauseResume")
  });

  useEffect(() => {
    console.log({ from, till })
    let timeout
    if (ref.current !== till) {
      ref.current = till;
      if (from === 0) {
        start()
      } else {
        update(till)
        setStyles({ color: "green", fontWeight: "bold" })
        timeout = setTimeout(() => setStyles({}), 900)
      }
    }
    return () => clearTimeout(timeout)
  }, [till])

  return (
    <span style={styles}>
      {countUp}
    </span >
  );
};

export default ({ totalRaised }) => {
  const res = totalRaised.split(".");

  const refPrev = useRef(0);
  const ref = useRef(0);
  // const key = useRef(0);

  useEffect(() => {
    if (ref.current !== parseInt(res[1]))
      // key.current = key.current + 1

      // refPrev.current = ref.current;
      ref.current = parseInt(res[1]);
  }, [totalRaised])


  return (
    <React.Fragment>
      {res[0]}.<Counter from={ref.current} till={parseInt(res[1])} />
    </React.Fragment>
  );
};
