import React from 'react'
import twitterIcon from '../../img/icons/twitter.svg'
import instagramIcon from '../../img/icons/instagram.svg'

export default () => <div className="footer-02">
  <div className="container-template">
    <div className="footer-02__wrapper">
      <div className="footer-02__text">
        ©
                &nbsp;<a className="footer-02__link" href="javascript:void(0);">Wild Cards</a>. All rights reserved.
              </div>
      <div className="social-buttons">
        <ul className="social-buttons__list">
          {/*
              <li class="social-buttons__item"><a class="social-buttons__link"
                href="https://www.facebook.com/unicornplatform/" target="_blank"><img class="social-buttons__icon"
                src="../img/icons/facebook.svg" /></a></li>
              */}
          <li className="social-buttons__item">
            <a className="social-buttons__link" href="https://twitter.com/wildcards_world" target="_blank"><img className="social-buttons__icon" src={twitterIcon} /></a>
          </li>
          <li className="social-buttons__item">
            <a className="social-buttons__link" href="http://instagram.com/wildcards_world" target="_blank"><img className="social-buttons__icon" src={instagramIcon} /></a>
          </li>
        </ul>
      </div>
    </div>
  </div>
</div>
