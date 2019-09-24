let twitterIcon = [%bs.raw {|require('../../img/icons/twitter.svg')|}];
let instagramIcon = [%bs.raw {|require('../../img/icons/instagram.svg')|}];

module Footer = {
  open Components;
  open Rimble;

  [@react.component]
  let make = () =>
    <Box className=Styles.footer>
      // // NOTE: Flexbox isn't really needed, just made dealing with the padding easier
      //   <Flex>
      //     <Box className=Styles.extraInfoFooterBox>
      //       <Box className=Styles.emoticonHeader>
      //         <img
      //           className=Styles.emojiStyles
      //           src=raisingHandsEmoji
      //           alt="Emoji"
      //         />
      //       </Box>
      //       <Box className=Styles.finalNoteContent>
      //         <Heading className=Styles.whiteText>
      //           <S> "Wildcards is currently under active development." </S>
      //         </Heading>
      //         <br />
      //         <a
      //           className=Styles.linkPillBox
      //           href="https://youtu.be/ibBAlrrwjp0?t=322"
      //           target="_blank">
      //           <span className=Styles.linkPillText>
      //             <S> "Watch presentation at the EthCapeTown hacakthan." </S>
      //           </span>
      //         </a>
      //       </Box>
      //     </Box>
      //   </Flex>

        <div className=Styles.footerContainer>
          <div className=Styles.footerWrapper>
            <div className=Styles.footerText>
              <S> {js|©|js} </S>
              <a className=Styles.footerLink href="javascript:void(0);">
                <S> "Wild Cards" </S>
              </a>
              <S> ". All rights reserved." </S>
            </div>
            <div>
              <ul className=Styles.footerSocialButtons>
                <li className=Styles.footerSocialButton>
                  <a
                    className="social-buttons__link"
                    href="https://twitter.com/wildcards_world"
                    target="_blank">
                    <img
                      className=Styles.footerSocailButtonLink
                      src=twitterIcon
                    />
                  </a>
                </li>
                <li className="social-buttons__item">
                  <a
                    className="social-buttons__link"
                    href="http://instagram.com/wildcards_world"
                    target="_blank">
                    <img className="social-buttons__icon" src=instagramIcon />
                  </a>
                </li>
              </ul>
            </div>
          </div>
        </div>
      </Box>;
} /* export default () =*/;
