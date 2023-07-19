#include "mfplay_impl.h"

hresult_t create_mfplay(const wchar_t* url, hwnd_t hwnd_video, mfplay** ret)
{
    return mfplay_impl::create_instance(url, reinterpret_cast<HWND>(hwnd_video), ret);
}
