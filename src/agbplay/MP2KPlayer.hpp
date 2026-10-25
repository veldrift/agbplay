#pragma once

#include "Constants.hpp"
#include "MP2KTrack.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

class Rom;
struct MP2KContext;

struct MP2KPlayer
{
    MP2KPlayer(const MP2KContext &ctx, const PlayerInfo &playerInfo, uint8_t playerIdx);
    MP2KPlayer(const MP2KPlayer &) = delete;
    MP2KPlayer(MP2KPlayer &&) = default;
    MP2KPlayer &operator=(const MP2KPlayer &) = delete;

    void Init(const Rom &rom, size_t songHeaderPos);

    std::vector<MP2KTrack> tracks;

    /* playback state */
    bool playing = false;
    bool finished = true;
    /* frog-switch: game-driven tempo factor (m4aMPlayTempoControl mirror).
       HW multiplies song tempo (tempoD) by game tempo (tempoU / 256); the
       song half already lives in bpm, this is the game half. MPlayStart
       resets tempoU to 0x100, hence the Init reset. */
    float tempoFactor = 1.0f;
    /* frog-switch: bumped by every accepted m4aMPlayStart (priority can
       refuse one), so the bridge knows when HW would clear its tracks. */
    uint32_t startCount = 0;

    /* player timing state */
    size_t interframeCount = 0;
    size_t frameCount = 0;
    size_t tickCount = 0;
    uint64_t tickProgress_32_32 = 0;
    uint16_t bpm = 0;

    /* variables initialized by song start */
    size_t songHeaderPos = 0;
    size_t bankPos = 0;
    uint8_t tracksUsed = 0;
    uint8_t reverb = 0;
    uint8_t priority = 0;

    /* player constants */
    const uint8_t trackLimit;
    const uint8_t playerIdx;
    const bool usePriority;
};
