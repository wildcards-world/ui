[@react.component]
let make = () => {
  let topContributorsOpt = QlHooks.useLoadTopContributorsData();

  <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
    <div> "hello"->React.string </div>
    <div>
      {switch (topContributorsOpt) {
       | Some(topContributors) =>
         ReasonReact.array(
           Array.mapi(
             (index, (contributor, amount)) => {
               <div>
                 <div> {(index + 1)->string_of_int->React.string} </div>
                 <div> contributor->React.string </div>
                 <div> {(amount ++ " ETH")->React.string} </div>
               </div>
             },
             topContributors,
           ),
         )
       | None => React.null
       }}
    </div>
  </Rimble.Flex>;
};
