let config = require('./webpack.config.js');
const webpack = require('webpack');
config.mode = "production";
config.plugins = [new webpack.DefinePlugin({
    URL: JSON.stringify('https://app.playchessnstuff.com'),
    WSURL: JSON.stringify('wss://app.playchessnstuff.com'),
  })];
module.exports = config;