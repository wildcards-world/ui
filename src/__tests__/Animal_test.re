open Jest;

describe("Expect", () =>
  Expect.(
    test("toBe", () =>
      expect((
        Animal.getAnimalFromId("0"),
        Animal.getAnimalFromId("1"),
        Animal.getAnimalFromId("42"),
      ))
      |> toEqual((
           Some(Animal.Simon),
           Some(Animal.Andy),
           Some(Animal.Vitalik),
         ))
    )
  )
);
