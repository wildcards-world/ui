open Globals;

type teamMember = {
  name: string,
  img: string,
  title: string,
  linkedIn: string,
  twitterHandle: option(string),
};

let cardStyle =
  Css.(style([height(`percent(100.)), backgroundColor(`hex("73C8D7"))]));

[@react.component]
let make = () => {
  // Can maybe move this to the db, although overkill at this point
  let teamMembers: array(teamMember) = [|
    {
      name: "Denham Preen",
      img: "https://dd2wadt5nc0o7.cloudfront.net/team/denham.jpg",
      title: "co-founder",
      linkedIn: "",
      twitterHandle: Some(""),
    },
    {
      name: "Jason Smythe",
      img: "https://dd2wadt5nc0o7.cloudfront.net/team/jason.jpg",
      title: "co-founder",
      linkedIn: "",
      twitterHandle: Some(""),
    },
  |];
  <Rimble.Flex
    flexWrap="wrap"
    justifyContent="space-around"
    alignItems="stretch"
    // className=blueBackground
    pb=50
    px=50>
    {React.array(
       teamMembers->Array.mapWithIndex((i, member) =>
         <Rimble.Box
           key={i->string_of_int} mt=20 mb=20 width=[|0.45, 0.45, 0.18|]>
           <Rimble.Card className=cardStyle>
             <img
               className=Css.(
                 style([marginLeft(`percent(10.)), width(`percent(80.))])
               )
               src={member.img}
               alt={member.name}
             />
             <p> member.name->React.string </p>
             <p> member.title->React.string </p>
             <img
               className=Css.(style([height(px(20)), margin(`auto)]))
               src="/img/socials/linkedin.svg"
               alt={member.name}
             />
             {switch (member.twitterHandle) {
              | Some(handle) =>
                <img
                  className=Css.(style([height(px(20)), margin(`auto)]))
                  src="/img/socials/twitter.svg"
                  alt=handle
                />
              | None => React.null
              }}
           </Rimble.Card>
         </Rimble.Box>
       ),
     )}
  </Rimble.Flex>;
};
