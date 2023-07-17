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

class __declspec(uuid("27046F78-0339-4B93-893F-546D144ED54E")) mfplay : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE play() = 0;
    virtual HRESULT STDMETHODCALLTYPE pause() = 0;
    virtual HRESULT STDMETHODCALLTYPE repaint() = 0;
    virtual HRESULT STDMETHODCALLTYPE resize_video(std::int32_t width, std::int32_t height) = 0;
};

extern "C" {

HRESULT __declspec(dllexport) STDMETHODCALLTYPE mfplay_initialize();
HRESULT __declspec(dllexport) STDMETHODCALLTYPE mfplay_finalize();
HRESULT __declspec(dllexport) STDMETHODCALLTYPE create_mfplay(const wchar_t* url, HWND hwnd_video, mfplay **ret);
}