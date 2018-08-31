const path = require('path');
const webpack = require('webpack');

const NODE_ENV = (process.env.NODE_ENV || 'development').trim();
const mode = NODE_ENV === 'production' ? 'production' : 'development';
const react = NODE_ENV === 'production' ? 'production.min' : 'development';

module.exports = {
  mode,
  entry: path.join(__dirname, 'lib', 'es6', 'app', 'index.bs.js'),
  output: {
    filename: 'bundle.js',
    path: path.join(__dirname, 'dist'),
    publicPath: 'http://localhost:8080/',
  },
  optimization: {
    splitChunks: {
      chunks: 'all',
    },
  },
  resolve: {
    alias: {
      react: path.join(__dirname, `vendor/react/build/dist/react.${react}.js`),
      'react-dom': path.join(
        __dirname,
        `vendor/react/build/dist/react-dom.${react}.js`,
      ),
    },
  },
  plugins: [
    new webpack.DefinePlugin({
      'process.env': {
        NODE_ENV: `'${NODE_ENV}'`,
      },
    }),
  ],
};
