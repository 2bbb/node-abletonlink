const nbind = require('nbind');
const binding = nbind.init(__dirname);
const lib = binding.lib;

module.exports = lib.AbletonLink;
