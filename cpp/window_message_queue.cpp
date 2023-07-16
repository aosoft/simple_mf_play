#include "window_message_queue.h"

window_message_queue::window_message_queue()
    : _hwnd(nullptr)
{
}

window_message_queue::~window_message_queue() noexcept
{
    detach();
}

void window_message_queue::attach(HWND hwnd)
{
    if (_hwnd != nullptr) {
        return;
    }
    if (SubclassWindow(hwnd)) {
        _hwnd = hwnd;
    }
}

void window_message_queue::detach()
{
    if (_hwnd == nullptr) {
        return;
    }
    UnsubclassWindow();
}

void window_message_queue::push(std::function<void()> fn)
{
    {
        std::lock_guard lock(_mutex);
        _queue.push(fn);
    }
    PostMessage(WM_APP_MESSAGE_EVENT, 0, 0);
}

LRESULT window_message_queue::OnMessage(UINT msg, WPARAM wparam, LPARAM lparam, BOOL& handled)
{
    std::function<void()> fn;
    {
        std::lock_guard lock(_mutex);
        fn = _queue.front();
        _queue.pop();
    }

    fn();

    handled = TRUE;
    return 0;
}