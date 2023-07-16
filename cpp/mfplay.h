#pragma once

#include <cstdint>
#include <windows.h>

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
    virtual HRESULT STDMETHODCALLTYPE play() = 0;
    virtual HRESULT STDMETHODCALLTYPE pause() = 0;
};

extern "C" {

HRESULT __declspec(dllexport) mfplay_initialize();
HRESULT __declspec(dllexport) mfplay_finalize();
}