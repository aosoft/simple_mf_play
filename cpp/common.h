#pragma once

#include <comdef.h>
#include <memory>

template <class Intf>
using com_ptr = _com_ptr_t<_com_IIID<Intf, &__uuidof(Intf)>>;

#define CHECK_HR(hr)  \
    if (FAILED(hr)) { \
        return hr;    \
    }

#define CHECK_POINTER(p) CHECK_HR(p != nullptr ? S_OK : E_POINTER);

struct handle_deleter {
    void operator()(HANDLE p) const
    {
        CloseHandle(p);
    }
};

using unique_handle = std::unique_ptr<std::remove_pointer<HANDLE>::type, handle_deleter>;