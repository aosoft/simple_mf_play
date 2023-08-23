#include "mfplayer_impl.h"
#include <exception>

mfplayer_impl::mfplayer_impl()
    : _state(player_state::closed)
{
}

HRESULT mfplayer_impl::initialize(const wchar_t* url, HWND hwnd_video)
{
    HRESULT hr = S_OK;

    if (!_queue.attach(hwnd_video)) {
        return E_FAIL;
    }

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

    _callback = std::make_shared<async_callback>(_session.Get(), [self = weak_from_this()](com_ptr<IMFMediaEvent> event) -> HRESULT {
        auto self2 = self.lock();
        if (self2 != nullptr) {
            if (self2->_state != player_state::closing) {
                self2->_queue.push([self, event]() {
                    auto self2 = self.lock();
                    if (self2 != nullptr) {
                        self2->on_event_callback(event);
                    }
                });
            }
        }
        return S_OK;
    });
    CHECK_HR(_session->BeginGetEvent(_callback.get(), nullptr));

    CHECK_HR(MFCreateSourceResolver(&source_resolver));
    CHECK_HR(source_resolver->CreateObjectFromURL(
        url, MF_RESOLUTION_MEDIASOURCE, nullptr, &object_type, &source));
    CHECK_HR(source.As(&_source));

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
            CHECK_HR(source_node->SetUnknown(MF_TOPONODE_SOURCE, _source.Get()));
            CHECK_HR(source_node->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, source_pd.Get()));
            CHECK_HR(source_node->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, source_sd.Get()));

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
            CHECK_HR(output_node->SetObject(renderer_activate.Get()));

            CHECK_HR(topology->AddNode(source_node.Get()));
            CHECK_HR(topology->AddNode(output_node.Get()));
            CHECK_HR(source_node->ConnectOutput(0, output_node.Get(), 0));
        }
    }
    CHECK_HR(_session->SetTopology(0, topology.Get()));
    _state = player_state::open_pending;

    return S_OK;
}

mfplayer_impl::~mfplayer_impl()
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

HRESULT mfplayer_impl::create_instance(const wchar_t* url, HWND hwnd_video, mfplayer** ret)
try {
    CHECK_POINTER(url);
    CHECK_POINTER(ret);

    struct mfplay_impl_ : public mfplayer_impl {
    };

    auto p = std::make_shared<mfplay_impl_>();
    CHECK_HR(p->initialize(url, hwnd_video));
    p->_shared_this = p->shared_from_this();
    *ret = p.get();
    return S_OK;
} catch (const std::bad_alloc&) {
    return E_OUTOFMEMORY;
}

HRESULT mfplayer_impl::on_event_callback(com_ptr<IMFMediaEvent> event)
{
    MediaEventType event_type = MEUnknown;
    HRESULT status;
    MF_TOPOSTATUS topo_status = MF_TOPOSTATUS_INVALID;

    CHECK_HR(event->GetType(&event_type));
    CHECK_HR(event->GetStatus(&status));
    if (SUCCEEDED(status)) {
        switch (event_type) {
        case MESessionTopologyStatus:
            CHECK_HR(event->GetUINT32(
                MF_EVENT_TOPOLOGY_STATUS, reinterpret_cast<UINT32*>(&topo_status)));
            switch (topo_status) {
            case MF_TOPOSTATUS_READY:
                MFGetService(
                    _session.Get(), MR_VIDEO_RENDER_SERVICE, IID_PPV_ARGS(&_video_display));
                CHECK_HR(start_playback());
                break;
            default:
                break;
            }
        case MEEndOfPresentation:
            _state = player_state::stopped;
            break;
        }
    }
    return S_OK;
}

void mfplayer_impl::dispose()
{
    _shared_this = nullptr;
}

hresult_t mfplayer_impl::play()
{
    if (_state == player_state::paused || _state == player_state::stopped) {
        CHECK_HR(start_playback());
    }
    return S_OK;
}

hresult_t mfplayer_impl::pause()
{
    if (_state == player_state::started) {
        CHECK_HR(_session->Pause());
        _state = player_state::paused;
    }
    return S_OK;
}

hresult_t mfplayer_impl::repaint()
{
    return _video_display != nullptr ? _video_display->RepaintVideo() : S_OK;
}

hresult_t mfplayer_impl::resize_video(std::int32_t width, std::int32_t height)
{
    if (_video_display == nullptr) {
        return S_OK;
    }
    RECT rect = { 0, 0, width, height };
    return _video_display->SetVideoPosition(nullptr, &rect);
}

HRESULT mfplayer_impl::start_playback()
{
    PROPVARIANT varStart;
    PropVariantInit(&varStart);

    varStart.vt = VT_EMPTY;
    HRESULT hr = _session->Start(&GUID_NULL, &varStart);
    PropVariantClear(&varStart);
    CHECK_HR(hr);
    _state = player_state::started;

    return hr;
}
