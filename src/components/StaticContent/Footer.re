module Footer = {
  open Components;
  open Rimble;

  [@react.component]
  let make = () =>
    <Box className=Styles.footer>
      <div className=Styles.footerContainer>
        <div className=Styles.footerWrapper>
          <div className=Styles.footerText>
            <div>
              <S> {js|©|js} </S>
              <a className=Styles.footerLink href="https://wildcards.world">
                <S> "Wild Cards" </S>
              </a>
              <S> ". All rights reserved." </S>
            </div>
          </div>
          <div className=Styles.footerText>
            <a className=Styles.footerLink href="/privacy_policy.html">
              <S> "Privacy Policy" </S>
            </a>
          </div>
          <div className=Styles.footerText>
            <a className=Styles.footerLink href="/terms_and_conditions.html">
              <S> "Terms and Conditions" </S>
            </a>
          </div>
          // <a className="footer-02__link" href="/privacy_policy.html">Privacy Policy</a> &ensp; <a className="footer-02__link" href="/terms_and_conditions.html">Terms and Conditions</a>
          <ul className=Styles.footerSocialButtons>
            <li className=Styles.footerSocialButton>
              <SocialButtons
                url="https://twitter.com/wildcards_world"
                bgColor="transparent"
                fgColor="#aaa"
                className=Styles.footerSocailButtonLink
              />
            </li>
            <li className=Styles.footerSocialButton>
              <SocialButtons
                className=Styles.footerSocailButtonLink
                url="https://www.facebook.com/wildcardscrypto"
                bgColor="transparent"
                fgColor="#aaa"
              />
            </li>
          </ul>
        </div>
      </div>
    </Box>;
};
