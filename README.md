# Wild Cards web app

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
 run re:watch
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
