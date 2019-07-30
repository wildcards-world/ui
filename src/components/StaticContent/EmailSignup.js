
import React from 'react';
import topCornerBlue from "../../img/icons/corner-top--blue.svg"
import smirkingEmoji from '../../img/emoji/smirking.png'
import laughingEmoji from '../../img/emoji/laughing.png'
import coldSweat from '../../img/emoji/cold_sweat.png'
// import raisingHandsEmoji from '../img/emoji/raising_hands.png'
// import twitterIcon from '../img/icons/twitter.svg'
// import instagramIcon from '../img/icons/instagram.svg'

let EmailSignup = () => <div className="cta_form-03" style={{ backgroundColor: '#73c7d7' }} id="signup">
  <div className="container-template container--mid">
    <div className="cta_form-03__wrapper">
      <div className="cta_form-03__heading_box">
        <h2 className="cta_form-03__heading">Want to stay in the loop?</h2>
        <span className="cta_form-03__text">Sign up for our newsletter</span>
      </div>
      <div className="cta_form-03__form_box">
        {/* START OF: mailchimp form =====
        // If you need help with setting up the MailChimp form you can check the handy guides: https://unicornplatform.com/blog/category/guides-%F0%9F%8D%93/
        // If you need an assistance of a tech guy, simply ask in the chat on website or join Unicorn Platform Slack channel.
        */}
        <form className="form js-subscribe-mailchimp-form" action="https://world.us20.list-manage.com/subscribe/post?u=11056167825e6c3d276b0a362&id=66d23199dd" method="post">
          <div className="form__inputs">
            <div className="form__input">
              <input className="text-input js-email-input" type="email" name="EMAIL" placeholder="E-mail" />
              <div className="form__messages">
                <div className="message message--engaging js-message js-engaging-message" data-index="email-form-15">
                  <div className="message__box">
                    <button className="message__close js-close-message" type="button">
                      <img className="message__close_icon" src={topCornerBlue} />
                    </button>
                    <div className="message__body">
                      <div className="message__in">
                        <div className="message__bubble">
                          <div className="message__bubble_text">
                            Exciting product updates.
                                  </div>
                        </div>
                        <div className="message__bubble">
                          <div className="message__bubble_text">
                            The hottest stories from the blog.
                                  </div>
                        </div>
                        <div className="message__bubble">
                          <div className="message__bubble_text">
                            Tell your friends about us!<img className="emoji  " src={smirkingEmoji} alt="Emoji" />
                          </div>
                        </div>
                      </div>
                      <div className="message__out">
                        <div className="message__out_box">
                          <div className="message__bubble">
                            <div className="message__bubble_text message__bubble_text--out js-reaction-text">
                              {/*updated via JS, no need to enter any text here*/}
                            </div>
                          </div>
                        </div>
                      </div>
                    </div>
                    <div className="message__reply_box">
                      <div className="message__reply_word">Reply</div>
                      <div className="message__options">
                        <div className="message__option">
                          <button className="button js-react-on-message button--accent-bg " type="submit">
                            <span className="button__text">I will join</span>
                          </button>
                        </div>
                        <div className="message__option">
                          <button className="button js-react-on-message button--accent-bg " type="submit">
                            <span className="button__text">Sounds sexy!</span>
                          </button>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
                <div className="message message--success js-message js-success-message">
                  <div className="message__box">
                    <button className="message__close js-close-message" type="button">
                      <img className="message__close_icon" src={topCornerBlue} />
                    </button>
                    <div className="message__body">
                      <div className="message__in">
                        <div className="message__bubble">
                          <div className="message__bubble_text">
                            <b>Success!</b> Welcome aboard!<img className="emoji" src={laughingEmoji} alt="Emoji" />We will be touch in shortly!
                                  </div>
                        </div>
                        <div className="message__bubble">
                          <div className="message__bubble_text">
                            BTW, we post our happy faces on Instagram and
                            Twitter. Welcome:
                                  </div>
                        </div>
                        <div className="message__bubble">
                          <div className="message__bubble_text">
                            <a className="message__bubble_link" href="https://www.instagram.com/wildcards_world/">https://www.instagram.com/wildcards_world/</a>
                          </div>
                        </div>
                        <div className="message__bubble">
                          <div className="message__bubble_text">
                            <a className="message__bubble_link" href="https://twitter.com/wildcards_world">https://twitter.com/wildcards_world</a>
                          </div>
                        </div>
                      </div>
                      <div className="message__bubble">
                        <div className="message__bubble_text">
                          Which endangered species do you feel requires the
                          most protection?
                                </div>
                      </div>
                    </div>
                    <div className="message__reply_box">
                      <div className="message__reply_word">Reply</div>
                      <div className="message__options">
                        <div className="message__option">
                          <button className="button js-react-on-message button--emerald-bg " type="submit">
                            <span className="button__text">Flying Pig</span>
                          </button>
                        </div>
                        <div className="message__option">
                          <button className="button js-react-on-message button--emerald-bg " type="submit">
                            <span className="button__text">Three Toed Sloth</span>
                          </button>
                        </div>
                        <div className="message__option">
                          <button className="button js-react-on-message button--emerald-bg " type="submit">
                            <span className="button__text">Black Rhino</span>
                          </button>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
                <div className="message message--error js-message js-error-message">
                  <div className="message__box">
                    <button className="message__close js-close-message" type="button">
                      <img className="message__close_icon" src={topCornerBlue} />
                    </button>
                    <div className="message__body">
                      <div className="message__in">
                        <div className="message__bubble">
                          <div className="message__bubble_text">
                            <b>Error.</b> Your form has not been
                                    submitted<img className="emoji  " src={coldSweat} alt="Emoji" />
                          </div>
                        </div>
                        <div className="message__bubble">
                          <div className="message__bubble_text">
                            This is what the server says:
                                    <div className="message__bubble_error js-error-message-text">
                              There must be an @ at the beginning.
                                    </div>
                          </div>
                        </div>
                      </div>
                      <div className="message__out">
                        <div className="message__out_box">
                          <div className="message__bubble">
                            <div className="message__bubble_text message__bubble_text--out js-reaction-text">
                              I will retry
                                    </div>
                          </div>
                        </div>
                      </div>
                    </div>
                    <div className="message__reply_box">
                      <div className="message__reply_word">Reply</div>
                      <div className="message__options">
                        <div className="message__option">
                          <button className="button js-react-on-message button--ruby-bg " type="submit">
                            <span className="button__text">Uh oh!</span>
                          </button>
                        </div>
                        <div className="message__option">
                          <button className="button js-react-on-message button--ruby-bg " type="submit">
                            <span className="button__text">I will retry</span>
                          </button>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
            <div className="form__button">
              <button className="button js-submit-button button--alt-accent-bg " type="submit" style={{ backgroundColor: '#6bad3e' }}>
                <span className="button__text">Subscribe</span>
                <div className="spinner" />
                <div className="button__submit_success">
                  <div className="button__success_circle" />
                  <div>
                    <svg className="button__success_tick" width={13} height={13} viewBox="0 0 13 13" xmlns="http://www.w3.org/2000/svg">
                      <path className="button__success_tick_path" stroke="#FFF" d="M0 8l5.119 3.873L11.709.381" fill="none" fillRule="evenodd" strokeLinecap="square" />
                    </svg>
                  </div>
                </div>
                <span className="icon"><svg viewBox="0 0 13 10" xmlns="http://www.w3.org/2000/svg">
                  <path d="M12.823 4.164L8.954.182a.592.592 0 0 0-.854 0 .635.635 0 0 0 0 .88l2.836 2.92H.604A.614.614 0 0 0 0 4.604c0 .344.27.622.604.622h10.332L8.1 8.146a.635.635 0 0 0 0 .88.594.594 0 0 0 .854 0l3.869-3.982a.635.635 0 0 0 0-.88z" fillRule="nonzero" fill="#00396B" /></svg></span>
              </button>
            </div>
          </div>
        </form>
        {/* ===== END OF: mailchimp form*/}
      </div>
    </div>
  </div>
</div>
{/* ===== END OF: Email form 03*/ }

export default EmailSignup;
