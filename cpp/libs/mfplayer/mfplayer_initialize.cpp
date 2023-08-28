#include "mfplayer_impl.h"

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "evr.lib")
#pragma comment(lib, "dxva2.lib")

ATL::CAtlWinModule _Module;

hresult_t mfplayer_initialize()
{
    CHECK_HR(MFStartup(MF_VERSION, 0));
    return S_OK;
}

hresult_t mfplayer_finalize()
{
    CHECK_HR(MFShutdown());
    _Module.Term();
    return S_OK;
}
