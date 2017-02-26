#include <memory>
#include <queue>
#include <mutex>

#include <ableton/Link.hpp>

#include "nbind/nbind.h"
#include "nbind/api.h"
#include <node.h>

namespace bbb {
    class AbletonLink;

    struct ALTempoQueue {
        AbletonLink *that;
        double bpm;
    };
    struct ALNumPeersQueue {
        AbletonLink *that;
        std::size_t numPeers;
    };

    static uv_async_t async;
    static std::mutex bbb_mutex;
    static std::queue<ALTempoQueue> bbb_tempo_queue;
    static std::queue<ALNumPeersQueue> bbb_peers_queue;
    static void bbb_async_cb_handler(uv_async_t *handle);

    class AbletonLink {
        ableton::Link link;
        double beat{0.0};
        double phase{0.0};
        double bpm{120.0};
        double quantum{4.0};

    public:
        AbletonLink()
        : AbletonLink(120.0) {}
        AbletonLink(double bpm)
        : AbletonLink(bpm, 4.0) {}
        AbletonLink(double bpm, double quantum)
        : AbletonLink(bpm, quantum, true) {}
        AbletonLink(double bpm, double quantum, bool enable)
        : link(bpm)
        , bpm(bpm)
        , quantum(quantum)
        , tempoCallback(nullptr)
        , numPeersCallback(nullptr)
        {
            static bool b{true};
            if(b) {
                uv_async_init(uv_default_loop(), &async, bbb_async_cb_handler);
                b = false;
            }
            link.enable(enable);
            link.setTempoCallback([this](double bpm) {
                std::lock_guard<std::mutex> lock(bbb_mutex);
                bbb_tempo_queue.push({this, bpm});
                uv_async_send(&async);
            });
            link.setNumPeersCallback([this](std::size_t num) {
                std::lock_guard<std::mutex> lock(bbb_mutex);
                bbb_peers_queue.push({this, num});
                uv_async_send(&async);
            });
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
        
        double getBpm() const { return bpm; }
        void setBpm(double bpm) {
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
                tempoCallback = std::make_shared<nbind::cbFunction>(callback);
            } else if(key == "numPeers") {
                numPeersCallback = std::make_shared<nbind::cbFunction>(callback);
            }
        }

        void off(std::string key) {
            if(key == "tempo") {
                tempoCallback.reset();
            } else if(key == "numPeers") {
                numPeersCallback.reset();
            }
        }
        
    public:
        void tempoChanged(double tempo) {
            if(tempoCallback) {
                Nan::HandleScope scope;
                (*tempoCallback)(tempo);
            }
        }

        void numPeersChanged(std::size_t num) {
            if(numPeersCallback) {
                Nan::HandleScope scope;
                (*numPeersCallback)(num);
            }
        }

        std::shared_ptr<nbind::cbFunction> tempoCallback;
        std::shared_ptr<nbind::cbFunction> numPeersCallback;

        void update() {
            const auto time = link.clock().micros();
            auto timeline = link.captureAppTimeline();
            
            beat = timeline.beatAtTime(time, quantum);
            phase = timeline.phaseAtTime(time, quantum);
            bpm = timeline.tempo();
        };
    };

    static void bbb_async_cb_handler(uv_async_t *handle) {
        std::lock_guard<std::mutex> sl(bbb_mutex);
        while(!bbb_tempo_queue.empty()) {
            auto &front = bbb_tempo_queue.front();
            front.that->tempoChanged(front.bpm);
            bbb_tempo_queue.pop();
        }
        while(!bbb_peers_queue.empty()) {
            auto &front = bbb_peers_queue.front();
            front.that->numPeersChanged(front.numPeers);
            bbb_peers_queue.pop();
        }
    }
};
