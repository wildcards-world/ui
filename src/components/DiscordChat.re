[@react.component]
let make = () => {
  let (showChat, setShowChat) = React.useState(_ => false);
  <div
    className=Css.(
      style([
        position(`fixed),
        bottom(px(10)),
        right(px(10)),
        zIndex(1000),
      ])
    )>
    {showChat
       ? <div>
           <iframe
             src="https://discordapp.com/widget?id=723502058426073108&theme=dark"
             width="350"
             height="500"
             sandbox="allow-popups allow-popups-to-escape-sandbox allow-same-origin allow-scripts"
             className=Css.(style([borderWidth(px(0))]))
           />
         </div>
       //Add close button above here
       : <img
           src="/img/icons/discord.svg"
           onClick={_ => setShowChat(_ => true)}
         />}
  </div>;
};
