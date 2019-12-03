open Belt.Option;

[@bs.module "./userVerification"]
external generateSignature:
  (. Web3.t, string) => (. string, string => string) => unit =
  "generateSignature";

[@bs.module "./userVerification"]
external submitVerification: (. string, string, string, bool => unit) => unit =
  "submitVerification";

type verificationState =
  | InputTwitterHandle(string)
  | InputTweetLink(string, string, string)
  | SumbittingVerification(string, string, string)
  | WasVerificationSuccessful(bool);

module Input = {
  [@react.component]
  let make = (~setModalOpen) => {
    let (copyText, setCopyText) = React.useState(() => "Copy to Clipboard");
    let (appState, setAppState) =
      React.useState(() => InputTwitterHandle(""));
    let web3 = Hooks.useWeb3();
    let currentUser = Hooks.useCurrentUser()->mapWithDefault("0x", a => a);
    let currentUserLower = currentUser->Js.String.toLowerCase;
    let userContext = UserProvider.useUserInfoContext();
    let genSignature = generateSignature(. web3, currentUser);

    switch (appState) {
    | InputTwitterHandle(twitterHandle) =>
      <Rimble.Box p=4 mb=3>
        <Rimble.HeadingS> "Verify your identity publically" </Rimble.HeadingS>
        <Rimble.TextS> "Please enter your Twitter handle:" </Rimble.TextS>
        <p>
          <small>
            {React.string(
               "Note: once you publish this proof of identity on the internet you cannot take it back.",
             )}
          </small>
        </p>
        {React.string("@")}
        <Rimble.Input
          _type="string"
          placeholder="twitter_handle"
          onChange={event => {
            let value =
              ReactEvent.Form.target(event)##value->getWithDefault("");

            setAppState(_ => InputTwitterHandle(value));
          }}
          value=twitterHandle
        />
        <br />
        <Rimble.Button
          m=3
          onClick={_ => {
            let twitterHandleLowwerCase =
              Js.String.toLowerCase(twitterHandle);
            genSignature(.
              {j|@$twitterHandleLowwerCase is my twitter handle on @wildcards_world.|j},
              result => {
              setAppState(_ => InputTweetLink(twitterHandle, result, ""));
              result;
            });
          }}>
          {React.string("Generate Proof")}
        </Rimble.Button>
      </Rimble.Box>
    | InputTweetLink(twitterHandle, signature, tweetLink) =>
      let tweetMessage = {j|I am validating my identity on https://wildcard.world and contributing to wildlife conservation. @wildcards_world $signature|j};
      <Rimble.Box p=4 mb=3>
        <Rimble.HeadingS> "Finalize verification" </Rimble.HeadingS>
        <Rimble.TextS>
          "Post the following into a tweet and paste the link to that tweet below."
        </Rimble.TextS>
        <Rimble.Text>
          <small>
            {React.string(
               "You can change the text as long as you don't change the hash value (the value starting with \"0x\")",
             )}
          </small>
        </Rimble.Text>
        <Rimble.Button
          className=Styles.copyButton
          onClick={_ => {
            setCopyText(_ => "Copied");
            CopyToClipboard.copy(tweetMessage);
          }}>
          {React.string(copyText)}
        </Rimble.Button>
        <Rimble.Card>
          <Rimble.Text className=Styles.wrapText>
            {React.string(tweetMessage)}
          </Rimble.Text>
        </Rimble.Card>
        // <textarea
        //   onChange={event => {
        //     let value =
        //       ReactEvent.Form.target(event)##value->getWithDefault("");
        //     Js.log(value);
        //   }}>
        //   {React.string(tweetMessage)}
        // </textarea> // TODO: maybe it will be better to use a textarea here
        // //                   - the user can then edit the text, and we can validate that they didn't
        // //                     mess up the hash or that it isn't too long for a tweet.
        // //                     Added bonus, all the word wrap settings in textarea just work!
        <br />
        <Rimble.Text>
          {React.string("Once the tweet is posted paste the link:")}
        </Rimble.Text>
        <Rimble.Input
          className=Styles.fiftyPercentWidth
          _type="string"
          placeholder={j|https://twitter.com/$twitterHandle/status/1234567890123456789|j}
          onChange={event => {
            let value =
              ReactEvent.Form.target(event)##value->getWithDefault("");

            setAppState(_ => InputTweetLink(twitterHandle, signature, value));
          }}
          value=tweetLink
        />
        <br />
        <Rimble.Button
          m=3
          onClick={_ =>
            submitVerification(.
              currentUser,
              tweetLink,
              twitterHandle,
              wasSuccesful => {
                if (wasSuccesful) {
                  userContext->UserProvider.update(currentUserLower);
                } else {
                  ();
                };

                setAppState(_ => WasVerificationSuccessful(wasSuccesful));
              },
            )
          }>
          {React.string("Submit Verification")}
        </Rimble.Button>
      </Rimble.Box>;
    | SumbittingVerification(_, _, _) =>
      <Rimble.Box p=1>
        <Rimble.Heading>
          {React.string("Sumbitting the verification")}
          <WildcardsLoader />
        </Rimble.Heading>
        <Rimble.TextS>
          "We are verifying and storing your crypographic proof of identity in our system."
        </Rimble.TextS>
        <Rimble.Loader className=Styles.centerItems color=`green size="80px" />
      </Rimble.Box>
    | WasVerificationSuccessful(_success) =>
      // TODO: handle the case when success==false. (show a message to help the user)
      <Rimble.Box p=1>
        <Rimble.Heading>
          {React.string("Your identity has been verified!")}
        </Rimble.Heading>
        <Rimble.Button onClick={_ => setModalOpen(_ => false)}>
          {React.string("close")}
        </Rimble.Button>
      </Rimble.Box>
    };
  };
};

[@react.component]
let make = () => {
  let (isModalOpen, setModalOpen) = React.useState(() => false);

  let onUnlockMetamaskAndOpenModal = () => {
    setModalOpen(_ => true);
  };
  let onOpenModal = event => {
    ReactEvent.Form.preventDefault(event);
    ReactEvent.Form.stopPropagation(event);
    setModalOpen(_ => true);
  };

  <React.Fragment>
    <Rimble.Box p=1>
      <Rimble.Button onClick=onOpenModal>
        {React.string("Verify")}
      </Rimble.Button>
    </Rimble.Box>
    <Rimble.Modal isOpen=isModalOpen>
      <Rimble.Card width={Rimble.AnyStr("70%")} p=0>
        <Rimble.Button.Text
          icononly=true
          icon="Close"
          color="moon-gray"
          position="absolute"
          top=0
          right=0
          m=3
          onClick={_ => setModalOpen(_ => false)}
        />
        <Input setModalOpen />
      </Rimble.Card>
    </Rimble.Modal>
  </React.Fragment>;
};
