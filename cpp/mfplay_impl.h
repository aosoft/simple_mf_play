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

class mfplay_impl : public mfplay, public std::enable_shared_from_this<mfplay_impl> {
private:
    std::atomic_int32_t _ref_count;
    std::shared_ptr<mfplay_impl> _shared_this;

    com_ptr<IMFMediaSession> _session;
    com_ptr<IMFMediaSource> _source;
    com_ptr<IMFVideoDisplayControl> _video_display;
    unique_handle _close_event;

private:
    mfplay_impl();
    HRESULT initialize(const wchar_t* url);

public:
    virtual ~mfplay_impl();
    static HRESULT create_instance(const wchar_t* url, mfplay** ret);


public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    virtual ULONG STDMETHODCALLTYPE AddRef(void) override;

    virtual ULONG STDMETHODCALLTYPE Release(void) override;
};