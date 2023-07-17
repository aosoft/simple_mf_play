#include "mfplay_impl.h"

HRESULT STDMETHODCALLTYPE create_mfplay(const wchar_t* url, HWND hwnd_video, mfplay** ret)
{
    return mfplay_impl::create_instance(url, hwnd_video, hwnd_video, ret);
}
