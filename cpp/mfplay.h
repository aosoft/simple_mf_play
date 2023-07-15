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

const UINT WM_APP_PLAYER_EVENT = WM_APP + 1;

class __declspec(uuid("27046F78-0339-4B93-893F-546D144ED54E")) mfplay : public IUnknown {
public:
};