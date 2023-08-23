#pragma once

#include "common.h"
#include "mfplayer.h"
#include "async_callback.h"
#include "window_message_queue.h"

#include <atomic>

#include <evr.h>
#include <mfapi.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfobjects.h>

class mfplayer_impl : public mfplayer,
                    public std::enable_shared_from_this<mfplayer_impl> {
private:
    std::atomic_int32_t _ref_count;
    std::shared_ptr<mfplayer_impl> _shared_this;

    player_state _state;
    std::shared_ptr<async_callback> _callback;
    window_message_queue _queue;
    com_ptr<IMFMediaSession> _session;
    com_ptr<IMFMediaSource> _source;
    com_ptr<IMFVideoDisplayControl> _video_display;
    unique_handle _close_event;

private:
    mfplayer_impl();
    HRESULT initialize(const wchar_t* url, HWND hwnd_video);

public:
    virtual ~mfplayer_impl();
    static HRESULT create_instance(const wchar_t* url, HWND hwnd_video, mfplayer** ret);

private:
    HRESULT on_event_callback(com_ptr<IMFMediaEvent> event);

public:
    virtual void dispose() override;
    virtual hresult_t play() override;
    virtual hresult_t pause() override;
    virtual hresult_t repaint() override;
    virtual hresult_t resize_video(std::int32_t width, std::int32_t height) override;

private:
    HRESULT start_playback();
};