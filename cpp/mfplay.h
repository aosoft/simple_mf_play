#pragma once

#include <windows.h>
#include <comdef.h>
#include <mfapi.h>
#include <mfobjects.h>
#include <mfidl.h>
#include <mferror.h>

template<class Intf>
using com_ptr = _com_ptr_t<_com_IIID<Intf, &__uuidof(Intf)>>;
