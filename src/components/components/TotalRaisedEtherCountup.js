import React from "react";
import { useCountUp } from "react-countup";

export default ({ totalRaised }) => {
  var res = totalRaised.split(".");

  const { countUp, start, pauseResume, reset, update } = useCountUp({
    start: 783705291160578410,
    end: 783708145041856818,
    delay: 1000,
    duration: 2,
    onReset: () => console.log("Resetted!"),
    onUpdate: () => console.log("Updated!"),
    onPauseResume: () => console.log("Paused or resumed!"),
    onStart: ({ pauseResume }) => console.log(pauseResume),
    onEnd: ({ pauseResume }) => console.log(pauseResume)
  });

  return (
    <div>
      <div>
        {res[0]}.{countUp}
      </div>
      <button onClick={start}>Start</button>
      <button onClick={reset}>Reset</button>
      <button onClick={pauseResume}>Pause/Resume</button>
      <button onClick={() => update(2000)}>Update to 2000</button>
    </div>
  );
};
