#include <ableton/Link.hpp>
#include <iostream>
#include <memory>
#include <map>

#include "nbind/nbind.h"
#include "nbind/api.h"

namespace bbb {
    class AbletonLink {
        ableton::Link link;
        double beat{0.0};
        double phase{0.0};
        double bpm{120.0};
        double quantum{4.0};
        std::map<std::string, nbind::cbFunction> callbacks;

    public:
        AbletonLink(double bpm = 120.0, double quantum = 4.0, bool enable = true)
        : link(bpm)
        , bpm(bpm)
        , quantum(quantum)
        {
            link.enable(enable);
        };
        explicit AbletonLink(const AbletonLink *other)
        : AbletonLink(other->bpm, other->quantum, other->getLinkEnable()) {}

        bool getLinkEnable() const { return link.isEnabled(); }
        void setLinkEnable(bool enable) { link.enable(enable); }
        void enable() { link.enable(true); }
        void disable() { link.enable(false); }
        
        double getBeat() const { return beat; }
        void setBeat(double beat) {
            const auto time = link.clock().micros();
            auto timeline = link.captureAppTimeline();
            timeline.requestBeatAtTime(beat, time, quantum);
            link.commitAppTimeline(timeline);
        }
        void setBeatForce(double beat) {
            const auto time = link.clock().micros();
            auto timeline = link.captureAppTimeline();
            timeline.forceBeatAtTime(beat, time, quantum);
            link.commitAppTimeline(timeline);
        }
        
        double getPhase() const { return phase; }
        void setPhase(double phase) {
            // const auto time = link.clock().micros();
            // auto timeline = link.captureAppTimeline();
        }
        
        double getBPM() const { return bpm; }
        void setBPM(double bpm) {
            this->bpm = bpm;
            const auto time = link.clock().micros();
            auto timeline = link.captureAppTimeline();
            timeline.setTempo(bpm, time);
            link.commitAppTimeline(timeline);
        }
        
        std::size_t getNumPeers() const { return link.numPeers(); }
        
        void setQuantum(double quantum) { this->quantum = quantum; }
        double getQuantum() const { return quantum; }
        
        void onTempoChanged(nbind::cbFunction &callback) {
            on("tempo", callback);
        }
        void onNumPeersChanged(nbind::cbFunction &callback) {
            on("numPeers", callback);
        }
        void on(std::string key, nbind::cbFunction &callback) {
            if(key == "tempo") {
                if(tempoCallback) delete tempoCallback;
                tempoCallback = new nbind::cbFunction(callback);
                link.setTempoCallback([this](double bpm) {
                    if(tempoCallback) tempoCallback->call<void>(bpm);
                });
            }
        }

        void off(std::string key) {
            if(key == "tempo") link.setTempoCallback([](double) {});
            else if(key == "numPeers") link.setNumPeersCallback([](std::size_t) {});
        }
    public:
        nbind::cbFunction *tempoCallback{nullptr};
        void update() {
            const auto time = link.clock().micros();
            auto timeline = link.captureAppTimeline();
            
            beat = timeline.beatAtTime(time, quantum);
            phase = timeline.phaseAtTime(time, quantum);
            bpm = timeline.tempo();
        };
    };
};
