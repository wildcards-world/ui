module.exports = {
  webpack: (config, { buildId, dev, isServer, defaultLoaders, webpack }) => {
    // Note: we provide webpack above so you should not `require` it
    // Perform customizations to webpack config
    // Important: return the modified config
    config.plugins.push(new webpack.IgnorePlugin(/\/__tests__\//));

    //     module.exports = {
    //   module: {
    //     rules: [
    //       {
    //         test: /\.(png|jpe?g|gif)$/i,
    //         use: [
    //           {
    //             loader: 'file-loader',
    //           },
    //         ],
    //       },
    //     ],
    //   },
    // };
    // return config;
    const { module = {} } = config;
    return {
      ...config,
      module: {
        ...module,
        rules: [
          ...(module.rules || []),
          {
            test: /\.(png|woff|woff2|eot|ttf|gif|jpg|ico|svg)$/,
            use: [
              {
                loader: "file-loader",
              },
            ],
          },
        ],
      },
    };
  },
  webpackDevMiddleware: (config) => {
    // Perform customizations to webpack dev middleware config
    // Important: return the modified config
    return config;
  },
};
