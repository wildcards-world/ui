open Jest;

describe("Expect", () =>
  Expect.(
    test("toBe", () =>
      expect((
        Animal.getAnimalFromId("0"),
        Animal.getAnimalFromId("1"),
        Animal.getAnimalFromId("41"),
      ))
      |> toEqual((
           Some(Animal.Simon),
           Some(Animal.Andy),
           Some(Animal.Vitalik),
         ))
    )
  )
);
