const nbind = require('nbind');
const binding = nbind.init(__dirname);
const lib = binding.lib;

lib.AbletonLink.prototype.startUpdate = function(interval_ms, callback) {
    if(callback) {
        this.timer = setInterval(() => {
            this.update();
            callback(this.beat, this.phase, this.bpm);
        }, interval_ms);
    } else {
        this.timer = setInterval(() => {
            this.update();
        }, interval_ms);
    }
    this.update();
};

lib.AbletonLink.prototype.stopUpdate = function() {
    if(this.timer) {
        clearInterval(this.timer);
        this.timer = null;
    }
}

module.exports = lib.AbletonLink;
