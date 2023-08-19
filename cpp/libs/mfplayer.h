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

using hresult_t = std::int32_t;
using hwnd_t = void*;

class mfplayer {
public:
    virtual void dispose() = 0;
    virtual hresult_t play() = 0;
    virtual hresult_t pause() = 0;
    virtual hresult_t repaint() = 0;
    virtual hresult_t resize_video(std::int32_t width, std::int32_t height) = 0;
};

extern "C" {

hresult_t __declspec(dllexport) mfplayer_initialize();
hresult_t __declspec(dllexport) mfplayer_finalize();
hresult_t __declspec(dllexport) create_mfplayer(const wchar_t* url, hwnd_t hwnd_video, mfplayer**ret);
}