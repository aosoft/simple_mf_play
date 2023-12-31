#include "window_message_queue.h"

window_message_queue::window_message_queue() = default;

window_message_queue::~window_message_queue() noexcept
{
    detach();
}

bool window_message_queue::attach(HWND hwnd)
{
    if (m_hWnd != nullptr) {
        return hwnd == m_hWnd;
    }
    if (SubclassWindow(hwnd)) {
        return true;
    }
    return false;
}

void window_message_queue::detach()
{
    if (m_hWnd == nullptr) {
        return;
    }
    UnsubclassWindow();
}

void window_message_queue::push(const std::function<void()>& fn)
{
    {
        std::lock_guard lock(_mutex);
        _queue.push(fn);
    }
    PostMessage(WM_APP_MESSAGE_EVENT, 0, 0);
}

LRESULT window_message_queue::OnMessage(UINT msg, WPARAM wparam, LPARAM lparam, BOOL& handled)
{
    if (msg == WM_APP_MESSAGE_EVENT) {
        std::function<void()> fn;
        {
            std::lock_guard lock(_mutex);
            fn = _queue.front();
            _queue.pop();
        }

        fn();
        handled = TRUE;
    }

    return 0;
}