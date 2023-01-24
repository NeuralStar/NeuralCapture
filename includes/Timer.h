#pragma once
#include <chrono>

template <
	class Delta = std::chrono::microseconds,
	class Clock = std::chrono::steady_clock
>
class Timer
{
	public:
		typedef Clock						clock_type;
		typedef typename Clock::time_point	timestamp_type;
		typedef Delta						delta_type;

	private:
		timestamp_type	_start;
		timestamp_type	_end;

	public:
		Timer() { start(); }
		~Timer() {}

		void	start()
		{
			_end = clock_type::now();
			_start = clock_type::now();
		}

		void	finish()
		{
			_end = clock_type::now();
		}

		template < class T = Delta >
		long long duration() const
		{
			return std::chrono::duration_cast<T>(_end - _start).count();
		}
};