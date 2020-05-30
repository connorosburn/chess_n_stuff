const path = require("path");
const webpack = require("webpack");

module.exports = {
    entry: "./lib/index.js",
    mode: "development",
    module: {
        rules: [
            {
                test: /\.(js|jsx)$/,
                exclude: /(node_modules|bower_components)/,
                loader: "babel-loader",
                options: { 
                    presets: ["@babel/preset-react"], 
                    plugins: ["@babel/plugin-proposal-class-properties", "@babel/plugin-proposal-private-methods"]
                }
            },
            {
                test: /\.css$/,
                use: ["style-loader", "css-loader"]
            }
        ]
    },
    resolve: { extensions: ["*", ".js", ".jsx"] },
    output: {
        path: __dirname,
        publicPath: "/",
        filename: "bundle.js"
    },
    devServer: {
        contentBase: path.join(__dirname, "/"),
        port: 3000,
        publicPath: "http://localhost:3000/",
        hotOnly: true
    },
    plugins: [new webpack.HotModuleReplacementPlugin(), new webpack.DefinePlugin({
        URL: JSON.stringify('http://localhost:8000'),
        WSURL: JSON.stringify('ws://localhost:8000'),
    })]
};