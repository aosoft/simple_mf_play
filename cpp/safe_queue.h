#pragma once

#include <queue>
#include <mutex>

template <typename data_type>
class safe_queue {
private:
    std::mutex _mutex;
    std::queue<data_type> _queue;

public:
    void push(const data_type& data)
    {
        std::lock_guard lock(_mutex);
        _queue.push(data);
    }

    void push(data_type&& data)
    {
        std::lock_guard lock(_mutex);
        _queue.push(data);
    }

    data_type pop()
    {
        std::lock_guard lock(_mutex);
        auto r = _queue.front();
        _queue.pop();
        return r;
    }
};