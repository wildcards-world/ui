let twitterIcon = [%bs.raw {|require('../../img/icons/twitter.svg')|}];
let instagramIcon = [%bs.raw {|require('../../img/icons/instagram.svg')|}];

module Footer = {
  open Components;
  open Rimble;

  [@react.component]
  let make = () =>
    <Box className=Styles.footer>
      <div className=Styles.footerContainer>
        <div className=Styles.footerWrapper>
          <div className=Styles.footerText>
            <S> {js|©|js} </S>
            <a className=Styles.footerLink href="javascript:void(0);">
              <S> "Wild Cards" </S>
            </a>
            <S> ". All rights reserved." </S>
          </div>
          <ul className=Styles.footerSocialButtons>
            <li className=Styles.footerSocialButton>
              <a
                className=Styles.footerSocailButtonLink
                href="https://twitter.com/wildcards_world"
                target="_blank">
                <img className=Styles.footerSocailButtonLink src=twitterIcon />
              </a>
            </li>
            <li className=Styles.footerSocialButton>
              <a
                className=Styles.footerSocailButtonLink
                href="http://instagram.com/wildcards_world"
                target="_blank">
                <img className="social-buttons__icon" src=instagramIcon />
              </a>
            </li>
          </ul>
        </div>
      </div>
    </Box>;
};
