import React, { Fragment } from "react"

export default ({
  animalArray
}) => {
  return <Fragment>
    <p>available animals</p>
    {animalArray.map((Animal, key) => <div key={key} style={{
      maxWidth: "150px",
      textAlign: "center"
    }}>
      <  Animal />
    </div>
    )}
  </Fragment >
};

