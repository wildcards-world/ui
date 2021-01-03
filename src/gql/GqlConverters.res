module BigInt = {
  type t = BN.t
  let parse = json =>
    switch json->Js.Json.decodeString {
    | Some(str) => BN.new_(str)
    | None =>
      // In theory graphql should never allow this to not be a correct string
      Js.log("CRITICAL - should never happen!")
      BN.newInt_(0)
    }
  let serialize = bn => bn->BN.toString->Js.Json.string
}

module BigIntStr = {
  type t = BN.t
  let parse = stringBn => BN.new_(stringBn)
  let serialize = bn => bn->BN.toString
}

module GqlTokenId = {
  type t = TokenId.t
  let parse = tokenIdJson =>
    tokenIdJson
    ->Js.Json.decodeString
    ->Option.mapWithDefault("0", a => a)
    ->TokenId.make
    ->Option.getWithDefault(TokenId.makeFromInt(0))
  let serialize = tokenId => tokenId->TokenId.toString->Js.Json.string
}
module GqlTokenIdStr = {
  type t = TokenId.t
  let parse = tokenIdJson =>
    tokenIdJson->TokenId.make->Option.getWithDefault(TokenId.makeFromInt(0))
  let serialize = tokenId => tokenId->TokenId.toString
}

// let decodePrice: Js.Json.t => Eth.t =
module Price = {
  type t = Eth.t
  let parse = price =>
    price->Js.Json.decodeString->Option.mapWithDefault("0", a => a)->Eth.makeWithDefault(0)
  let serialize = price => price->BN.toString->Js.Json.string
}

module GqlMoment = {
  type t = MomentRe.Moment.t
  let parse = moment =>
    moment
    ->Js.Json.decodeString
    ->Option.mapWithDefault(0, a => a->int_of_string) /* trusting that gql will be reliable here */
    ->MomentRe.momentWithUnix
  let serialize = moment => moment->MomentRe.Moment.toUnix->string_of_int->Js.Json.string
}
module GqlAddress = {
  // TODO: make a real address string
  type t = string
  let parse = address => address->Js.Json.decodeString->Option.mapWithDefault("0x0", a => a)

  let serialize = address => address->Js.Json.string
}
