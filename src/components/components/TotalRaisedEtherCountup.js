import React, { useState, useEffect } from "react";
import { useCountUp } from "react-countup";

export default ({ totalRaised }) => {
  const [res, setRes] = useState(totalRaised.split("."));

  useEffect(() => {
    setRes(totalRaised.split("."));
    console.log(totalRaised);
  }, [totalRaised]);

  const { countUp, start, pauseResume, reset, update } = useCountUp({
    start: 783705291160578410,
    end: res[1],
    delay: 1000,
    duration: 2,
    onReset: () => console.log("Resetted!"),
    onUpdate: () => console.log("Updated!"),
    onPauseResume: () => console.log("Paused or resumed!"),
    onStart: ({ pauseResume }) => console.log("pauseResume"),
    onEnd: ({ pauseResume }) => console.log("pauseResume")
  });

  const counterup = useCountUp({ end: res[1] });

  return (
    <div>
      <div>
        {res[0]}.{countUp}
        {res[0]}.{counterup}
      </div>
      <button onClick={start}>Start</button>
      <button onClick={reset}>Reset</button>
      <button onClick={pauseResume}>Pause/Resume</button>
      <button onClick={() => update(2000)}>Update to 2000</button>
    </div>
  );
};
