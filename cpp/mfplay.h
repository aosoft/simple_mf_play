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

class mfplay : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE play() = 0;
    virtual HRESULT STDMETHODCALLTYPE pause() = 0;
};