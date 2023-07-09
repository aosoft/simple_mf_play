#pragma once

#include <windows.h>
#include <cstdint>

enum class player_state : std::int32_t {
    closed = 0,
    ready,
    open_pending,
    started,
    paused,
    stopped,
    closing,
};

class mfplay : public IUnknown {
};