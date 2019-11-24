#pragma once

#if defined(BOOST_NO_ANSI_APIS) || (defined(_MSC_VER) && (_MSC_VER >= 1800))
#   include <malloc.h>
#endif

#include <memory>
#include <queue>
#include <mutex>

#include <ableton/Link.hpp>

#include "nbind/nbind.h"
#include "nbind/api.h"
#include <node.h>

namespace bbb {
    template <bool is_audio_thread>
    class AbletonLink {
        struct ALTempoQueue {
            AbletonLink *that;
            double bpm;
        };
        struct ALNumPeersQueue {
            AbletonLink *that;
            std::size_t numPeers;
        };
        struct ALPlayingQueue {
            AbletonLink *that;
            bool isPlaying;
        };

        inline static uv_async_t &async() {
            static uv_async_t _;
            return _;
        };
        inline static std::mutex &bbb_mutex() {
            static std::mutex _;
            return _;
        }
        inline static std::queue<ALTempoQueue> &bbb_tempo_queue() {
            static std::queue<ALTempoQueue> _;
            return _;
        }
        static std::queue<ALNumPeersQueue> &bbb_peers_queue() {
            static std::queue<ALNumPeersQueue> _;
            return _;
        }
        static std::queue<ALPlayingQueue> &bbb_is_playing_queue() {
            static std::queue<ALPlayingQueue> _;
            return _;
        }
        static void bbb_async_cb_handler(uv_async_t *handle) {
            std::lock_guard<std::mutex> sl(bbb_mutex());
            while(!bbb_tempo_queue().empty()) {
                auto &front = bbb_tempo_queue().front();
                front.that->tempoChanged(front.bpm);
                bbb_tempo_queue().pop();
            }
            while(!bbb_peers_queue().empty()) {
                auto &front = bbb_peers_queue().front();
                front.that->numPeersChanged(front.numPeers);
                bbb_peers_queue().pop();
            }
            while(!bbb_is_playing_queue().empty()) {
                auto &front = bbb_is_playing_queue().front();
                front.that->playStateChanged(front.isPlaying);
                bbb_is_playing_queue().pop();
            }
        }

        ableton::Link link;
        double beat{0.0};
        double phase{0.0};
        double bpm{120.0};
        double quantum{4.0};
        bool isPlayingWhenUpdate{false};

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
                uv_async_init(uv_default_loop(), &async(), bbb_async_cb_handler);
                b = false;
            }
            link.enable(enable);
            link.setTempoCallback([this](double bpm) {
                std::lock_guard<std::mutex> lock(bbb_mutex());
                bbb_tempo_queue().push({this, bpm});
                uv_async_send(&async());
            });
            link.setNumPeersCallback([this](std::size_t num) {
                std::lock_guard<std::mutex> lock(bbb_mutex());
                bbb_peers_queue().push({this, num});
                uv_async_send(&async());
            });
            link.setStartStopCallback([this](bool isPlaying) {
                std::lock_guard<std::mutex> lock(bbb_mutex());
                bbb_is_playing_queue().push({this, isPlaying});
                uv_async_send(&async());
            });
        };
        explicit AbletonLink(const AbletonLink *other)
        : AbletonLink(other->bpm, other->quantum, other->getLinkEnable()) {}

        inline bool getLinkEnable() const { return link.isEnabled(); }
        inline void setLinkEnable(bool enable) { link.enable(enable); }
        inline void enable() { link.enable(true); }
        inline void disable() { link.enable(false); }
        
        inline bool getIsPlayStateSync() const { return link.isStartStopSyncEnabled(); }
        inline void setIsPlayStateSync(bool bEnable) { link.enableStartStopSync(bEnable); };
        inline void enablePlayStateSync() { link.enableStartStopSync(true); };
        inline void disablePlayStateSync() { link.enableStartStopSync(false); };
        
        double getBeat() const { return beat; }
        void setBeat(double beat) {
            const auto time = get_time();
            auto &&sessionState = get_session_state();
            sessionState->requestBeatAtTime(beat, time, quantum);
        }
        void setBeatForce(double beat) {
            const auto &&time = get_time();
            auto &&sessionState = get_session_state();
            sessionState->forceBeatAtTime(beat, time, quantum);
        }
        
        inline double getPhase() const { return phase; }
        void setPhase(double phase) {
            this->phase = phase;
            const auto &&time = get_time();
            auto &&sessionState = get_session_state();
            sessionState->phaseAtTime(phase, time);
        }
        
        inline double getBpm() const { return bpm; }
        void setBpm(double bpm) {
            this->bpm = bpm;
            const auto &&time = get_time();
            auto &&sessionState = get_session_state();
            sessionState->setTempo(bpm, time);
        }
        
        inline bool getIsPlaying() const
        { return link.captureAppSessionState().isPlaying(); };
        inline void setIsPlaying(bool isPlaying) {
            const auto &&time = get_time();
            auto &&sessionState = get_session_state();
            sessionState->setIsPlaying(isPlaying, time);
        }
        inline void play() {
            const auto &&time = get_time();
            auto &&sessionState = get_session_state();
            sessionState->setIsPlaying(true, time);
        }
        inline void stop() {
            const auto &&time = get_time();
            auto &&sessionState = get_session_state();
            sessionState->setIsPlaying(false, time);
        }

        inline std::size_t getNumPeers() const { return link.numPeers(); }
        
        inline void setQuantum(double quantum) { this->quantum = quantum; }
        inline double getQuantum() const { return quantum; }
        
        inline void onTempoChanged(nbind::cbFunction &callback) {
            on("tempo", callback);
        }
        inline void onNumPeersChanged(nbind::cbFunction &callback) {
            on("numPeers", callback);
        }
        inline void onPlayStateChanged(nbind::cbFunction &callback) {
            on("playState", callback);
        }
        void on(std::string key, nbind::cbFunction &callback) {
            if(key == "tempo") {
                tempoCallback = std::make_shared<nbind::cbFunction>(callback);
            } else if(key == "numPeers") {
                numPeersCallback = std::make_shared<nbind::cbFunction>(callback);
            } else if(key == "playState") {
                playStateCallback = std::make_shared<nbind::cbFunction>(callback);
            }
         }

        void off(std::string key) {
            if(key == "tempo") {
                tempoCallback.reset();
            } else if(key == "numPeers") {
                numPeersCallback.reset();
            } else if(key == "playState") {
                playStateCallback.reset();
            }
        }
        
        inline void tempoChanged(double tempo) {
            if(tempoCallback) {
                Nan::HandleScope scope;
                (*tempoCallback)(tempo);
            }
        }

        inline void numPeersChanged(std::size_t num) {
            if(numPeersCallback) {
                Nan::HandleScope scope;
                (*numPeersCallback)(num);
            }
        }

        inline void playStateChanged(bool isPlaying) {
            if(playStateCallback) {
                Nan::HandleScope scope;
                (*playStateCallback)(isPlaying);
            }
        }

        std::shared_ptr<nbind::cbFunction> tempoCallback;
        std::shared_ptr<nbind::cbFunction> numPeersCallback;
        std::shared_ptr<nbind::cbFunction> playStateCallback;

        void update() {
            const auto &&time = link.clock().micros();
            auto &&sessionState = link.captureAppSessionState();
            
            beat = sessionState.beatAtTime(time, quantum);
            phase = sessionState.phaseAtTime(time, quantum);
            bpm = sessionState.tempo();
            isPlayingWhenUpdate = sessionState.isPlaying();
        };

    private:
        inline std::chrono::microseconds get_time() const
        { return link.clock().micros(); }

        struct scoped_session_state {
            scoped_session_state() = delete;
            scoped_session_state(ableton::Link &link)
            : link(link)
            , sessionState(is_audio_thread ? link.captureAudioSessionState() : link.captureAppSessionState())
            {}
            ~scoped_session_state()
            {
                if(is_audio_thread) {
                    link.commitAppSessionState(sessionState);
                } else {
                    link.commitAudioSessionState(sessionState);
                }
            };

            ableton::Link &link;
            ableton::Link::SessionState sessionState;
            inline ableton::Link::SessionState *operator->()
            { return &sessionState; };
            inline const ableton::Link::SessionState *operator->() const
            { return &sessionState; };
        };
        inline scoped_session_state get_session_state()
        { return { link }; };
    };
};
