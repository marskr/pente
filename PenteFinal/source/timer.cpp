#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>

#include "../include/timer.h"

std::chrono::time_point <std::chrono::system_clock> Timer::Measure_Start() {
	std::chrono::time_point <std::chrono::system_clock> start;
	start = std::chrono::system_clock::now();
	return start;
}

std::chrono::time_point <std::chrono::system_clock> Timer::Measure_Stop() {
	std::chrono::time_point <std::chrono::system_clock> stop;
	stop = std::chrono::system_clock::now();
	return stop;
}

void Timer::Measure_screen(std::chrono::time_point <std::chrono::system_clock> start,
	std::chrono::time_point <std::chrono::system_clock> stop) {
	std::chrono::duration <double> result = stop - start;
	std::ofstream fout;
	fout.open("FILENAME.txt", std::ios::app);
	fout << stop.time_since_epoch().count() - start.time_since_epoch().count() << std::endl;
	fout.close();
	//std::cout << "The result of measure between START and STOP is: " << result.count() << std::endl;
}

/*
std::chrono::time_point <std::chrono::system_clock> start = Timer::instance()->Measure_Start();
std::chrono::time_point <std::chrono::system_clock> stop = Timer::instance()->Measure_Stop();
Timer::instance()->Measure_screen(start, stop);
*/
