#include "mfplay_impl.h"

ATL::CAtlWinModule _Module;

HRESULT STDMETHODCALLTYPE mfplay_initialize()
{
    CHECK_HR(MFStartup(MF_VERSION, 0));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE mfplay_finalize()
{
    CHECK_HR(MFShutdown());
    _Module.Term();
    return S_OK;
}
