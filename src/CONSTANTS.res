let cdnBase = "https://dd2wadt5nc0o7.cloudfront.net"

let nullEthAddress = "0x0000000000000000000000000000000000000000"

// BN constants
@ocaml.doc(`This is equivalent to the number of seconds in a year 86400×365 times 1000000000000 which is the level of precision needed in our smart contracts.
BN.new_("1000000000000")->BN.mul( BN.new_("31536000"))`)
let secondsIn365DaysPrecisionScaled = BN.new_("31536000000000000000")

@ocaml.doc("A month of 30 days has 2592000 seconds in it")
let secondsInAMonth = 2592000

@ocaml.doc("A month of 30 days has 2592000 seconds in it")
let secondsInAMonthBn = BN.new_("2592000")

let zeroBn = BN.new_("0")
