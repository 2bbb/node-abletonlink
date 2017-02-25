const abletonlink = require('./index.js');

const link = new abletonlink();

function fn(tempo) {
    console.log(tempo);
}

link.on('tempo', fn);

setInterval(() => {
    link.update();
    console.log('phase', link.phase);
}, 60);
