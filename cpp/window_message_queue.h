#pragma once

#include "common.h"
#include <queue>
#include <mutex>
#include <functional>
#include <atlwin.h>

class window_message_queue : ATL::CWindowImpl<window_message_queue> {
private:
    const UINT WM_APP_MESSAGE_EVENT = WM_APP + 1;

    std::mutex _mutex;
    std::queue<std::function<void()>> _queue;

public:
    void push(std::function<void()> fn);

public:
    BEGIN_MSG_MAP(window_message_queue)
    MESSAGE_HANDLER(WM_APP_MESSAGE_EVENT, OnMessage)
    END_MSG_MAP()

private:
    LRESULT OnMessage(UINT msg, WPARAM wparam, LPARAM lparam, BOOL& handled);
};