#include "main_window.h"

main_window::main_window()
{
    _player = nullptr;
}

main_window::~main_window()
{
    if (_player != nullptr) {
        _player->dispose();
        _player = nullptr;
    }
}

void main_window::OnFinalMessage(HWND)
{
    _self = nullptr;
}

LRESULT main_window::OnCreate(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    _self = shared_from_this();

    HRESULT hr = create_mfplay(L"C:\\WORK\\Media\\Video\\test.mp4", m_hWnd, &_player);
    if (SUCCEEDED(hr)) {
        _player->play();
    }

    bHandled = TRUE;
    return 0;
}

LRESULT main_window::OnEraseBackground(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
    return 1;
}

LRESULT main_window::OnSize(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    RECT rect;
    if (GetClientRect(&rect)) {

        _player->resize_video(rect.right - rect.left, rect.bottom - rect.top);
    }

    bHandled = TRUE;
    return 0;
}

LRESULT main_window::OnPaint(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    _player->repaint();
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
