# Wildcards web app

## Basic Usage

Make sure you have yarn [installed](https://classic.yarnpkg.com/en/docs/install#debian-stable). (Currently we are just using yarn v1. When v2 is more stable we can consider upgrading).

```
yarn install
```

```
yarn start
```

For development, using the [vs-code reason plugin](https://github.com/jaredly/reason-language-server) your code should reload on every save.

If however you don't use such a plugin you can still watch for file changes using the following command. (Sometimes it is useful to do this to see what is happening in the terminal.)

```
 yarn res:watch
```

Sometimes VsCode stops working properly. In this case you can run `<ctrl+shift+p>` and then either `Developer: Reload Window` or `Restart Reason Language Server`. Another thing that you can do is run `npm run re:build` (which rebuilds everything, and might fix any buggy vscode behaviour.)

### Build for production

```
yarn run build
```

### Test

```
yarn run test
```

## Code Quality and Analysis

Dead code analysis is provided by (reanalyse)[https://github.com/reason-association/reanalyze]. Running `yarn re:analyse-write` will add `@dead` annotations to all places in the code where code could be unused/dead. There are places where the code is needed for javascript interop and in those cases the `@dead` annotation should typically be converted into a `@live` annotation ((see their meanings)[https://github.com/reason-association/reanalyze#dce-controlling-reports-with-annotations]).

You can also get a breakdown of the bundle size (which packages etc contribute towards it) by running `yarn build && yarn analyze`.
