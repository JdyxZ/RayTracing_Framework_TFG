#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <iostream>

using time_point = optional<std::chrono::steady_clock::time_point>;

struct elapsed_time
{
    int hours;
    int minutes;
    int seconds;
    int milliseconds;
};

class benchmark {
public:

	benchmark() 
    {
		start_time = nullopt;
		end_time = nullopt;
    }

    // Start measuring time
	void start() 
    {
		start_time = std::chrono::high_resolution_clock::now();
	}

    void end()
    {
		end_time = std::chrono::high_resolution_clock::now();
    }

	void print_elapsed()
	{
		if (!start_time.has_value())
		{
			std::cerr << "Error: Benchmark not started" << std::endl;
			return;
		}
        
		if (!end_time.has_value())
		{
			std::cerr << "Error: Benchmark not ended" << std::endl;
			return;
		}

		auto et = elapsed();

		std::cout << "Elapsed time: ";

		if (et.hours > 0)
			std::cout << et.hours << "h ";

		if (et.minutes > 0) 
			std::cout << et.minutes << "m ";

		if (et.seconds > 0) 
			std::cout << et.seconds << "s ";

		if (et.milliseconds > 0) 
			std::cout << et.milliseconds << "ms";

		std::cout << std::endl;
	}

private:
    time_point start_time;
    time_point end_time;

    elapsed_time elapsed()
    {
        // Calculate elapsed time
        auto elapsed = end_time.value() - start_time.value();

        // Get total seconds and then convert to more granular units
        auto total_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
        auto total_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

        // Create new elapsed_time struct
        elapsed_time et;

        // Extract days, hours, minutes, and seconds (all integer divisions)
        et.hours = int(total_seconds / (60 * 60));
        et.minutes = int((total_seconds % (60 * 60)) / 60);
        et.seconds = int(total_seconds % 60);
        et.milliseconds = int(total_milliseconds % 1000);

        return et;
    }
};

#endif