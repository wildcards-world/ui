let transparency_link = "https://blog.wildcards.world/where-is-the-money-going/";
let privacy_policy_link = "https://wildcards.world/privacy_policy.html";
let terms_and_conditions_link = "https://wildcards.world/terms_and_conditions.html";

open Components;
open Rimble;

[@react.component]
let make = () =>
  <Box className=Styles.footer>
    <div className=Styles.footerWrapper>
      <div className=Styles.footerText>
        <div>
          <S> {js|©|js} </S>
          <a
            className=Styles.footerLink
            target="_blank"
            rel="noopener noreferrer"
            href="https://wildcards.world">
            <S> "Wildcards" </S>
          </a>
        </div>
      </div>
      <div className=Styles.footerText>
        <a
          className=Styles.footerLink
          target="_blank"
          rel="noopener noreferrer"
          href=transparency_link>
          <S> "Transparency" </S>
        </a>
      </div>
      <div className=Styles.footerText>
        <a
          className=Styles.footerLink
          target="_blank"
          rel="noopener noreferrer"
          href=privacy_policy_link>
          <S> "Privacy Policy" </S>
        </a>
      </div>
      <div className=Styles.footerText>
        <a
          className=Styles.footerLink
          target="_blank"
          rel="noopener noreferrer"
          href=terms_and_conditions_link>
          <S> "Terms and Conditions" </S>
        </a>
      </div>
      <ul className=Styles.footerSocialButtons>
        <li className=Styles.footerSocialButton>
          <SocialButtons
            url="https://twitter.com/wildcards_world"
            bgColor="transparent"
            fgColor="#aaa"
            target="_blank"
            rel="noopener noreferrer"
            network="twitter"
            className=Styles.footerSocailButtonLink
          />
        </li>
        <li className=Styles.footerSocialButton>
          <SocialButtons
            className=Styles.footerSocailButtonLink
            url="https://www.facebook.com/wildcardscrypto"
            bgColor="transparent"
            target="_blank"
            rel="noopener noreferrer"
            network="facebook"
            fgColor="#aaa"
          />
        </li>
        <li className=Styles.footerSocialButton>
          <SocialButtons
            className=Styles.footerSocailButtonLink
            url="https://t.me/wildcardsworld"
            bgColor="transparent"
            target="_blank"
            rel="noopener noreferrer"
            network="telegram"
            fgColor="#aaa"
          />
        </li>
      </ul>
    </div>
  </Box>;
