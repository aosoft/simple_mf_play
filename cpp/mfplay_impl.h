#pragma once

#include "common.h"
#include "mfplay.h"

#include <atomic>

#include <comdef.h>
#include <evr.h>
#include <mfapi.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfobjects.h>
#include <windows.h>

class mfplay_impl : public mfplay,
                    public IMFAsyncCallback,
                    public std::enable_shared_from_this<mfplay_impl> {
private:
    std::atomic_int32_t _ref_count;
    std::shared_ptr<mfplay_impl> _shared_this;

    player_state _state;
    HWND _hwnd_event;
    com_ptr<IMFMediaSession> _session;
    com_ptr<IMFMediaSource> _source;
    com_ptr<IMFVideoDisplayControl> _video_display;
    unique_handle _close_event;

private:
    mfplay_impl(HWND hwnd_event);
    HRESULT initialize(const wchar_t* url, HWND hwnd_video, HWND hwnd_event);

public:
    virtual ~mfplay_impl();
    static HRESULT create_instance(const wchar_t* url, HWND hwnd_video, HWND hwnd_event, mfplay** ret);

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
};