# node-abletonlink

node.js port of [ableton Link](https://github.com/ableton/link)

## require env

see detail on [node-gyp](https://github.com/nodejs/node-gyp))

* python v2.7

### Mac

* Xcode

### UNIX

* make

### Windows

* Microsoft windows-build-tools (`npm install --global --production windows-build-tools`)
* Visual C++

## tested

* OSX 10.10〜10.12 with Xcode
* node.js 6.x

## install

```
npm install abletonlink --save
```

or

```
npm install 2bbb/node-abletonlink --save
```

## how to use

```js
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
