#pragma once

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

class mfplay {
public:
    virtual void dispose() = 0;
    virtual HRESULT play() = 0;
    virtual HRESULT pause() = 0;
    virtual HRESULT repaint() = 0;
    virtual HRESULT resize_video(std::int32_t width, std::int32_t height) = 0;
};

extern "C" {

HRESULT __declspec(dllexport) mfplay_initialize();
HRESULT __declspec(dllexport) mfplay_finalize();
HRESULT __declspec(dllexport) create_mfplay(const wchar_t* url, HWND hwnd_video, mfplay **ret);
}