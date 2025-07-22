#pragma once
#include "Signal.h"
#include "defines.h"
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

namespace rendell_ui {
class Timer final {
public:
    Timer(size_t interval = 1000);
    ~Timer();

    bool isRunning() const;
    bool isRepeat() const;
    size_t getInterval() const;

    void setInterval(size_t ms);
    void setIsRepeat(bool value);
    void start(size_t ms = 0);
    void stop();

    Action triggered;

private:
    std::atomic<bool> _isRunning{false};
    std::atomic<bool> _isRepeat{true};
    std::atomic<size_t> _interval{1000};

    std::thread _thread;
    std::mutex _mutex;
    std::condition_variable _cond_var;
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(Timer)
} // namespace rendell_ui