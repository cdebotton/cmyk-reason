const path = require('path');

const NODE_ENV = (process.env.NODE_ENV || 'development').trim();
const mode = NODE_ENV === 'production' ? NODE_ENV : 'development';

module.exports = {
  mode,
  entry: path.join(__dirname, 'lib', 'es6', 'client', 'Client.bs.js'),
  output: {
    filename: 'bundle.js',
    path: path.join(__dirname, 'dist/client'),
    publicPath: 'http://localhost:8080/',
  },
  resolve: {
    alias: {
      react: path.join(__dirname, `vendor/react/build/dist/react.${mode}.js`),
      'react-dom': path.join(
        __dirname,
        `vendor/react/build/dist/react-dom.${mode}.js`,
      ),
    },
  },
};
