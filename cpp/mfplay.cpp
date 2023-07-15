#include "mfplay_impl.h"
#include <exception>

mfplay_impl::mfplay_impl(HWND hwnd_event)
    : _hwnd_event(hwnd_event)
    , _state(player_state::closed)
{
}

HRESULT mfplay_impl::initialize(const wchar_t* url, HWND hwnd_video, HWND hwnd_event)
{
    HRESULT hr = S_OK;

    _close_event = unique_handle(::CreateEventW(nullptr, FALSE, FALSE, nullptr));
    if (_close_event == nullptr) {
        CHECK_HR(HRESULT_FROM_WIN32(::GetLastError()));
    }

    com_ptr<IMFSourceResolver> source_resolver;
    MF_OBJECT_TYPE object_type = MF_OBJECT_INVALID;
    com_ptr<IUnknown> source;
    com_ptr<IMFTopology> topology;
    com_ptr<IMFPresentationDescriptor> source_pd;
    DWORD source_streams;

    CHECK_HR(MFCreateMediaSession(nullptr, &_session));
    CHECK_HR(MFCreateSourceResolver(&source_resolver));
    CHECK_HR(source_resolver->CreateObjectFromURL(
        url, MF_RESOLUTION_MEDIASOURCE, nullptr, &object_type, &source));
    CHECK_HR(source.QueryInterface(IID_PPV_ARGS(&_source)));

    CHECK_HR(MFCreateTopology(&topology));
    CHECK_HR(_source->CreatePresentationDescriptor(&source_pd));
    CHECK_HR(source_pd->GetStreamDescriptorCount(&source_streams));

    for (DWORD i = 0; i < source_streams; i++) {
        com_ptr<IMFStreamDescriptor> source_sd;
        com_ptr<IMFTopologyNode> source_node;
        com_ptr<IMFTopologyNode> output_node;
        BOOL selected = FALSE;

        CHECK_HR(source_pd->GetStreamDescriptorByIndex(i, &selected, &source_sd));
        if (selected) {
            CHECK_HR(MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &source_node));
            CHECK_HR(source_node->SetUnknown(MF_TOPONODE_SOURCE, _source));
            CHECK_HR(source_node->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, source_pd));
            CHECK_HR(source_node->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, source_sd));

            com_ptr<IMFMediaTypeHandler> handler;
            com_ptr<IMFActivate> renderer_activate;
            GUID major_type;

            CHECK_HR(source_sd->GetMediaTypeHandler(&handler));
            CHECK_HR(handler->GetMajorType(&major_type));
            CHECK_HR(MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &output_node));
            if (major_type == MFMediaType_Audio) {
                CHECK_HR(MFCreateAudioRendererActivate(&renderer_activate));
            } else if (major_type == MFMediaType_Video) {
                CHECK_HR(MFCreateVideoRendererActivate(hwnd_video, &renderer_activate));
            } else {
                return E_FAIL;
            }
            CHECK_HR(output_node->SetObject(renderer_activate));

            CHECK_HR(topology->AddNode(source_node));
            CHECK_HR(topology->AddNode(output_node));
            CHECK_HR(source_node->ConnectOutput(0, output_node, 0));
        }
    }
    CHECK_HR(_session->SetTopology(0, topology));
    _state = player_state::open_pending;

    return S_OK;
}

mfplay_impl::~mfplay_impl()
{
    if (_session != nullptr) {
        _session->Close();
    }
    if (_close_event != nullptr) {
        WaitForSingleObject(_close_event.get(), 5000);
    }
    if (_source != nullptr) {
        _source->Shutdown();
    }
    if (_session != nullptr) {
        _session->Shutdown();
    }
}

HRESULT mfplay_impl::create_instance(const wchar_t* url, HWND hwnd_video, HWND hwnd_event, mfplay** ret)
try {
    CHECK_POINTER(url);
    CHECK_POINTER(ret);
    auto p = new mfplay_impl(hwnd_event);
    auto p2 = com_ptr<mfplay>(p);
    CHECK_HR(p->initialize(url, hwnd_video));
    CHECK_HR(p->QueryInterface(IID_PPV_ARGS(ret)));
    return S_OK;
} catch (const std::bad_alloc&) {
    return E_OUTOFMEMORY;
}

HRESULT STDMETHODCALLTYPE mfplay_impl::QueryInterface(
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

ULONG STDMETHODCALLTYPE mfplay_impl::AddRef(void)
{
    int32_t count = ++_ref_count;
    if (count == 1) {
        _shared_this = weak_from_this().lock();
    }
    return count;
}

ULONG STDMETHODCALLTYPE mfplay_impl::Release(void)
{
    int32_t count = --_ref_count;
    if (count == 0) {
        _shared_this = nullptr;
    }
    return count;
}

HRESULT STDMETHODCALLTYPE mfplay_impl::GetParameters(
    /* [out] */ __RPC__out DWORD* pdwFlags,
    /* [out] */ __RPC__out DWORD* pdwQueue)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE mfplay_impl::Invoke(
    /* [in] */ __RPC__in_opt IMFAsyncResult* pAsyncResult)
{
    MediaEventType event_type = MEUnknown; // Event type
    com_ptr<IMFMediaEvent> event;

    CHECK_HR(_session->EndGetEvent(pAsyncResult, &event));
    CHECK_HR(event->GetType(&event_type));
    if (event_type == MESessionClosed) {
        SetEvent(_close_event.get());
    } else {
        CHECK_HR(_session->BeginGetEvent(this, nullptr));
    }

    if (_state != player_state::closing) {
        event.AddRef();
        PostMessageW(_hwnd_event, WM_APP_PLAYER_EVENT, reinterpret_cast<IUnknown*>(event), 0);
    }

    return S_OK;
}
