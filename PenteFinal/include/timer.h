#pragma once

#ifndef TIMER_CPP
#define TIMER_CPP

class Timer {
public:
	static Timer *s_instance;
	Timer() { }
	static Timer *instance() {
		if (!s_instance)
		{
			s_instance = new Timer;
		}

		return s_instance;
	}
	std::chrono::time_point <std::chrono::system_clock> Measure_Start();
	std::chrono::time_point <std::chrono::system_clock> Measure_Stop();
	void Measure_screen(std::chrono::time_point <std::chrono::system_clock> start, std::chrono::time_point <std::chrono::system_clock> stop);
};

#endif // !TIMER_CPP
