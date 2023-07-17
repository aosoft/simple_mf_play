#pragma once

#include "common.h"
#include "mfplay.h"
#include "window_message_queue.h"

#include <atomic>

#include <evr.h>
#include <mfapi.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfobjects.h>

class mfplay_impl : public mfplay,
                    public IMFAsyncCallback,
                    public std::enable_shared_from_this<mfplay_impl> {
private:
    std::atomic_int32_t _ref_count;
    std::shared_ptr<mfplay_impl> _shared_this;

    player_state _state;
    window_message_queue _queue;
    com_ptr<IMFMediaSession> _session;
    com_ptr<IMFMediaSource> _source;
    com_ptr<IMFVideoDisplayControl> _video_display;
    unique_handle _close_event;

private:
    mfplay_impl();
    HRESULT initialize(const wchar_t* url, HWND hwnd_video);

public:
    virtual ~mfplay_impl();
    static HRESULT create_instance(const wchar_t* url, HWND hwnd_video, mfplay** ret);

public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;

    virtual ULONG STDMETHODCALLTYPE Release(void) override;

public:
    virtual HRESULT STDMETHODCALLTYPE GetParameters(
        /* [out] */ __RPC__out DWORD* pdwFlags,
        /* [out] */ __RPC__out DWORD* pdwQueue) override;

    virtual HRESULT STDMETHODCALLTYPE Invoke(
        /* [in] */ __RPC__in_opt IMFAsyncResult* pAsyncResult) override;

private:
    static void on_event_callback(std::weak_ptr<mfplay_impl> self, com_ptr<IMFMediaEvent> event);
    HRESULT on_event_callback2(com_ptr<IMFMediaEvent> event);

public:
    virtual HRESULT STDMETHODCALLTYPE play() override;
    virtual HRESULT STDMETHODCALLTYPE pause() override;
    virtual HRESULT STDMETHODCALLTYPE repaint() override;
    virtual HRESULT STDMETHODCALLTYPE resize_video(std::int32_t width, std::int32_t height) override;

private:
    HRESULT start_playback();
};