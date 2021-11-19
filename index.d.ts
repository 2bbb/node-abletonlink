type EventTypes = 'tempo' | 'numPeers' | 'playState';
type updateCallback = (beat:number, phase: number, bpm: number, playState: boolean) => any;

declare class AbletonLinkBase {
  setBeatForce(beat: number): void
  play(): void
  stop(): void
  getNumPeers(): number
  enable(): void
  disable(): void
  enablePlayStateSync(): void
  disablePlayStateSync(): void
  update(): void
  onTempoChanged(cb: Function): void
  onNumPeersChanged(cb: Function): void
  onPlayStateChanged(cb: Function): void
  on(key: 'tempo', cb: (tempo: number) => any): void
  on(key: 'numPeers', cb: (num_peers: number) => any): void
  on(key: 'playState', cb: (play_state: boolean) => any): void
  on(key: EventTypes, cb: Function): void
  off(key: EventTypes, cb?: Function): void
  // JavaScript
  startUpdate(quantum: number, cb: updateCallback): void
  startUpdate(interval_ms: number, cb?: updateCallback): void
  stopUpdate(): void
};

export default class AbletonLink extends AbletonLinkBase {
  constructor(bpm?: number, quantum?: number, enable?: boolean): AbletonLink;
};

class AbletonLinkAudio extends AbletonLinkBase {
  constructor(bpm?: number, quantum?: number, enable?: boolean): AbletonLinkAudio;
};

declare namespace AbletonLink {
    export const Audio: typeof AbletonLinkAudio;
}

