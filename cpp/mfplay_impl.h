#pragma once

#include "mfplay.h"

#include <windows.h>
#include <comdef.h>
#include <mfapi.h>
#include <mfobjects.h>
#include <mfidl.h>
#include <mferror.h>
#include <evr.h>

#include <memory>

template<class Intf>
using com_ptr = _com_ptr_t<_com_IIID<Intf, &__uuidof(Intf)>>;

#define CHECK_HR(hr) if (FAILED(hr)) { return hr; }

class mfplay_impl : public mfplay {
private:
    com_ptr<IMFMediaSession> _session;
    com_ptr<IMFMediaSource> _source;
    com_ptr<IMFVideoDisplayControl> _video_display;
    HANDLE _close_event;

public:

private:
    mfplay_impl();
    HRESULT initialize();
};