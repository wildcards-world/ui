let raisingHandsEmoji = [%bs.raw
  {|require('../../img/emoji/raising_hands.png')|}
];

module FinalNote = {
  open Components;
  open Rimble;

  [@react.component]
  let make = () =>
    <Box className=Styles.infoBackground>
      // NOTE: Flexbox isn't really needed, just made dealing with the padding easier

        <Flex>
          <Box className=Styles.extraInfoFooterBox>
            <Box className=Styles.emoticonHeader>
              <img
                className=Styles.emojiStyles
                src=raisingHandsEmoji
                alt="Emoji"
              />
            </Box>
            <Box className=Styles.finalNoteContent>
              <Heading className=Styles.whiteText>
                <S> "Wildcards is currently under active development." </S>
              </Heading>
              <br />
              <a
                className=Styles.linkPillBox
                href="https://youtu.be/ibBAlrrwjp0?t=322"
                target="_blank"
                rel="noopener noreferrer">
                <span className=Styles.linkPillText>
                  <S> "Watch presentation at the EthCapeTown hackathon." </S>
                </span>
              </a>
            </Box>
          </Box>
        </Flex>
      </Box>;
};
