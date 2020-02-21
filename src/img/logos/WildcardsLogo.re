[@react.component]
let make = (~maxWidth: string) => {
  let style = ReactDOMRe.Style.make(~width="100%", ~maxWidth, ());
  <svg style version="1.1" id="Layer_1" x="0" y="0" viewBox="0 0 566.9 158.7">
    <path
      d="M214.5 64.4C227.2 43.2 239.9 22 253 0c5.6 8.6 10.8 16.4 16.1 24.2 1.8-2.2 3.2-4 5.1-6.5 10 16.6 19.7 32.8 29.5 49-2.4 0-4.7 0-7.1-.1-5.8-.2-11.3 0-16.3 3.2-2.8 1.8-5.5 3.7-8.5 5.5-3.2-2.1-6.1-4.1-9.2-6.3-5.9-4-12.5-6.7-19.5-7.1-8.3-.5-16.8-1.1-24.9 2.1-1.2.7-2.5.4-3.7.4zm26.5-35c2.7-1.3 5.5-2.7 8.2-4 2.2 2.5 4.2 4.7 6.5 7.3 1.2-2.8 2.2-5.1 3.2-7.3 2.6.8 4.8 1.7 7.1 2.4-4-6.9-8-13.3-13-19.8-4.6 7.6-8.6 14.3-12.7 20.9-.2.4-.5.7-.7 1.1.5-.3.9-.4 1.4-.6zm30.8 6.1c.4 2.1.6 3.9 1.1 7.1 1.5-2.6 2.5-4 3.2-5.3 2.9 1.4 4.8 2.4 6.8 3.3.4-.2.6-.6.9-.8-3.2-5.2-6.3-10.4-9.9-16.2-2.5 3.9-4.7 7.2-7.1 10.9 1.9.4 3.2.7 5 1z"
      fill="#333"
    />
    <path
      d="M214.5 64.4c1.2 0 2.5.2 3.5-.2 8.1-3.2 16.6-2.6 24.9-2.1 7.1.4 13.6 3.2 19.5 7.1 3.1 2 6.1 4.1 9.2 6.3 2.9-1.9 5.6-3.8 8.5-5.5 5.1-3.2 10.7-3.4 16.3-3.2 2.4.1 4.7.1 7.1.1 2.1 3.1 4.1 6.1 6.7 9.8C293 87 276.1 97 259 107.4c-17.3-10.3-34.3-20.4-52-30.8 2.6-4.3 5.1-8.1 7.5-12.2z"
      fill="#6cad3f"
    />
    <path
      d="M258.2 116.5c16.3-9.7 32.4-19.1 49-28.9-1.4 4.7-40.8 61.4-49.4 71.2-16.6-23.4-32.9-46.7-49.4-70 .2-.4.5-.6.7-.9 8.5 4 16.2 9.4 24.3 14.2 8.4 4.5 16.4 9.3 24.8 14.4z"
      fill="#6dae42"
    />
    <path
      d="M66.2 57.4h8.5c-5.3 16.2-10.6 32-15.8 47.9h-9.6C45.5 94.2 41.7 83 37.6 70.6c-5.1 11.9-8 23.6-12.3 34.8h-9.5C10.6 89.7 5.4 73.8 0 57.5h9.3c3.8 11.6 7.6 23.3 11.9 36 4.3-12.7 8.3-24.3 12.3-36.1h8.3C45.7 69 49.7 80.8 54 93.8c4.3-13 8.2-24.6 12.2-36.4zM461.6 89.9c3.1 4.5 6 8.9 8.9 13-.2 3.1-2.4 2.8-4.3 2.8-2.4 0-4.6.4-6.1-2.2-2.5-4-5.3-7.8-7.9-11.4h-11.8v13.3h-10V57.5c1.5-.1 3.1-.5 4.6-.5 5.5 0 10.9-.1 16.5 0 1.8 0 3.5.1 5.3.5 7.3 1.2 13.2 6 14.2 12.3 1.8 10-1.5 15.8-7.5 19.4-.7 0-1.2.4-1.9.7zm-21.3-6.3h7.3c2 0 3.9 0 5.9-.4 5.1-.7 8.2-4.4 8-9.2-.1-4.5-3.2-8-8.1-8.4-4.2-.4-8.5-.1-13-.1-.1 5.9-.1 11.6-.1 18.1zM477.9 104.6v-46l1.2-1.2c8-.1 16-.5 24-.1 13.3.6 22.8 10.9 22.8 24.2s-10 24-23.4 24.3c-7.8.2-15.6-.1-23.4-.1-.5-.5-.8-.9-1.2-1.1zm9.8-7.2c4.8 0 9.2.2 13.4 0 8.8-.6 14.7-7.1 14.8-15.9 0-9-5.6-15.2-14.6-15.9-4.5-.4-8.8 0-13.5 0-.1 10.5-.1 20.8-.1 31.8zM141.1 105.5c-.1-1.4-.4-2.1-.4-2.8V57.6c1.4-.2 2.7-.5 4-.5 6.6 0 13.3-.1 19.9.2 16.1.8 26.8 13.6 22.6 31-1.9 7.8-7.3 12.9-14.8 15.3-2.9.9-6.1 1.7-9.2 1.8-7.1.3-14.4.1-22.1.1zm9-8c5.3 0 10.1.4 14.9-.1 7.8-.7 13.6-6.8 14.1-14.8.4-8.1-3.2-15.2-12-17-5.4-1.1-11-.9-16.9-1.3-.1 11.5-.1 22-.1 33.2zM383.4 105.4h-9.6c.7-2.1 1.2-3.8 1.9-5.3 5.6-12.6 11.4-25.3 17-38 1.4-3.1 3.1-5.1 6.7-5.2 3.9-.1 5.9 1.7 7.4 5.1 5.5 12.5 11.3 24.9 16.8 37.4 2.4 5.3 1.8 6.3-3.9 6.4h-3.6c-1.6-3.5-3.1-7-4.7-10.6h-23.5c-1.4 3.2-2.8 6.5-4.5 10.2zM408 86.5c-2.8-6.5-5.4-12.5-8.2-19.1-2.8 6.7-5.4 12.7-8.1 19.1H408zM527.2 100.8c1.4-3 2.5-5.2 3.4-7.1 4.7 1.5 8.8 3.3 13.2 4.1 2.9.6 6.2 0 9.3-.5 2.2-.4 4.2-2.1 3.8-4.5a6.3 6.3 0 00-3.1-4.5c-2.4-1.3-5.2-1.8-7.8-2.6-3.3-1.1-6.8-1.8-10-3.2-9.3-4.4-10.2-16.5-1.9-22.5 7.8-5.5 21.9-5.1 30.6 1.2-.9 2.2-1.8 4.6-2.7 7-2.8-.9-5.3-2-7.9-2.6-2.5-.6-4.9-.9-7.5-.9-4.6.1-7.8 2.4-8.2 5.3-.4 2.7 2 5 6.6 6.4 4.1 1.2 8.3 2.1 12.3 3.5 6.3 2.1 9.8 6.5 9.6 11.9-.1 6-3.1 10.7-8.6 12.9-10.7 3.9-20.9 2.3-31.1-3.9zM366.4 92.1c1.9 2 3.6 3.9 5.3 5.7-4.8 7.7-16 11-26.7 8-10.5-2.8-17.9-11.7-18.5-22.4-.7-11.3 5.4-20.9 15.5-25.3 9.2-3.9 24.6-2.1 30.1 6.8-1.9 1.8-3.8 3.5-6 5.7-4.2-4-9.1-6.3-14.8-5.7-4.2.5-8 2-10.8 5.2-5.4 6-5.2 16.8.4 22.7 6.1 6.6 15.4 6.6 25.5-.7zM135.3 97.4v7.3c-4 1.2-25.9 1.4-34 .5V57.5c1.3-.1 2.6-.4 4-.5 1.3-.1 2.7 0 4.8 0v40.5c8.4-.1 16.5-.1 25.2-.1zM82.3 57.2h8.3v47.9h-8.3V57.2z"
      fill="#333"
    />
    <path
      d="M479.1 57.2l-1.2 1.2c.4-.3.9-.7 1.2-1.2zM477.9 104.6c.4.4.7.7 1.2.9-.5-.3-.8-.7-1.2-.9z"
      fill="#252525"
    />
    <path
      d="M240.4 28.9c4-6.6 8-13.3 12.7-20.9 5.1 6.5 9.1 12.9 12.9 19.7-2.2-.7-4.5-1.5-7.1-2.4-1.1 2.4-2 4.6-3.2 7.3-2.4-2.6-4.3-4.8-6.5-7.3-2.8 1.3-5.5 2.7-8.2 4-.3 0-.4-.3-.6-.4z"
      fill="#f1f1f1"
    />
    <path
      d="M271.8 35.5c-1.8-.4-3.1-.6-5.1-.9 2.4-3.7 4.6-7.1 7.1-10.9 3.5 5.8 6.7 11 9.9 16.2-.4.2-.6.6-.9.8-2-.9-4-1.9-6.8-3.3-.7 1.3-1.6 2.7-3.2 5.3-.4-3.3-.8-5.1-1-7.2z"
      fill="#f1f1f1"
    />
    <path
      d="M241 29.4c-.4.1-.8.4-1.2.5.2-.4.5-.7.7-1.1.1.2.2.5.5.6z"
      fill="#f4f4f4"
    />
  </svg>;
};