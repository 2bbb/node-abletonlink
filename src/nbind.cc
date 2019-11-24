#include "./node-abletonlink.hpp"
#include "nbind/nbind.h"

using AbletonLink = bbb::AbletonLink<false>;
using AbletonLinkAudio = bbb::AbletonLink<true>;

NBIND_CLASS(AbletonLink) {
    construct<>();
    construct<double>();
    construct<double, double>();
    construct<double, double, bool>();

    getset(getBeat, setBeat);
    method(setBeatForce);

    getter(getPhase);

    getset(getQuantum, setQuantum);

    getset(getIsPlaying, setIsPlaying);
    method(play);
    method(stop);

    method(getNumPeers);

    getset(getBpm, setBpm);
    getset(getLinkEnable, setLinkEnable);
    method(enable);
    method(disable);

    getset(getIsPlayStateSync, setIsPlayStateSync);
    method(enablePlayStateSync);
    method(disablePlayStateSync);

    method(update);

    method(onTempoChanged);
    method(onNumPeersChanged);
    method(onPlayStateChanged);
    method(on);
    method(off);
}

NBIND_CLASS(AbletonLinkAudio) {
    construct<>();
    construct<double>();
    construct<double, double>();
    construct<double, double, bool>();

    getset(getBeat, setBeat);
    method(setBeatForce);

    getter(getPhase);

    getset(getQuantum, setQuantum);

    getset(getIsPlaying, setIsPlaying);
    method(play);
    method(stop);

    getter(getNumPeers);
    method(getNumPeers);

    getset(getBpm, setBpm);
    getset(getLinkEnable, setLinkEnable);
    method(enable);
    method(disable);

    getset(getIsPlayStateSync, setIsPlayStateSync);
    method(enablePlayStateSync);
    method(disablePlayStateSync);

    method(update);

    method(onTempoChanged);
    method(onNumPeersChanged);
    method(onPlayStateChanged);
    method(on);
    method(off);
}