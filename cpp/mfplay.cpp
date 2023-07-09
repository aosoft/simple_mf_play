#include "mfplay_impl.h"

mfplay_impl::mfplay_impl()
{
}

HRESULT mfplay_impl::initialize()
{
    _close_event = unique_handle(::CreateEventW(nullptr, FALSE, FALSE, nullptr));
    if (_close_event == nullptr)
    {
        CHECK_HR(HRESULT_FROM_WIN32(::GetLastError()));
    }
    return S_OK;
}

HRESULT STDMETHODCALLTYPE mfplay_impl::QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
    CHECK_POINTER(ppvObject);
    if (riid == IID_IUnknown)
    {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE mfplay_impl::AddRef(void)
{
    int32_t count = ++_ref_count;
    if (count == 1)
    {
        _shared_this = weak_from_this().lock();
    }
    return count;
}

ULONG STDMETHODCALLTYPE mfplay_impl::Release(void)
{
    int32_t count = --_ref_count;
    if (count == 0)
    {
        _shared_this = nullptr;
    }
    return count;
}
