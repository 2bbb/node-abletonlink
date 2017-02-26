# node-abletonlink

node.js port of [ableton Link](https://github.com/ableton/link)

## install

```
npm install 2bbb/node-abletonlink --save
```

## require env

* python v2.7

### Mac

* Xcode

### UNIX

* make

### Windows

* Microsoft windows-build-tools (`npm install --global --production windows-build-tools`)
* Visual C++

(see [node-gyp](https://github.com/nodejs/node-gyp))

## how to use

```
const abletonlink = require('abletonlink');
const link = new abletonlink();

link.startUpdate(60, (beat, phase, bpm) => {
    console.log("updated: ", beat, phase, bpm);
});

function do_something() {
    const bpm = link.bpm;
    const phase = link.phase;
    const bpm = link.bpm;
    ...
}

```
