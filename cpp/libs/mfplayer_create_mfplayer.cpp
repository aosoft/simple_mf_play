#include "mfplayer_impl.h"

hresult_t create_mfplayer(const wchar_t* url, hwnd_t hwnd_video, mfplayer** ret)
{
    return mfplayer_impl::create_instance(url, reinterpret_cast<HWND>(hwnd_video), ret);
}
