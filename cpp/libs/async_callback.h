#pragma once

#include "common.h"
#include <atomic>
#include <functional>
#include <memory>
#include <mfidl.h>

class async_callback : public IMFAsyncCallback, public std::enable_shared_from_this<async_callback> {
private:
    std::atomic_int32_t _ref_count;
    std::shared_ptr<async_callback> _shared_this;
    com_ptr<IMFMediaSession> _session;

    std::function<HRESULT(IMFAsyncResult*)> _fn;

public:
    async_callback(IMFMediaSession* session, std::function<HRESULT(IMFAsyncResult*)> fn);

    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

    ULONG STDMETHODCALLTYPE AddRef() override;

    ULONG STDMETHODCALLTYPE Release() override;

    HRESULT STDMETHODCALLTYPE GetParameters(
        /* [out] */ __RPC__out DWORD* pdwFlags,
        /* [out] */ __RPC__out DWORD* pdwQueue) override;

    HRESULT STDMETHODCALLTYPE Invoke(
        /* [in] */ __RPC__in_opt IMFAsyncResult* pAsyncResult) override;
};
