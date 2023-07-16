#include "window_message_queue.h"

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