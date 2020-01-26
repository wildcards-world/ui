type previousNextAnimal = option((Animal.t, Animal.t));
type animalPageState =
  | DetailView(previousNextAnimal, option(Animal.t))
  | NormalView;
type urlState =
  | User(Web3.ethAddress)
  | Explorer(animalPageState)
  // | Unknown
  | Home(animalPageState);

let useUrlState = () => {
  let url = ReasonReactRouter.useUrl();

  React.useMemo1(
    () =>
      switch (Js.String.split("/", url.hash)) {
      | [|"user", address|] => User(address)
      // | [|"explorer"|] => Explorer(NormalView)
      | [|"explorer", "details", animalStr|]
      | [|"explorer", "details", animalStr, ""|] =>
        let optionAnimal = Animal.getAnimal(animalStr);
        Explorer(
          DetailView(
            optionAnimal->Belt.Option.map(animal =>
              Animal.getNextPrev(animal)
            ),
            Animal.getAnimalFromId(animalStr),
          ),
        );
      // | [|"details"|] => Home(NormalView)
      | [|"details", animalStr|] =>
        let optionAnimal = Animal.getAnimal(animalStr);
        Home(
          DetailView(
            optionAnimal->Belt.Option.map(animal =>
              Animal.getNextPrev(animal)
            ),
            Animal.getAnimalFromId(animalStr),
          ),
        );
      | urlArray =>
        switch (
          Belt.Array.get(urlArray, 0)->Belt.Option.mapWithDefault("", a => a)
        ) {
        | "explorer" => Explorer(NormalView)
        | _ => Home(NormalView)
        // | _ => Unknown
        }
      },
    [|url.hash|],
  );
};
let useIsExplorer = () => {
  let urlState = useUrlState();

  React.useMemo1(
    () =>
      switch (urlState) {
      | Explorer(_) => true
      | _ => false
      },
    [|urlState|],
  );
};
let isDetailsAnimalPage: animalPageState => bool =
  animalPageState =>
    switch (animalPageState) {
    | DetailView(_) => true
    | _ => false
    };
let useIsDetails = () => {
  let urlState = useUrlState();

  React.useMemo1(
    () =>
      switch (urlState) {
      | Explorer(inside) => isDetailsAnimalPage(inside)
      | Home(inside) => isDetailsAnimalPage(inside)
      | _ => false
      },
    [|urlState|],
  );
};
