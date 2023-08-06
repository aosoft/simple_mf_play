#include "main_window.h"

main_window::main_window() { }
main_window::~main_window() { }

void main_window::OnFinalMessage(HWND)
{
    _self = nullptr;
}

LRESULT main_window::OnCreate(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    _self = shared_from_this();
    bHandled = TRUE;
    return 0;
}

LRESULT main_window::OnClose(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ::PostQuitMessage(0);
    bHandled = TRUE;
    return 0;
}

LRESULT main_window::OnDestroy(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
    return 0;
}
