declare module 'abletonlink' {
  export const Audio = class AbletonLinkAudio extends AbletonLinkBase {}
  export = class AbletonLink extends AbletonLinkBase {}
}

type eventTypes = 'tempo' | 'numPeers' | 'playState'
type updateCallback = (beat:number, phase: number, bpm: number, playState: boolean) => any

abstract class AbletonLinkBase {
  // native code
  constructor(bpm?: number, quantum?: number, enable?: boolean)
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
  on(key:eventTypes, cb: Function): void
  off(key:eventTypes): void
  // JavaScript
  startUpdate(interval_ms: number, cb?: updateCallback): void
  stopUpdate(): void
}
