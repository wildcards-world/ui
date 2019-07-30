import React from 'react'
import raisingHandsEmoji from '../../img/emoji/raising_hands.png'

export default () => <div className="text--01 vine-background" id="about">
  <div className="container-template container--small">
    <div className="text--01__box" style={{ backgroundColor: '#6bad3e', borderRadius: 0 }}>
      <div className="text--01__emoji">
        <img className="emoji   emoji--large" src={raisingHandsEmoji} alt="Emoji" />
      </div>
      <p className="text--01__content white-text">
        Wildcards is currently under active development.
              </p>
      <div className="text--01__link_box">
        <a className="pill-link  pill-link--black  pill-link--small " href="https://youtu.be/ibBAlrrwjp0?t=322" target="_blank"><span className="pill-link__text">Watch the original hackathon presentation</span><span className="pill-link__icon"><span className="icon"><svg viewBox="0 0 13 10" xmlns="http://www.w3.org/2000/svg">
          <path d="M12.823 4.164L8.954.182a.592.592 0 0 0-.854 0 .635.635 0 0 0 0 .88l2.836 2.92H.604A.614.614 0 0 0 0 4.604c0 .344.27.622.604.622h10.332L8.1 8.146a.635.635 0 0 0 0 .88.594.594 0 0 0 .854 0l3.869-3.982a.635.635 0 0 0 0-.88z" fillRule="nonzero" fill="#00396B" /></svg></span></span></a>
      </div>
    </div>
  </div>
</div>
