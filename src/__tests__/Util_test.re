open Jest;

describe("Expect", () =>
  Expect.(
    testAll(
      "isStringInteger",
      [
        ("003", true),
        ("302", true),
        ("3546674266447602", true),
        ("1250000000000000000", true),
        ("302.2", false),
        ("-5", false),
        ("", false),
      ],
      ((num, result)) =>
      expect(Helper.isStringInteger(num)) |> toBe(result)
    )
  )
);
