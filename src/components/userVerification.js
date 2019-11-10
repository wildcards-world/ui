import ethUtil from 'ethereumjs-util'
import sigUtil from 'eth-sig-util'

export const generateSignature = (web3, from) => (message, signatureCallback) => {
  const messageHex = ethUtil.bufferToHex(new Buffer(message, 'utf8'))

  if (!from) return

  /*  web3.personal.sign not yet implemented!!!
   *  We're going to have to assemble the tx manually!
   *  This is what it would probably look like, though:
    web3.personal.sign(msg, from) function (err, result) {
      if (err) return console.error(err)
      console.log('PERSONAL SIGNED:' + result)
    })
  */

  const params = [messageHex, from]
  const method = 'personal_sign'

  web3.currentProvider.sendAsync({
    method,
    params,
    from,
  }, function (err, { result, error }) {
    if (err) return console.error(err)
    if (error) return console.error(error)

    signatureCallback(result)

    const msgParams = { data: messageHex, sig: result }
    const recovered = sigUtil.recoverPersonalSignature(msgParams)

    if (recovered.toLowerCase() === from.toLowerCase()) {
      console.log('SigUtil Successfully verified signer as ' + from)
    } else {
      console.dir(recovered)
      console.log('SigUtil Failed to verify signer when comparing ' + recovered + ' to ' + from)
      console.log('Failed, comparing %s to %s', recovered, from)
    }

    //   method = 'personal_ecRecover'
    //   const params = [messageHex, result]
    //   web3.currentProvider.sendAsync({
    //     method,
    //     params,
    //     from,
    //   }, function (err, recovered) {
    //     console.dir({ err, recovered })
    //     if (err) return console.error(err)
    //     if (error) return console.error(error)

    //     if (result === from) {
    //       console.log('Successfully verified signer as ' + from)
    //     } else {
    //       console.log('Failed to verify signer when comparing ' + result + ' to ' + from)
    //     }

    //   })
  })
}


export const submitVerification = async (ethAddress, link, twitterHandle, resultCallback) => {
  const tweetIdRegex = /[0-9]*$/g;
  // TODO: you could check if the tweet comes from the correct twitter user. This is some basic clientside validation (can quickly notify the user about this).
  const tweetId = link.match(tweetIdRegex)[0]

  let result = await fetch('http://34.65.206.129:5000/verification', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({
      ethAddress,
      twitterHandle,
      tweetId,
    })
  })
  let resultJson = await result.json()
  resultCallback(resultJson.success)
}
