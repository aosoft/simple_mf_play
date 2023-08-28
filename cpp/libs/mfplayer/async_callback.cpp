#include "async_callback.h"

#include <utility>

async_callback::async_callback(IMFMediaSession* session, std::function<HRESULT(com_ptr<IMFMediaEvent>)> fn)
    : _session(session)
    , _fn(std::move(fn))
{
}

HRESULT STDMETHODCALLTYPE async_callback::QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
    CHECK_POINTER(ppvObject);
    if (riid == IID_IUnknown) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE async_callback::AddRef()
{
    int32_t count = ++_ref_count;
    if (count == 1) {
        _shared_this = weak_from_this().lock();
    }
    return count;
}

ULONG STDMETHODCALLTYPE async_callback::Release()
{
    int32_t count = --_ref_count;
    if (count == 0) {
        _shared_this = nullptr;
    }
    return count;
}

HRESULT STDMETHODCALLTYPE async_callback::GetParameters(
    /* [out] */ __RPC__out DWORD* pdwFlags,
    /* [out] */ __RPC__out DWORD* pdwQueue)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE async_callback::Invoke(
    /* [in] */ __RPC__in_opt IMFAsyncResult* pAsyncResult)
{
    if (_session != nullptr && _fn != nullptr) {
        MediaEventType event_type = MEUnknown;
        com_ptr<IMFMediaEvent> event;

        CHECK_HR(_session->EndGetEvent(pAsyncResult, &event));
        CHECK_HR(event->GetType(&event_type));
        if (event_type != MESessionClosed) {
            CHECK_HR(_session->BeginGetEvent(this, nullptr));
        }
        return _fn(event);
    }

    return S_OK;
}
