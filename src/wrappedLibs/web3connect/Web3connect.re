module Core = {
  [@bs.deriving {abstract: light}]
  type web3connectCore = {
    on: (. string, unit => unit) => unit,
    toggleModal: (. unit) => unit,
  };

  let new_: (. Js.Json.t, string) => web3connectCore = [%bs.raw
    {|
    (providerOptions, network) => {
      const web3connect = require('@wildcards/web3connect').default;
      const core = new web3connect.Core({
        network: network,
        // lightboxOpacity: settings.lightboxOpacity,
        providerOptions: providerOptions
      })
      return core
    }
  |}
  ];
};

module CustomButton = {
  [@react.component]
  let make =
      (
        ~children: React.element,
        ~providerOptions: Js.Json.t=?,
        ~onConnect: 'provider => unit,
        ~onClose: 'close => unit=Js.log,
        ~onError: 'error => unit=Js.log,
        ~onDisconnect: 'disconnect' => unit=Js.log,
        ~network: string="mainnet",
      ) => {
    open Core;
    let core: Core.web3connectCore = Core.new_(. providerOptions, network);
    core->on(. "connect", onConnect);
    core->on(. "disconnect", onDisconnect);
    core->on(. "error", onError);
    core->on(. "close", onClose);

    <Rimble.Button
      onClick={event => {
        Js.log(event);
        Js.log(" I was clicked");
        core->toggleModal(.);
      }}>
      children
    </Rimble.Button>;
  };
};
