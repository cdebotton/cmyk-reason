const path = require('path');

const NODE_ENV = (process.env.NODE_ENV || 'development').trim();

module.exports = {
  mode: NODE_ENV === 'production' ? 'production' : 'development',
  entry: path.join(__dirname, 'lib', 'es6', 'client', 'Client.bs.js'),
  output: {
    filename: 'bundle.js',
    path: path.join(__dirname, 'dist'),
  },
};
