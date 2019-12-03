// TODO: remove the "NoAnimal" option, it makes the code messy for no reason. Rather use an `option` if there might not be a animal.
type t =
  | Andy
  | Vitalik
  | Simon
  | Verano
  | Tarkus
  | Pancho
  | Mijungla
  | Llajuita
  // | Espumita
  | Cubai
  | CatStevens
  | Aruma
  | Apthapi
  | Ajayu
  | Isisa
  | Dlala
  | Nonhlanhla;

let getId: t => option(string) =
  anAnimal =>
    switch (anAnimal) {
    | Simon => Some("0")
    | Andy => Some("1")
    | Apthapi => Some("2")
    | Aruma => Some("3")
    | CatStevens => Some("4")
    | Cubai => Some("5")
    | Llajuita => Some("6")
    | Pancho => Some("7")
    // | Espumita => Some("8")
    | Verano => Some("9")
    | Nonhlanhla => Some("10")
    | Dlala => Some("11")
    | Isisa => Some("12")
    // | Vitalik => Some("42") // We don't show an ID for vitalik since using legacy contract still.
    | Tarkus
    | Mijungla
    | Ajayu
    | Vitalik => None
    };

let getTokenId: t => option(TokenId.t) =
  anAnimal =>
    switch (anAnimal) {
    | Simon => Some(TokenId.makeFromInt(0))
    | Andy => Some(TokenId.makeFromInt(1))
    | Apthapi => Some(TokenId.makeFromInt(2))
    | Aruma => Some(TokenId.makeFromInt(3))
    | CatStevens => Some(TokenId.makeFromInt(4))
    | Cubai => Some(TokenId.makeFromInt(5))
    | Llajuita => Some(TokenId.makeFromInt(6))
    | Pancho => Some(TokenId.makeFromInt(7))
    // | Espumita => Some(TokenId.makeFromInt(8))
    | Verano => Some(TokenId.makeFromInt(9))
    | Nonhlanhla => Some(TokenId.makeFromInt(10))
    | Dlala => Some(TokenId.makeFromInt(11))
    | Isisa => Some(TokenId.makeFromInt(12))
    // | Tarkus => Some(TokenId.makeFromInt(13))
    // | Mijungla => Some(TokenId.makeFromInt(14))
    // | Ajayu => Some(TokenId.makeFromInt(15))
    | Tarkus
    | Mijungla
    | Ajayu
    // | Vitalik => "42",
    | Vitalik => None
    };

let getNameFromId: option(string) => string =
  animalId =>
    switch (animalId) {
    | None => "Vitalik"
    | Some(tokenIdSet) =>
      switch (tokenIdSet) {
      | "0" => "Nonhlanhla"
      | "1" => "Dlala"
      | "2" => "Isisa"
      | "3" => "Verano"
      | "4" => "Espumita"
      | "5" => "Pancho"
      | "6" => "Llajuita"
      | "7" => "Cubai"
      | "8" => "CatStevens"
      | "9" => "Aruma"
      | "10" => "Apthapi"
      | "11" => "Andy"
      | "12" => "Simon"
      | _ => "Unknown"
      }
    };

let getName: t => string =
  animalId =>
    switch (animalId) {
    | Vitalik => "Vitalik"
    | Simon => "Simon"
    | Andy => "Andy"
    | Verano => "Verano"
    | Tarkus => "Tarkus"
    | Pancho => "Pancho"
    | Mijungla => "Mijungla"
    | Llajuita => "Llajuita"
    // | Espumita => "Espumita"
    | Cubai => "Cubai"
    | CatStevens => "CatStevens" // removing the space since no time to fix bug
    | Aruma => "Aruma"
    | Apthapi => "Apthapi"
    | Ajayu => "Ajayu"
    | Nonhlanhla => "Nonhlanhla"
    | Isisa => "Isisa"
    | Dlala => "Dlala"
    };

let getAnimal: string => option(t) =
  animalName => {
    let animalNameLower = Js.String.toLowerCase(animalName);
    switch (animalNameLower) {
    | "vitalik" => Some(Vitalik)
    | "simon" => Some(Simon)
    | "andy" => Some(Andy)
    | "verano" => Some(Verano)
    | "tarkus" => Some(Tarkus)
    | "pancho" => Some(Pancho)
    | "mijungla" => Some(Mijungla)
    | "llajuita" => Some(Llajuita)
    // | "espumita" => Some(Espumita)
    | "cubai" => Some(Cubai)
    | "catstevens" => Some(CatStevens)
    | "aruma" => Some(Aruma)
    | "apthapi" => Some(Apthapi)
    | "ajayu" => Some(Ajayu)
    | "dlala" => Some(Dlala)
    | "isisa" => Some(Isisa)
    | "nonhlanhla" => Some(Nonhlanhla)
    | _ => None
    };
  };

let getNextPrev = animal =>
  switch (animal) {
  | Simon => (Vitalik, Dlala)
  | Vitalik => (Andy, Simon)
  | Andy => (Verano, Vitalik)
  | Verano => (Tarkus, Andy)
  | Tarkus => (Pancho, Verano)
  | Pancho => (Mijungla, Tarkus)
  | Mijungla => (Llajuita, Pancho)
  | Llajuita => (Cubai, Mijungla)
  // | Espumita => (Cubai, Llajuita)
  | Cubai => (CatStevens, Llajuita)
  | CatStevens => (Aruma, Cubai)
  | Aruma => (Apthapi, CatStevens)
  | Apthapi => (Ajayu, Aruma)
  | Ajayu => (Nonhlanhla, Apthapi)
  | Nonhlanhla => (Isisa, Ajayu)
  | Isisa => (Simon, Nonhlanhla)
  | Dlala => (Simon, Isisa)
  | _ => (Simon, Vitalik)
  };

let getImage = animal =>
  switch (animal) {
  | Vitalik =>
    %bs.raw
    {|require('../../img/animals/gorilla1.png')|}
  | Simon =>
    %bs.raw
    {|require('../../img/animals/gorilla2.png')|}
  | Andy =>
    %bs.raw
    {|require('../../img/animals/gorilla3.png')|}
  | Pancho =>
    %bs.raw
    {|require('../../img/animals/PanchoTheTortoiseCropped.png')|}
  | Llajuita =>
    %bs.raw
    {|require('../../img/animals/LlajuitaTheDeerCropped.png')|}
  // | Espumita =>
  //   %bs.raw
  //   {|require('../../img/animals/EspumitaThePuma2Cropped.png')|}
  | Cubai =>
    %bs.raw
    {|require('../../img/animals/CubaiTheJaguarCropped.png')|}
  | CatStevens =>
    %bs.raw
    {|require('../../img/animals/catstevenstheocelotCropped.png')|}
  | Aruma =>
    %bs.raw
    {|require('../../img/animals/ArumaTheAndeanBearCropped.png')|}
  | Apthapi =>
    %bs.raw
    {|require('../../img/animals/ApthapiTheTapirCropped.png')|}
  | Verano =>
    %bs.raw
    {|require('../../img/animals/VeranoTheMackawCropped.png')|}
  | Nonhlanhla =>
    %bs.raw
    {|require('../../img/animals/EditedHyena.png')|}
  | Isisa =>
    %bs.raw
    {|require('../../img/animals/editedHyena3.png')|}
  | Dlala =>
    %bs.raw
    {|require('../../img/animals/EditedHyena2.png')|}
  | Ajayu
  | Mijungla
  | Tarkus
  | _ =>
    %bs.raw
    {|require('../../img/animals/comingsoon.png')|}
  };

// let getAlternateImage: t => array(string) = // TODO: I want to turn this into an array in the future, show a carousel of images instead.
let getAlternateImage: t => option(string) =
  animal =>
    switch (animal) {
    | Simon => None
    | Andy => None
    | Vitalik => None
    | Verano => Some([%bs.raw {|require('../../img/animals/Verano.jpg')|}])
    | Pancho => Some([%bs.raw {|require('../../img/animals/Pancho.jpg')|}])
    | Llajuita =>
      Some([%bs.raw {|require('../../img/animals/Llajuita.jpg')|}])
    // | Espumita =>
    //   Some([%bs.raw {|require('../../img/animals/Espumita.jpg')|}])
    | Cubai =>
      %bs.raw
      {|require('../../img/animals/Cubai.jpg')|}
    | CatStevens =>
      %bs.raw
      {|require('../../img/animals/CatStevens.jpg')|}
    | Aruma =>
      %bs.raw
      {|require('../../img/animals/Aruma.jpg')|}
    | Apthapi =>
      %bs.raw
      {|require('../../img/animals/Apthapi.jpg')|}
    | Nonhlanhla =>
      %bs.raw
      {|require('../../img/animals/Nonhlanhla.jpg')|}
    | Isisa =>
      %bs.raw
      {|require('../../img/animals/Isisa.jpg')|}
    | Dlala =>
      %bs.raw
      {|require('../../img/animals/Dlala.jpg')|}
    // | Ajayu =>
    //   %bs.raw
    //   {|require('../../img/animals/comingsoon.png')|}
    // | Mijungla =>
    //   %bs.raw
    //   {|require('../../img/animals/comingsoon.png')|}
    // | Tarkus =>
    //   %bs.raw
    //   {|require('../../img/animals/comingsoon.png')|}
    | _ => None
    };

let getOrgBadgeImage: t => option(string) =
  animal =>
    switch (animal) {
    | Simon
    | Andy
    | Vitalik => Some([%bs.raw {|require('../../img/badges/OGBadge.png')|}])
    | Pancho
    | Llajuita
    // | Espumita
    | Cubai
    | CatStevens
    | Aruma
    | Apthapi
    | Verano
    | Ajayu
    | Mijungla
    | Tarkus =>
      Some([%bs.raw {|require('../../img/badges/SendaVerdeBadge.png')|}])
    | Nonhlanhla
    | Isisa
    | Dlala =>
      Some([%bs.raw {|require('../../img/badges/WildTomorrowBadge.png')|}])
    // | _ => None
    };

let getStoryParagraphs = animal =>
  switch (animal) {
  | Vitalik => [|
      "Original Gorilla",
      "Vitalik is the first ever animal launched on wildcards and therefore often termed an OG (Original Gorilla). The wildcards project was born at the #ETHCapeTown hackathon in May 2019 where Vitalik Buterin was one of the judges. We named Vitalik the Gorilla after Vitalik as a testament to the impact and innovation Vitalik Buterin has had on the blockchain ecosystem. Vitalik, if you are reading this, start saving some animals and buy me!",
      "Funds raised by Vitalik, the first wildcard, flow toward the Wild Tomorrow Fund, our first conservation partner.",
    |]
  | Simon => [|
      "Original Gorilla",
      "Simon, named after Simon de la Rouviere originated from the deep dark github jungle, and his code \'this artwork is always on sale\' inspired this project. He is a wizard who put us on the right track. He has helped inform some of our design decisions regarding our subsequent smart contracts, and always been willing to lend some great advice or checkout a GitHub issue for us.",
      "Funds raised by Simon, one of the first wildcards, flow toward the Wild Tomorrow Fund, our first conservation partner.",
    |]
  | Andy => [|
      "Original Gorilla",
      "Andy was named after the Crypto Wanderer Andy Tudhope. An interplanetary Cat-herder and early crypto adopter. ",
      "Funds raised by Andy, one of the first wildcards, flow toward the Wild Tomorrow Fund, our first conservation partner.",
    |]
  | Verano => [|
      "Verano is a victim of illegal wildlife trafficking. He was confiscated when he was being transported to the black market in the city of La Paz. Macaws and especially Blue-and-Yellow macaws have been an all time favourite for the pet trade market. They are very smart birds and in comparison to other wildlife, are easily domesticated. They are beloved because of their intelligence, their ability to imitate sounds, their beauty and their companionship.",
      "In countries like the United States, Blue-and-Yellow Macaws are raised as pets and a legal pet market exists for them. This is not the case in Bolivia however. There are absolutely no regulations on the matter and all pet parrots and macaws come from the illegal market. This means that they were stolen when chicks from their nests and went through a lot of suffering being transported illegally in hide-outs where they can barely breathe. Once with their final owners, their life is not always the best. Most parrot and macaw owners in Bolivia have no knowledge on the dietary and life requirements for these animals and they normally survive in very precarious conditions. Many of them are kept in La Paz, where low temperatures and high altitude are far from the suitable environment they need.",
      "When macaws like Verano arrive to La Senda Verde, they normally do so with their plumage deteriorated and with signs of psychological trauma. They go through a long recovery phase until they can be finally released into our new large aviaries, where they receive all the attention they need.",
    |]
  | Tarkus => [|
      "Tarkus was found on the roadside by a trucker in a section between Sucre and Tarija where Andean bears are known to exist. A female bear got scared by the truck and run with the two cubs. The smallest one got a bit behind. The trucker decided to stop, capture the cub and take him to his house, where he kept the him for more than two weeks. Later he called the environmental authorities in the City of Tarija to turn him in. Capturing the cub is never a good idea, it is virtually condemning him to a life in captivity for he will never be able to learn the abilities to survive in the wild his mother would have taught him. If he would have been left alone, the mother would have come back for him later.",
      "Tarkus stayed in Tarija for three weeks while the formal requirements were fulfilled. National authorities advised he should be sent to La Senda Verde, where there is a place for the cub, but also the experience of raising bear cubs with appropriate diet and management. He has been placed in a 6x5x6 enclosure just for him, and he has been getting well habituated to the refuge and the caretakers.",
      "Tarkus has adapted well to the refuge and loves to play with the ball and in the water.",
    |]
  | Pancho => [|
      "Pancho is a large tortoise that was rescued before he was going to be cooked. He had spent by then a long time as a pet and that can be seen by the deformations on his shell, which show mistreatment and poor feeding habits.",
      "Pancho is one of the many tortoises that have similar stories and that arrive in large numbers to La Senda Verde. Tortoises are easy to catch in the rainforest and for that reason many of them are often taken to be sold as pets.",
    |]
  | Mijungla => [|
      "Mijungla arrived December 13th to La Senda Verde to be the second jaguar to live in the animal refuge. She was picked up from a family in the town of Palos Blancos, located in a low tropical rainforest area where the Amazon meets the Andes.",
      "The family has a cattle ranch, and one of the cowboys in the ranch had found her in a very bad state of health laying in the forest, with a wound in her head, full of skin parasites and parts of her body without skin. They picked her up and started rehabilitating her. First they fed her the wrong food, then they realised they needed to feed her meat. It was until she arrived with us that she was able to drink cat milk, for she was so small she really needed it.",
      "The cub was partially rehabilitated by the family but still had some difficulties when she arrived at Senda Verde. She couldn't walk very well and her stomach was swollen. Just like the Andean Bear cub Tarkus that arrived around the same time Mijungla did, this Jaguar cub spent very little time with her mother, which affects them greatly both psychologically and physiologically. Senda Verde monitors very closely both cub's digestion since they could get sick because of the fact that they did not get enough milk from their mothers.",
      "When arrived, Mijungla was three months old and weighted 3.2 kilograms.",
    |]
  | Llajuita => [|
      "Llajuita arrived at La Senda Verde with 8-10 months of age along with Apthapi the Tapir, which has the same age. They come from a confiscation performed by the Forestry and Environment Police (POFOMA) in Palos Blancos, North of La Paz. Both mammals are large and require a lot of space. They are also both foragers and get along well.",
      "They lived in a small enclosure until Senda Verde was able to build a larger one for them. The new enclosure has a pool since Tapirs need to swim. For both foraging mammals Senda Verde has alfalfa growing in the enclosure. Sadly in the dry season this alfalfa dries out and runs scarce and Senda Verde has trouble staying to date with these forager's diets.",
    |]
  // | Espumita => [|
  //     "Espumita has been with La Senda Verde for just over two years. She came as a small kitten from the authorities in La Paz who had confiscated her from a poacher. At the time La Senda Verde didn't have anywhere suitable to accommodate her and as a small kitten she spent her first few months in the clinic. La Senda Verde raised funds and built her a management cage whilst she was still growing. Then, at the beginning of 2017 they completed her full sized enclosure with high platforms, on a hill so she can get to higher ground and with a den at the top that she loves to sit in and watch out from.",
  //     "Espumita has always been quite a shy and easily stressed cat but loves her new home. In December Espumita had unfortunately been under stress and was pacing a lot due to recent construction works going on close to her enclosure. Unfortunately, Espumita also wasn’t eating very well and had lost condition. Eight weeks ago, la Senda Verde sedated her and a visiting group of vets studying ultrasonography came and performed an ultrasound on her and we were able to give her a thorough check. It was found that she likely had some mild hepatic lipidosis (a form of liver disease) which was also contributing to her loss of appetite.",
  //     "Along with one of the sanctuary staff members, Vicky from Senda Verde started to add in extra enrichment every day for Espumita and slightly change the time and way in which she was fed. Luckily, at this time the construction was also finished. Over the next eight weeks there was an amazing transformation! Espumita started to eat normally, gain weight and is noticeably more relaxed and confident. She comes down to greet the staff from la Senda Verde every time they go for enrichment or cleaning and feeding (which she never did before) and Senda Verde hasn't seen her do any pacing that she was doing before.",
  //     "She absolutely loves cardboard boxes of any shape or size! La Senda Verde started off with relaxing smells such as chamomile flowers and lavender in cardboard boxes. They have also been using feathers which she loves. She is still a huge fan of lavender and will roll in it and will then often have a mad ten minutes running all over her enclosure with the box and up on her platforms having so much fun.",
  //   |]
  | Cubai => [|
      "Cubai is a 15 year old Jaguar that in 2017 was relocated from a private property in the Yungas region to the La Paz Zoo. The animal was in good hands, but his advanced age mixed with the cold weather and the high altitude of La Paz, were beginning to affect his health, he needed a lower place to live in.",
      "It was then that the authorities asked Senda Verde if they could receive him. In face of the urgency they decided to do so and make him part of their family. Cubai arrived on the 20th of November and at the beginning was being kept in a small, temporary enclosure, while la senda verde gathered funds to build him a proper enclosure.",
      "Luckily he didn’t stay there for long. A few months were enough to build a brand new enclosure, with management cage, platform, pool, the whole works! The place where it’s located has a lot of trees, vegetation and is near the river. He now spends his days playing around and enjoying a space he never had before in his lifetime.",
    |]
  | CatStevens => [|
      "Cat Stevens is a male Ocelot that was rescued by an engineer working at the San Buenaventura sugar factory. This Factory was finished in 2016 and was built in the middle of the northern Madidi forests, just outside the national park. There is no suitable land to grow sugar cane to feed this factory, so they have simply chosen to destroy the nearby primary rainforest for that. So far 1,200 hectares have been destroyed to lay way for the cane plantations, and many more are coming. Thousands of animals have seen their homes destroyed before their eyes. Cat Stevens is one of them. Clear-cutting of the forest happened so fast (two years) that these animals didn't have time to run somewhere else or fall prey to local bush-meat hunters. They simply where left there to wonder in what once was a forest.",
      "Cat Stevens was a baby then seen wondering around in a empty field by one of the engineers. He had probably lost his mother and was probably going to die if he wasn't rescued by the engineer. Cat Stevens was a small cat when he arrived. He was underweight and la Senda Verde noticed he could not walk too well, but assumed it was because of age and undernourishment. After a month, he gained weight and suddenly stopped walking. He was taken to La Paz for X-ray and noticed he had an old fracture that would not heal due to decalcification. After a couple of weeks of feeding him with cat formula, he recovered and started to walk again. Cat Stevens was alone for a few months until Cat Iris arrival, a female ocelot. They where put together and get along well.",
    |]
  | Aruma => [|
      "He is the first bear to arrive and was the game-changer for LSV. After he arrived we knew that our mission was to shelter and rescue animals. Aruma means 'night' in Aymara language.",
      "A little boy and his dad had stumbled in the forest upon a mother bear with two cubs. The mother and one of the cubs ran away but the other cub stood there frozen. The cub was taken by the little boy and he tried to sell it in the town of Quime. The doctor in town noticed that the cub had terrible diarrhea, called the police and the bear cub was confiscated from the little boy. The doctor arrived in La Paz at 4 am, and called Animals SOS. Aruma stayed with a volunteer for 2 months while he was getting medical treatment, until he was sent to La Senda Verde in December 2007.",
    |]
  | Apthapi => [|
      "Apthapi arrived at La Senda Verde with 8-10 months of age along with a female deer of the same age. They came from a confiscation performed by the Forestry and Environment Police (POFOMA) in Palos Blancos, North of La Paz. The tapir is the largest mammal in South America. They are known to be the \'gardeners of the forest\' because they are nutrient recyclers. They are also excellent swimmers.",
    |]
  | Ajayu => [|
      "Ajayu was saved by the police in Cochabamba from being brutally beaten to death by a local nearby community in 2016. He arrived in La Paz and La Senda Verde took care of his treatments. He received several surgeries, one to save his left eye and one to save his jaw from a severe infection. He stopped eating for days and had to be force fed. It was a struggle to save him, but with time he got better and is now weighing 100 kilograms.",
      "As a result from this brutal story was that la Senda Verde has become more involved in trying to change legislation in Bolivia to protect this species and avoid stories like this one from happening again. They are now working alongside biologist Ximena Velez-Liendo to present a law project that will be called the Ajayu law, named after this bear. Ajayu means spirit in Aymara language.",
    |]
  | Nonhlanhla => [|
      "Nonhlanhla is an 11 year old female spotted hyena who lives in the forests and grasslands of central Zululand, South Africa. Though spotted hyenas can be difficult to see in this area, Nonhlanhla is a particularly great breeder in her clan having cubs regularly, meaning she is seen often at the den site nursing her young. In an area where spotted hyenas are persecuted and there is evidence for their decline, her name in Zulu, appropriately means \"luck\".",
    |]
  | Isisa => [|
      "Isisa is a 10 year old female spotted hyena in a small clan who inhabit the rocky outcrops and low-lying mountains of Zululand. Isisa is the mother of the young male hyena Dlala and is often seen in the Tamboti tree lined drainage canyons in the area. She is an incredibly successful hunter and is often found to have brought down large nyala antelope by herself. ",
    |]
  | Dlala => [|
      "Dlala is a young male spotted hyena, currently about 13 months old. At this age he has recently grown large enough to leave the safety of his clan's den and is out exploring the world within their territory. Too young to breed, his current existence is more about learning and interacting with his new environment; in Zulu his name means \"play\".",
    |]
  };

let useBuy = animal => {
  open Hooks;

  let animalId = getId(animal);
  switch (animalId) {
  | None =>
    let buyObj = useBuyTransaction();
    (
      (
        (newPrice, txObject) =>
          buyObj##send(. newPrice->Web3Utils.toWeiFromEth, txObject)
      ),
      buyObj##_TXObjects,
    );
  | Some(tokenIdSet) =>
    let buyObj = useBuyTransactionNew();
    (
      (
        (newPrice, txObject) =>
          buyObj##send(.
            tokenIdSet,
            newPrice->Web3Utils.toWeiFromEth,
            txObject,
          )
      ),
      buyObj##_TXObjects,
    );
  };
};

let useCurrentPrice = animal => {
  open Hooks;
  open Accounting;

  let animalId = getId(animal);
  switch (animalId) {
  | None => useCurrentPriceWei()->defaultZeroS
  | Some(tokenIdSet) => useCurrentPriceWeiNew(tokenIdSet)->defaultZeroS
  };
};

// TODO: you could get this from the contract, but seems like a waste
let pledgeRate = animal => {
  let animalId = getId(animal);

  // TODO: get this from the blockchain / graph - bad hardcoding it here!
  switch (animalId) {
  | None => ("3", "10", 0.025, 40.)
  | Some(tokenIdSet) =>
    switch (tokenIdSet) {
    | "2"
    | "7" => ("12", "10", 0.1, 10.)
    | "0"
    | "1"
    | "3"
    | "4"
    | "6"
    | "10"
    | "11" => ("24", "10", 0.2, 5.)
    | "5"
    | "8"
    | "12" => ("60", "10", 0.5, 2.)
    | "9" => ("120", "10", 1., 1.)
    | _ => ("24", "10", 0.2, 5.)
    }
  };
};

let useCurrentPriceEth = animal => {
  open Belt.Option;
  let animalId = getId(animal);
  (
    switch (animalId) {
    | None => Hooks.useCurrentPriceEth()
    | Some(tokenIdSet) => Hooks.useCurrentPriceEthNew(tokenIdSet)
    }
  )
  ->mapWithDefault("loading", a => a);
};
let useCurrentPriceUsd = animal => {
  open Belt.Option;
  let animalId = getId(animal);
  (
    switch (animalId) {
    | None => Hooks.useCurrentPriceUsd()
    | Some(tokenIdSet) => Hooks.useCurrentPriceUsdNew(tokenIdSet)
    }
  )
  ->mapWithDefault("loading", a => a);
};

type launchStatus =
  | Launched
  | LaunchDate(MomentRe.Moment.t);

let isLaunched: t => launchStatus =
  anAnimal =>
    switch (anAnimal) {
    | Simon => Launched
    | Andy => Launched
    | Vitalik => Launched
    | Cubai =>
      LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-03T11:00:00"))
    | Dlala =>
      LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-03T11:00:00"))
    | Verano =>
      LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-03T11:00:00"))
    | CatStevens =>
      LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-03T13:00:00"))
    | Pancho =>
      LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-03T15:00:00"))
    | Isisa =>
      LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-03T17:00:00"))
    | Llajuita =>
      LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-04T17:00:00"))
    | Apthapi =>
      LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-05T17:00:00"))
    | Aruma =>
      LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-06T17:00:00"))
    | Nonhlanhla =>
      LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-07T21:00:00"))
    | _ => LaunchDate(MomentRe.momentUtcDefaultFormat("2019-12-10T11:00:00"))
    };

let useIsAnimalOwened = animal => {
  let animalId = animal->getId;
  let currentAccount =
    Hooks.useCurrentUser()->Belt.Option.mapWithDefault("loading", a => a);
  let currentPatron =
    switch (animalId) {
    | Some(id) =>
      Hooks.useCurrentPatronNew(id->int_of_string)
      ->Belt.Option.mapWithDefault("no-patron-defined", a => a)

    | None =>
      Hooks.useCurrentPatron()
      ->Belt.Option.mapWithDefault("no-patron-defined", a => a)
    };
  currentAccount == currentPatron;
};
