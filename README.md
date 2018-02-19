# node-abletonlink

node.js port of [ableton Link](https://github.com/ableton/link) with [nbind](https://github.com/charto/nbind)

## Dependencies

* [ableton/link](https://github.com/ableton/link)
    * [chriskohlhoff/asio](https://github.com/chriskohlhoff/asio)
    * [philsquared/Catch](https://github.com/philsquared/Catch)
* [charto/nbind](https://github.com/charto/nbind)

## Required

see detail on [node-gyp](https://github.com/nodejs/node-gyp)

### Common

* python v2.7

### Mac

* Xcode

### UNIX

* make

### Windows

* Microsoft windows-build-tools (`npm install --global --production windows-build-tools`)
* if you install version of MSVS isn't 2015, `npm config set msvs_version 2015` before install this package.

## Tested env

* OSX 10.10〜10.12 with Xcode / node.js 6.x, 8.9.4
* Ubuntu 16.04 (on docker) with gcc 4.9.4 / node.js 7.0.0
* Ubuntu 16.04 (on docker) with gcc 6.2.0 / node.js 7.0.0
* Fedora 25 (on docker) with gcc 6.2.1 / node.js 7.0.0
      * but it will get **stack** [issue#1](https://github.com/2bbb/node-abletonlink/issues/1)
* Windows 10 with windows-build-tools / node.js 8.9.4

## Install

```
npm install abletonlink --save
```

or

```
npm install 2bbb/node-abletonlink --save
```

## How to use

```js
const abletonlink = require('abletonlink');
const link = new abletonlink();

link.startUpdate(60, (beat, phase, bpm) => {
    console.log("updated: ", beat, phase, bpm);
});

// callback is option.
// link.startUpdate(60); // correct!

function do_something() {
    const beat = link.beat;
    const phase = link.phase;
    const bpm = link.bpm;
    ...
}
```

## Example

* [node-abletonlink-example](https://github.com/2bbb/node-abletonlink-example)

## API

## property

* `beat`: `number`
* `bpm`: `number`
* `phase`: `number`
* `quantum`: `number`

## method

* `getNumPeers`: `(void) -> number`

get num peers.

* `setBeatForce`: `(beat: number) -> void`

set beat force.

* `on`: `(key: string, callback: (number) -> void) -> void`

set callback will call change event.

`key` is `'tempo'` then argument of callback is new `tempo`.

`key` is `'numPeers'` then argument of callback is new `numPeers`.

* `off` : `(key: string) -> void`

remove callback.

* `enable`: `(void) -> void`
* `disable`: `(void) -> void`

* `update`: `(void) -> void`

call update manually.

* `startUpdate`: `(interval: number [, callback: (beat:number, phase:number, bpm:number) -> void]) -> void`

start update timer with interval.

if given callback, it will call every interval with arguments `beat`, `phase`, `bpm`.

* `stopUpdate`: `(void) -> void`

stop update timer.

## License

MIT

## Author

* ISHII 2bit [bufferRenaiss co., ltd.]
* ishii[at]buffer-renaiss.com

## Special Thanks

* [hlolli](https://github.com/hlolli)

## At last

If you get happy with using this addon, and you're rich, please donation for support continuous development.

Bitcoin: `17AbtW73aydfYH3epP8T3UDmmDCcXSGcaf`
