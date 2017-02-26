const abletonlink = require('./index.js');

// const link = new abletonlink(bpm = 120.0, quantum = 4.0, enable = true);
const link = new abletonlink(160.0);

link.on('tempo', (tempo) => console.log("tempo", tempo));
link.on('numPeers', (numPeers) => console.log("numPeers", numPeers));
link.startUpdate(60, (beat, phase, bpm) => console.log("updated", beat, phase, bpm));
// or link.startUpdate(60);