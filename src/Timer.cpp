#include <rendell_ui/Timer.h>
#include <chrono>
#include <rendell_ui/rendell_ui.h>
#include <iostream>

namespace rendell_ui
{
	Timer::Timer(size_t interval) : _interval(interval)
	{

	}

	Timer::~Timer()
	{
		stop();
	}

	bool Timer::isRunning() const
	{
		return _isRunning;
	}

	bool Timer::isRepeat() const
	{
		return _isRepeat;
	}

	size_t Timer::getInterval() const
	{
		return _interval;
	}

	void Timer::setInterval(size_t ms)
	{
		_interval = ms;
	}

	void Timer::setIsRepeat(bool value)
	{
		_isRepeat = value;
	}

	void Timer::start(size_t ms)
	{
		if (_isRunning)
		{
			return;
		}

		if (ms > 0)
		{
			_interval = ms;
		}

		if (_thread.joinable())
		{
			return;
		}

		_isRunning = true;
		_thread = std::thread([this]()
			{
				std::unique_lock<std::mutex> lock(_mutex);

				while (_isRunning && _interval > 0)
				{
					_cond_var.wait_for(lock, std::chrono::milliseconds(_interval), [this]()
						{
							const bool result = _isRunning;
							return !result;
						});

					if (_isRunning)
					{
						callInMainThread([this]() { triggered.emit(); });
					}
					if (!_isRepeat)
					{
						break;
					}
				}
				_isRunning = false;
			});
	}

	void Timer::stop()
	{
		if (_isRunning)
		{
			_isRunning = false;
			_cond_var.notify_all();
			if (_thread.joinable())
			{
				_thread.join();
			}
		}
	}
}