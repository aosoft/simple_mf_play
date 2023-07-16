#pragma once

#define WIN32_LEAN_AND_MEAN
#define _ATL_NO_AUTOMATIC_NAMESPACE

#include <wrl/client.h>
#include <memory>

#include <atlbase.h>
extern ATL::CAtlWinModule _Module;
#include <atlwin.h>

template <class Intf>
using com_ptr = Microsoft::WRL::ComPtr<Intf>;

#define CHECK_HR(hr)         \
    {                        \
        HRESULT hrtmp = hr;  \
        if (FAILED(hrtmp)) { \
            return hrtmp;    \
        }                    \
    }

#define CHECK_POINTER(p) CHECK_HR(p != nullptr ? S_OK : E_POINTER);

struct handle_deleter {
    void operator()(HANDLE p) const
    {
        CloseHandle(p);
    }
};

using unique_handle = std::unique_ptr<std::remove_pointer<HANDLE>::type, handle_deleter>;