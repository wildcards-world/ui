import Document, { Head, Main, NextScript } from "next/document";
import { extractCritical } from "emotion-server";
import { ServerStyleSheet } from "styled-components";

// This file is a mix between : https://github.com/zeit/next.js/blob/master/examples/with-styled-components/pages/_document.js
//                  and       : https://github.com/zeit/next.js/blob/master/examples/with-emotion/pages/_document.js
//   I am a bit concerned that the performance of this isn't great because it has to render the page twice to get all the styles. But not really an issue for now.
export default class MyDocument extends Document {
  static async getInitialProps(ctx) {
    const page = ctx.renderPage();
    const styles = extractCritical(page.html);

    const sheet = new ServerStyleSheet();
    const originalRenderPage = ctx.renderPage;

    try {
      ctx.renderPage = () =>
        originalRenderPage({
          enhanceApp: (App) => (props) =>
            sheet.collectStyles(<App {...props} />),
        });

      const initialProps = await Document.getInitialProps(ctx);
      return {
        ...page,
        ...styles,
        ...initialProps,
        styles: (
          <>
            {initialProps.styles}
            {sheet.getStyleElement()}
          </>
        ),
      };
    } finally {
      sheet.seal();
    }
  }

  render() {
    return (
      <html>
        <Head>
          <style
            data-emotion-css={this.props.ids.join(" ")}
            dangerouslySetInnerHTML={{ __html: this.props.css }}
          />
          <meta name="fortmatic-site-verification" content="3sfoP9ttXolKkEqR" />
          <link href="./assets/css/global.css" rel="stylesheet" />
          <link href="./assets/css/carousel-style.css" rel="stylesheet" />

          {/* Global site tag (gtag.js) - Google Analytics  */}
          <script
            async
            src="https://www.googletagmanager.com/gtag/js?id=UA-139348292-1"
          ></script>
          <script
            type="text/javascript"
            src="./assets/js/custom-gtags.js"
          ></script>

          <script type="text/javascript" src="./assets/js/tawk.js"></script>

          <meta charSet="utf-8" />

          <link href="./assets/css/global.css" rel="stylesheet" />
          <link href="./assets/css/carousel-style.css" rel="stylesheet" />
          <link
            href="https://fonts.googleapis.com/css?family=Lato:400,700|Lora"
            rel="stylesheet"
          />
          <link
            rel="shortcut icon"
            type="image/png"
            href="img/favicon-32x32.png"
            sizes="32x32"
          />
          <link
            rel="icon"
            type="image/png"
            href="img/favicon-16x16.png"
            sizes="16x16"
          />

          <meta
            name="description"
            content="Wildcards lets you support conservation, become a patron of at risk animals and even earn a profit since wildcards are always for sale."
          />
          <meta name="author" content="Wildcards" />
          <meta
            name="keywords"
            content="Wildcards, Blockchain, digital assets, Non Fungible Tokens, Ethereum, Crypto Collectables, Conservation, Gorilla, Black Rhino, Endangered Animals, Harberger Tax, NFT, Always For Sale"
          />

          <meta
            name="viewport"
            content="width=device-width, initial-scale=1, shrink-to-fit=no"
          />
          <meta
            property="og:title"
            content="Wildcards | Protecting at risk animals"
          />
          <meta
            property="og:description"
            content="Wildcards lets you support conservation, become a patron of at risk animals and even earn a profit since wildcards are always for sale."
          />
          <meta
            property="og:image"
            content="./assets/img/logos/wildcards.png"
          />

          <meta
            name="twitter:title"
            content="Wildcards | Protecting at risk animals"
          />
          <meta
            name="twitter:description"
            content="Wildcards lets you support conservation, become a patron of at risk animals and even earn a profit since wildcards are always for sale."
          />
          <meta
            name="twitter:image"
            content="./assets/img/logos/wildcards.png"
          />
          <meta
            name="twitter:card"
            content="./assets/img/logos/wildcards.png"
          />

          {/* <!--
    manifest.json provides metadata used when your web app is added to the
      homescreen on Android. See https://developers.google.com/web/fundamentals/web-app-manifest/
    --> */}
          <link rel="manifest" href="./manifest.webmanifest" />
        </Head>
        <body>
          <Main />
          <NextScript />
        </body>
      </html>
    );
  }
}

// <!-- Global site tag (gtag.js) - Google Analytics -->

// {/* <!-- <script type="text/javascript">
//   window.$crisp = []; window.CRISP_WEBSITE_ID = "6b881313-dab6-4e17-b29e-b1e39eb58400"; (function () { var d = document; var s = d.createElement("script"); s.src = "https://client.crisp.chat/l.js"; s.async = 1; d.getElementsByTagName("head")[0].appendChild(s); })();
// </script> --> */}
// <meta charset="utf-8" />

// <link href="https://fonts.googleapis.com/css?family=Lato:400,700|Lora" rel="stylesheet" />
// <link rel="shortcut icon" type="image/png" href="img/favicon-32x32.png" sizes="32x32" />
// <link rel="icon" type="image/png" href="img/favicon-16x16.png" sizes="16x16" />

// <meta name="description"
//   content="Wildcards lets you support conservation, become a patron of at risk animals and even earn a profit since wildcards are always for sale.">
// <meta name="author" content="Wildcards">
// <meta name="keywords"
//   content="Wildcards, Blockchain, digital assets, Non Fungible Tokens, Ethereum, Crypto Collectables, Conservation, Gorilla, Black Rhino, Endangered Animals, Harberger Tax, NFT, Always For Sale" />

// <title>Wildcards | Protecting at risk animals</title>

// <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
// <meta property="og:title" content="Wildcards | Protecting at risk animals">
// <meta property="og:description"
//   content="Wildcards lets you support conservation, become a patron of at risk animals and even earn a profit since wildcards are always for sale.">
// <meta property="og:image" content="./assets/img/logos/wildcards.png">

// <meta name="twitter:title" content="Wildcards | Protecting at risk animals">
// <meta name="twitter:description"
//   content="Wildcards lets you support conservation, become a patron of at risk animals and even earn a profit since wildcards are always for sale.">
// <meta name="twitter:image" content="./assets/img/logos/wildcards.png">
// <meta name="twitter:card" content="./assets/img/logos/wildcards.png">

// {/* <!--
//   manifest.json provides metadata used when your web app is added to the
//     homescreen on Android. See https://developers.google.com/web/fundamentals/web-app-manifest/
//   --> */}
// <link rel="manifest" href="./manifest.webmanifest" />
