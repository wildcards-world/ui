let isPositiveStringInteger = str => {
  // TODO: this should possibly use something like - https://stackoverflow.com/a/48569311/3103033 ?

  // NOTE: This allows leading 0s (which seams to not be a problem for web3 or truffle)
  //       This doesn't check if the number is smaller than 2^256 which is the largest integer ethereum can handle
  let f = [%bs.re "/^([0-9]\d+)$/"];
  //   let test = [%bs.raw
  //     {|
  //  function (value) {
  //    console.log(`The type of ${value} is:`, typeof value)
  //  }
  // |}
  //   ];
  //   let _ = test(. str);
  Js.Re.test_(f, str);
};

let elipsify: (string, int) => string =
  (inputString, maxLength) =>
    if (inputString->String.length > maxLength) {
      String.sub({j|$inputString|j}, 0, maxLength - 3) ++ "...";
    } else {
      inputString;
    };
