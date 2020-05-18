type t =
  | Andy
  | Glen
  | Vitalik
  | Simon
  | Verano
  // | Tarkus
  | Pancho
  // | Mijungla
  | Llajuita
  | Espumita
  | Cubai
  | CatStevens
  | Aruma
  | Apthapi
  // | Ajayu
  | Isisa
  | Dlala
  | Nonhlanhla
  | Tarkus
  | Ucok;

let orderedArray = [|
  Apthapi,
  // Ajayu,
  Vitalik,
  Nonhlanhla,
  Cubai,
  Glen,
  Simon,
  Dlala,
  Aruma,
  Verano,
  // Tarkus,
  Pancho,
  Andy,
  // Mijungla,
  Llajuita,
  Espumita,
  CatStevens,
  Isisa,
  Tarkus,
  Ucok,
|];

let getId: t => string =
  anAnimal =>
    switch (anAnimal) {
    | Simon => "0"
    | Andy => "1"
    | Apthapi => "2"
    | Aruma => "3"
    | CatStevens => "4"
    | Cubai => "5"
    | Llajuita => "6"
    | Pancho => "7"
    | Espumita => "8"
    | Verano => "9"
    | Nonhlanhla => "10"
    | Dlala => "11"
    | Isisa => "12"
    | Glen => "13"
    | Ucok => "14"
    | Tarkus => "15"
    // | Vitalik => Some("42") // We don't show an ID for vitalik since using legacy contract still.
    // | Tarkus
    // | Mijungla
    // | Ajayu
    | Vitalik => "42"
    };

let getTokenId: t => TokenId.t =
  anAnimal =>
    switch (anAnimal) {
    | Simon => TokenId.makeFromInt(0)
    | Andy => TokenId.makeFromInt(1)
    | Apthapi => TokenId.makeFromInt(2)
    | Aruma => TokenId.makeFromInt(3)
    | CatStevens => TokenId.makeFromInt(4)
    | Cubai => TokenId.makeFromInt(5)
    | Llajuita => TokenId.makeFromInt(6)
    | Pancho => TokenId.makeFromInt(7)
    | Espumita => TokenId.makeFromInt(8)
    | Verano => TokenId.makeFromInt(9)
    | Nonhlanhla => TokenId.makeFromInt(10)
    | Dlala => TokenId.makeFromInt(11)
    | Isisa => TokenId.makeFromInt(12)
    | Glen => TokenId.makeFromInt(13)
    | Ucok => TokenId.makeFromInt(14)
    | Vitalik => TokenId.makeFromInt(42)
    | Tarkus => TokenId.makeFromInt(15)
    // | Mijungla => Some(TokenId.makeFromInt(15))
    // | Ajayu => Some(TokenId.makeFromInt(16))
    // | Tarkus
    // | Mijungla
    // | Ajayu
    };

let getNameFromId: string => string =
  animalId =>
    switch (animalId) {
    | "0" => "Simon"
    | "1" => "Andy"
    | "2" => "Apthapi"
    | "3" => "Aruma"
    | "4" => "CatStevens"
    | "5" => "Cubai"
    | "6" => "Llajuita"
    | "7" => "Pancho"
    | "8" => "Espumita"
    | "9" => "Verano"
    | "10" => "Nonhlanhla"
    | "11" => "Dlala"
    | "12" => "Isisa"
    | "13" => "Glen"
    | "14" => "Ucok"
    | "15" => "Tarkus"
    | "42" => "Vitalik"
    | _ => "Unknown"
    };

let getName: t => string =
  animalId =>
    switch (animalId) {
    | Vitalik => "Vitalik"
    | Simon => "Simon"
    | Andy => "Andy"
    | Verano => "Verano"
    // | Tarkus => "Tarkus"
    | Pancho => "Pancho"
    // | Mijungla => "Mijungla"
    | Llajuita => "Llajuita"
    | Espumita => "Espumita"
    | Cubai => "Cubai"
    | CatStevens => "CatStevens" // removing the space since no time to fix bug
    | Aruma => "Aruma"
    | Apthapi => "Apthapi"
    // | Ajayu => "Ajayu"
    | Nonhlanhla => "Nonhlanhla"
    | Isisa => "Isisa"
    | Dlala => "Dlala"
    | Ucok => "Ucok"
    | Glen => "Glen"
    | Tarkus => "Tarkus"
    };

let getAnimal: string => option(t) =
  animalName => {
    let animalNameLower = Js.String.toLowerCase(animalName);
    switch (animalNameLower) {
    | "vitalik" => Some(Vitalik)
    | "simon" => Some(Simon)
    | "andy" => Some(Andy)
    | "verano" => Some(Verano)
    // | "tarkus" => Some(Tarkus)
    | "pancho" => Some(Pancho)
    // | "mijungla" => Some(Mijungla)
    | "llajuita" => Some(Llajuita)
    | "espumita" => Some(Espumita)
    | "cubai" => Some(Cubai)
    | "catstevens" => Some(CatStevens)
    | "aruma" => Some(Aruma)
    | "apthapi" => Some(Apthapi)
    // | "ajayu" => Some(Ajayu)
    | "dlala" => Some(Dlala)
    | "isisa" => Some(Isisa)
    | "nonhlanhla" => Some(Nonhlanhla)
    | "glen" => Some(Glen)
    | "ucok" => Some(Ucok)
    | "tarkus" => Some(Tarkus)
    | _ => None
    };
  };

// TODO: deprecate this method, we should use TokenId.t everywhere instead.
let getAnimalFromId: string => option(t) =
  animalId => {
    switch (animalId) {
    | "42" => Some(Vitalik)
    | "0" => Some(Simon)
    | "1" => Some(Andy)
    | "2" => Some(Apthapi)
    | "3" => Some(Aruma)
    | "4" => Some(CatStevens)
    | "5" => Some(Cubai)
    | "6" => Some(Llajuita)
    | "7" => Some(Pancho)
    | "8" => Some(Espumita)
    | "9" => Some(Verano)
    | "10" => Some(Nonhlanhla)
    | "11" => Some(Dlala)
    | "12" => Some(Isisa)
    | "13" => Some(Glen)
    | "14" => Some(Ucok)
    | "15" => Some(Tarkus)
    | _ => None
    };
  };

let getNextPrev = animal =>
  switch (animal) {
  | Simon => (Vitalik, Dlala)
  | Vitalik => (Andy, Simon)
  | Andy => (Verano, Vitalik)
  | Verano => (Pancho, Andy)
  // | Tarkus => (Pancho, Verano)
  | Pancho => (Llajuita, Verano)
  // | Mijungla => (Llajuita, Pancho)
  | Llajuita => (Espumita, Pancho)
  | Espumita => (Cubai, Llajuita)
  | Cubai => (CatStevens, Espumita)
  | CatStevens => (Aruma, Cubai)
  | Aruma => (Apthapi, CatStevens)
  | Apthapi => (Nonhlanhla, Aruma)
  // | Ajayu => (Nonhlanhla, Apthapi)
  | Nonhlanhla => (Isisa, Apthapi)
  | Isisa => (Glen, Nonhlanhla)
  | Glen => (Ucok, Isisa)
  | Ucok => (Tarkus, Glen)
  | Tarkus => (Dlala, Ucok)
  | Dlala => (Simon, Tarkus)
  };

let getImage = animal =>
  switch (animal) {
  | Vitalik => "/img/animals/gorilla1.png"
  | Simon => "/img/animals/gorilla2.png"
  | Andy => "/img/animals/gorilla3.png"
  | Pancho => "/img/animals/PanchoTheTortoiseCropped.png"
  | Llajuita => "/img/animals/LlajuitaTheDeerCropped.png"
  | Espumita => "/img/animals/EspumitaThePuma2Cropped.png"
  | Cubai => "/img/animals/CubaiTheJaguarCropped.png"
  | CatStevens => "/img/animals/catstevenstheocelotCropped.png"
  | Aruma => "/img/animals/ArumaTheAndeanBearCropped.png"
  | Apthapi => "/img/animals/ApthapiTheTapirCropped.png"
  | Verano => "/img/animals/VeranoTheMackawCropped.png"
  | Nonhlanhla => "/img/animals/EditedHyena.png"
  | Isisa => "/img/animals/editedHyena3.png"
  | Dlala => "/img/animals/EditedHyena2.png"
  | Glen => "/img/animals/Glen.png"
  | Ucok => "/img/animals/Ucok.png"
  | Tarkus => "/img/animals/Tarkus.png"
  // | Ajayu
  // | Mijungla
  // | Tarkus
  | _ => "/img/animals/comingsoon.png"
  };

// let getAlternateImage: t => array(string) = // TODO: I want to turn this into an array in the future, show a carousel of images instead.
let getAlternateImage: t => option(string) =
  animal =>
    switch (animal) {
    | Simon
    | Andy
    | Glen
    | Vitalik => None
    | Ucok => Some("/img/animals/Ucokreal.jpg")
    | Verano => Some("/img/animals/Verano.jpg")
    | Pancho => Some("/img/animals/Pancho.jpg")
    | Llajuita => Some("/img/animals/Llajuita.jpg")
    | Espumita => Some("/img/animals/Espumita.jpg")
    | Cubai => Some("/img/animals/Cubai.jpg")
    | CatStevens => Some("/img/animals/CatStevens.jpg")
    | Aruma => Some("/img/animals/Aruma.jpg")
    | Apthapi => Some("/img/animals/Apthapi.jpg")
    | Nonhlanhla => Some("/img/animals/Nonhlanhla.jpg")
    | Isisa => Some("/img/animals/Isisa.jpg")
    | Dlala => Some("/img/animals/Dlala.jpg")
    | Tarkus => Some("/img/animals/TarkusReal.jpg")
    | _ => None
    };

let getOrgBadgeImage: t => string =
  animal =>
    switch (animal) {
    | Simon
    | Andy
    | Vitalik => "/img/badges/OGBadge.png"
    | Pancho
    | Llajuita
    | Cubai
    | CatStevens
    | Aruma
    | Apthapi
    | Espumita
    | Tarkus
    | Verano => "/img/badges/SendaVerdeBadge.png"
    | Nonhlanhla
    | Isisa
    | Dlala => "/img/badges/WildTomorrowBadge.png"
    | Glen => "/img/badges/EthTurin.svg"
    | Ucok => "/img/badges/DarwinAnimalDoctors.svg"
    // | Ajayu
    // | Mijungla
    // | Tarkus
    // | _ => None
    };

let getStoryParagraphs = animal =>
  switch (animal) {
  | Vitalik => [|
      "Original Gorilla",
      "Vitalik is the first ever animal launched on wildcards and therefore often termed an OG (Original Gorilla). The wildcards project was born at the #ETHCapeTown hackathon in May 2019 where Vitalik Buterin was one of the judges. We named Vitalik the Gorilla after Vitalik as a testament to the impact and innovation Vitalik Buterin has had on the blockchain ecosystem. Vitalik, if you are reading this, start saving some animals and buy me!",
      "Funds raised by Vitalik, initially, will go to Wild Tomorrow Fund; however Vitalik doesn\'t represent a real gorilla, and funds for Vitalik may be distributed via another allocation mechanism in the future (DAO or otherwise).",
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
      "Tarkus was found on the roadside by a trucker in a section between Sucre and Tarija where Andean bears are known to exist. A female bear got scared by the truck and ran with the two cubs. The smallest one fell a bit behind. The trucker decided to stop, capture the cub and take him to his house, where he kept him for more than two weeks. Later he called the environmental authorities in the City of Tarija to turn him in. Capturing the cub is never a good idea, it is virtually condemning him to a life in captivity for he will never be able to learn the abilities to survive in the wild his mother would have taught him. If he would have been left alone, the mother would have come back for him later.",
      "Tarkus stayed in Tarija for three weeks while the formal requirements were fulfilled. National authorities advised he should be sent to La Senda Verde, where there is a place for the cub, but also the experience of raising bear cubs with appropriate diet and management. He has been placed in a 6x5x6 enclosure, just for him, and he has been getting used to the refuge and the caretakers.",
      "Tarkus has adapted well to the refuge and loves to play with the ball and in the water.",
    |]
  | Pancho => [|
      "Pancho is a large tortoise that was rescued before he was going to be cooked. He had spent by then a long time as a pet and that can be seen by the deformations on his shell, which show mistreatment and poor feeding habits.",
      "Pancho is one of the many tortoises that have similar stories and that arrive in large numbers to La Senda Verde. Tortoises are easy to catch in the rainforest and for that reason many of them are often taken to be sold as pets.",
    |]
  // | Mijungla => [|
  //     "Mijungla arrived December 13th to La Senda Verde to be the second jaguar to live in the animal refuge. She was picked up from a family in the town of Palos Blancos, located in a low tropical rainforest area where the Amazon meets the Andes.",
  //     "The family has a cattle ranch, and one of the cowboys in the ranch had found her in a very bad state of health laying in the forest, with a wound in her head, full of skin parasites and parts of her body without skin. They picked her up and started rehabilitating her. First they fed her the wrong food, then they realised they needed to feed her meat. It was until she arrived with us that she was able to drink cat milk, for she was so small she really needed it.",
  //     "The cub was partially rehabilitated by the family but still had some difficulties when she arrived at Senda Verde. She couldn't walk very well and her stomach was swollen. Just like the Andean Bear cub Tarkus that arrived around the same time Mijungla did, this Jaguar cub spent very little time with her mother, which affects them greatly both psychologically and physiologically. Senda Verde monitors very closely both cub's digestion since they could get sick because of the fact that they did not get enough milk from their mothers.",
  //     "When arrived, Mijungla was three months old and weighted 3.2 kilograms.",
  //   |]
  | Llajuita => [|
      "Llajuita arrived at La Senda Verde with 8-10 months of age along with Apthapi the Tapir, which has the same age. They come from a confiscation performed by the Forestry and Environment Police (POFOMA) in Palos Blancos, North of La Paz. Both mammals are large and require a lot of space. They are also both foragers and get along well.",
      "They lived in a small enclosure until Senda Verde was able to build a larger one for them. The new enclosure has a pool since Tapirs need to swim. For both foraging mammals Senda Verde has alfalfa growing in the enclosure. Sadly in the dry season this alfalfa dries out and runs scarce and Senda Verde has trouble staying to date with these forager's diets.",
    |]
  | Espumita => [|
      "Espumita has been with La Senda Verde for just over two years. She came as a small kitten from the authorities in La Paz who had confiscated her from a poacher. At the time La Senda Verde didn't have anywhere suitable to accommodate her and as a small kitten she spent her first few months in the clinic. La Senda Verde raised funds and built her a management cage whilst she was still growing. Then, at the beginning of 2017 they completed her full sized enclosure with high platforms, on a hill so she can get to higher ground and with a den at the top that she loves to sit in and watch out from.",
      "Espumita has always been quite a shy and easily stressed cat but loves her new home. In December Espumita had unfortunately been under stress and was pacing a lot due to recent construction works going on close to her enclosure. Unfortunately, Espumita also wasn't eating very well and had lost condition. Eight weeks ago, la Senda Verde sedated her and a visiting group of vets studying ultrasonography came and performed an ultrasound on her and we were able to give her a thorough check. It was found that she likely had some mild hepatic lipidosis (a form of liver disease) which was also contributing to her loss of appetite.",
      "Along with one of the sanctuary staff members, Vicky from Senda Verde started to add in extra enrichment every day for Espumita and slightly change the time and way in which she was fed. Luckily, at this time the construction was also finished. Over the next eight weeks there was an amazing transformation! Espumita started to eat normally, gain weight and is noticeably more relaxed and confident. She comes down to greet the staff from la Senda Verde every time they go for enrichment or cleaning and feeding (which she never did before) and Senda Verde hasn't seen her do any pacing that she was doing before.",
      "She absolutely loves cardboard boxes of any shape or size! La Senda Verde started off with relaxing smells such as chamomile flowers and lavender in cardboard boxes. They have also been using feathers which she loves. She is still a huge fan of lavender and will roll in it and will then often have a mad ten minutes running all over her enclosure with the box and up on her platforms having so much fun.",
    |]
  | Cubai => [|
      "Cubai is a 15 year old Jaguar that in 2017 was relocated from a private property in the Yungas region to the La Paz Zoo. The animal was in good hands, but his advanced age mixed with the cold weather and the high altitude of La Paz, were beginning to affect his health, he needed a lower place to live in.",
      "It was then that the authorities asked Senda Verde if they could receive him. In face of the urgency they decided to do so and make him part of their family. Cubai arrived on the 20th of November and at the beginning was being kept in a small, temporary enclosure, while la senda verde gathered funds to build him a proper enclosure.",
      "Luckily he didn't stay there for long. A few months were enough to build a brand new enclosure, with management cage, platform, pool, the whole works! The place where it's located has a lot of trees, vegetation and is near the river. He now spends his days playing around and enjoying a space he never had before in his lifetime.",
    |]
  | CatStevens => [|
      "Cat Stevens is a male Ocelot that was rescued by an engineer working at the San Buenaventura sugar factory. This Factory was finished in 2016 and was built in the middle of the northern Madidi forests, just outside the national park. There is no suitable land to grow sugar cane to feed this factory, so they have simply chosen to destroy the nearby primary rainforest for that. So far 1,200 hectares have been destroyed to lay way for the cane plantations, and many more are coming. Thousands of animals have seen their homes destroyed before their eyes. Cat Stevens is one of them. Clear-cutting of the forest happened so fast (two years) that these animals didn't have time to run somewhere else or fall prey to local bush-meat hunters. They simply where left there to wonder in what once was a forest.",
      "Cat Stevens was a baby then seen wondering around in a empty field by one of the engineers. He had probably lost his mother and was probably going to die if he wasn't rescued by the engineer. Cat Stevens was a small cat when he arrived. He was underweight and la Senda Verde noticed he could not walk too well, but assumed it was because of age and undernourishment. After a month, he gained weight and suddenly stopped walking. He was taken to La Paz for X-ray and noticed he had an old fracture that would not heal due to decalcification. After a couple of weeks of feeding him with cat formula, he recovered and started to walk again. Cat Stevens was alone for a few months until Cat Iris arrival, a female ocelot. They where put together and get along well.",
    |]
  | Aruma => [|
      "Aruma was the first bear to arrive at la Senda Verde and was the game-changer for them. After he arrived they knew that our mission was to shelter and rescue animals. Aruma means 'night' in Aymara language.",
      "A little boy and his dad had stumbled in the forest upon a mother bear with two cubs. The mother and one of the cubs ran away but the other cub stood there frozen. The cub was taken by the little boy and he tried to sell it in the town of Quime. The doctor in town noticed that the cub had terrible diarrhea, called the police and the bear cub was confiscated from the little boy. The doctor arrived in La Paz at 4 am, and called Animals SOS. Aruma stayed with a volunteer for 2 months while he was getting medical treatment, until he was sent to La Senda Verde in December 2007.",
    |]
  | Apthapi => [|
      "Apthapi arrived at La Senda Verde with 8-10 months of age along with a female deer of the same age. They came from a confiscation performed by the Forestry and Environment Police (POFOMA) in Palos Blancos, North of La Paz. The tapir is the largest mammal in South America. They are known to be the \'gardeners of the forest\' because they are nutrient recyclers. They are also excellent swimmers.",
    |]
  // | Ajayu => [|
  //     "Ajayu was saved by the police in Cochabamba from being brutally beaten to death by a local nearby community in 2016. He arrived in La Paz and La Senda Verde took care of his treatments. He received several surgeries, one to save his left eye and one to save his jaw from a severe infection. He stopped eating for days and had to be force fed. It was a struggle to save him, but with time he got better and is now weighing 100 kilograms.",
  //     "As a result from this brutal story was that la Senda Verde has become more involved in trying to change legislation in Bolivia to protect this species and avoid stories like this one from happening again. They are now working alongside biologist Ximena Velez-Liendo to present a law project that will be called the Ajayu law, named after this bear. Ajayu means spirit in Aymara language.",
  //   |]
  | Nonhlanhla => [|
      "Nonhlanhla is an 11 year old female spotted hyena who lives in the forests and grasslands of central Zululand, South Africa. Though spotted hyenas can be difficult to see in this area, Nonhlanhla is a particularly great breeder in her clan having cubs regularly, meaning she is seen often at the den site nursing her young. In an area where spotted hyenas are persecuted and there is evidence for their decline, her name in Zulu, appropriately means \"luck\".",
    |]
  | Isisa => [|
      "Isisa is a 10 year old female spotted hyena in a small clan who inhabit the rocky outcrops and low-lying mountains of Zululand. Isisa is the mother of the young male hyena Dlala and is often seen in the Tamboti tree lined drainage canyons in the area. She is an incredibly successful hunter and is often found to have brought down large nyala antelope by herself. ",
    |]
  | Dlala => [|
      "Dlala is a young male spotted hyena, currently about 13 months old. At this age he has recently grown large enough to leave the safety of his clan's den and is out exploring the world within their territory. Too young to breed, his current existence is more about learning and interacting with his new environment; in Zulu his name means \"play\".",
    |]
  | Ucok => [|
      "Ucok is a baby sun bear who was taken way too young from his mom, and sold into wildlife trafficking. However, Darwin Animal Doctors local partner team, the Indonesian Species Conservation Program (ISCP) rescued him from the black market. Ucok is now being rehabilitated at the BKSDA - the Indonesian wildlife authority. Ucok is understandably often quite sad, because he misses his mom. But, he regularly overcomes his sadness to be an extremely curious and inquisitive boy and he loves to test his rehab team's boundaries! He is quite mischievous, which tells us he's a smart boy!",
    |]
  | Glen => [|
      "It was a tough and trying time in Turin, Italy. A pandemic had taken over the land and the villagers were facing hardships. Unexpectedly nature was prospering, the villagers were in hiding and the flora and fauna of the region was reclaiming its land. Most unexpectedly the dragon returned, which for a long time was believed to be extinct. Glen the Dragon of Turin emerged from the Alps just North of the city. One of the last of his kind.",
      "Glen is a unique Wildcard that is named after Glen Weyl the author of Radical Markets. He was developed during the ETHTurin hackathon. The funds raised by Glen go to the conservation voted by the community. Owners of wildcards can vote using their loyalty tokens using quadratic voting to vote which conservation should receive the funds raised for that month.",
    |]
  };

// [@bs.module "./animaltx.js"]
// external buyTx: (string, string, 'a, 'b) => unit = "default";

// let useCurrentPrice = animal => {
//   open Hooks;
//   open Accounting;

//   let animalId = getId(animal);
//   useCurrentPriceWeiNew(animalId)->defaultZeroS;
// };

// TODO: you could get this from the contract, but seems like a waste
let pledgeRate = animal => {
  // TODO: get this from the blockchain / graph - bad hardcoding it here!
  switch (animal) {
  | Vitalik => ("3", "10", 0.025, 40.) // 30% per year (2.5% per month)
  | Glen => ("6", "10", 0.05, 20.)
  | Apthapi
  | Pancho => ("12", "10", 0.1, 10.)
  | Simon
  | Andy
  | Aruma
  | CatStevens
  | Nonhlanhla
  | Llajuita
  | Tarkus
  | Dlala => ("24", "10", 0.2, 5.)
  | Cubai
  | Espumita
  | Ucok
  | Isisa => ("60", "10", 0.5, 2.)
  | Verano => ("120", "10", 1., 1.)
  };
};

// let useCurrentPriceEth = animal => {
//   open Belt.Option;
//   let animalId = getId(animal);

//   Hooks.useCurrentPriceEthNew(animalId)->mapWithDefault("loading", a => a);
// };
// let useCurrentPriceUsd = animal => {
//   open Belt.Option;
//   let animalId = getId(animal);
//   Hooks.useCurrentPriceUsdNew(animalId)->mapWithDefault("loading", a => a);
// };

type launchStatus =
  | Launched
  | LaunchDate(MomentRe.Moment.t);

let nextLaunchDate = MomentRe.momentUtcDefaultFormat("2020-05-21T17:00:00");

let isLaunched: t => launchStatus =
  anAnimal =>
    switch (anAnimal) {
    | Simon
    | Andy
    | Vitalik
    | Cubai
    | Dlala
    | CatStevens
    | Verano
    | Pancho
    | Isisa
    | Apthapi
    | Nonhlanhla
    | Aruma
    | Glen
    | Espumita
    | Ucok
    | Llajuita => Launched
    | Tarkus => LaunchDate(nextLaunchDate)
    };

let hasGovernance: t => bool =
  anAnimal =>
    switch (anAnimal) {
    | Simon
    | Andy
    | Vitalik
    | Cubai
    | Dlala
    | CatStevens
    | Verano
    | Pancho
    | Isisa
    | Apthapi
    | Nonhlanhla
    | Aruma
    | Espumita
    | Ucok
    | Tarkus
    | Llajuita => false
    | Glen => true
    };
