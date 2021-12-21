#pragma once
/* Simple Instrumentation class
 * This class can be used one of two ways.
 * 
 * As a stand alone instrumentation class typically used inside a section of code to print 
 * out time required at each call.
 * 
 * As a global map of timers that contains a count of calls and used as a simple method
 * to profile the code.
 * 
 * With this method you call NewCounter with a named string. The first time that
 * NewCounter is called the counter is created and added to the unordered_map "timers"
 * All subsequant calls will return the old counter name.
 * 
 * Since many times I wish to turn the summary counts off and on at compile time I added
 * A bunch of helper macros, that will add calls to the instrumentation class if 
 * INSTRUMENT is defined as 1
 * 
 */
#include <string>
#include <iostream>
#include <ostream>
#include <iomanip>
#include <chrono>
#include <atomic>
#include <unordered_map>

using namespace std::chrono;

// Whether to turn on Instrumentation - For now it is set to 1.  Eventually this will be
// added as a compilation flag.
#define INSTRUMENT 1

class Instrumentation
{
private:
	static std::unordered_map<std::string, Instrumentation> timers;
	static std::atomic<double> instrument_time;
	high_resolution_clock::time_point st_time;
	high_resolution_clock::time_point end_time;
	std::atomic<double> total_time;
	std::atomic<long> count = 0;
public:
	Instrumentation() :total_time(0.0), st_time(high_resolution_clock::now()) {}
	~Instrumentation() {}
	Instrumentation(Instrumentation& i) {
		st_time = i.st_time;
		end_time = i.end_time;
		total_time.store(i.total_time);
		count.store(i.count);
	}

	void start() {
		st_time = high_resolution_clock::now();
	}

	// Calculate the average amount of time used to start and and update the duration
	// over a certain number of runs, defaults to 25.
	static void calcInstrumentTime(unsigned averaged_over = 25) {
		using namespace std;
		Instrumentation t;

		// Set a sanity check of 80 nanosecond
		const double min_val = 1.0;		// set a sanity check of 1 nanoseconds
		const double max_val = 250.0;
		double sum{};
		for (unsigned i = 0; i < averaged_over; i++) {
			double val = max_val;
			t.start();
			t.updateDuration();

			duration<double, nano> tt = t.end_time - t.st_time;
			if (val > tt.count()) {
				val = tt.count();
				if (val == 0) val = min_val;
			}
			sum += val;
		}

		instrument_time = sum / averaged_over;
		cout << "instrument time = " << instrument_time << " nanoseconds" << endl;
	}

	// Factory method to create a list of Instruments for final display
	static Instrumentation& NewCounter(const std::string message) {
		using namespace std;
		return timers[message];
	}

	// Reads the current high resolution clock time and calculates 
	// the difference from the start time, and add the duration to 
	// the total time and updates the execution count.
	void updateDuration() {
		end_time = high_resolution_clock::now();
		duration<double, std::nano> dur = end_time - st_time;
		total_time += dur.count() - instrument_time;
		count++;
	}

	// Update duration from the result of another Instrumentation object.
	// Typically used inside a thread to update a counter defined in the 
	// unordered_map timers.
	void updateDuration(Instrumentation& t) {
		using namespace std;
		total_time += t.total_time;
		count++;
	}

	static void printResults() {
		using namespace std;
		cout << "Item                            Total Time                  # Runs     Average" << endl;
		cout << "------------------------------- --------------------------- ---------- ---------------------------" << endl;
		
		std::unordered_map<const std::string, Instrumentation>::iterator it = timers.begin();
		while (it != timers.end()) {
			Instrumentation& item = it->second;
			cout << left << setw(30) << it->first << ": " << it->second << endl;
			it++;
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Instrumentation& item);
};

std::ostream& operator<<(std::ostream& os, const Instrumentation& item);

// Counter macros
#if INSTRUMENT
/* NEW_COUNTER(name, description)
 * Create a reference to Instrumentation Variable and stores it in the unordered_map 
 * timers Used to create the counter in the .cpp file.
 * 
 * Parameters:
 *		name			Name of the variable
 *		description		Description of counter.  Used as a 'primary key' into the 
 *						unordered map
 */ 

#define NEW_COUNTER(name, description) \
Instrumentation &name = Instrumentation::NewCounter(description)

/* EXTERN_NEW_COUNTER(name, description)
 * Define the reference to an Instrumentation variable. Used to define the counter in 
 * a header
 *
 * Parameters:
 *		name			Name of the variable
 */

#define EXTERN_NEW_COUNTER(name) \
extern Instrumentation &name

/* THREAD_INSTR_START(name)
 * Creates a new local variable named t_<name>, typically in a method.  The macro name starts
 * woth THREAD so that I rememberto use this within a thread, instead of relying on a global 
 * thread variable.
 *
 * The name typically links to a previously declared NEW_COUNTER
 * Parameters:
 *		name			Name of the variable, which should match a "global name"
 */

#define THREAD_INSTR_START(name) Instrumentation t_##name

/* THREAD_INSTR_END(name)
 * Calculates the execution time in the local variable t_<name> and then updates the similarly
 * named "global" variable named "name"
 *
 * Parameters:
 *		name			Name of the variable, which should match a "global name"
 */

#define THREAD_INSTR_END(name) t_##name##.updateDuration(); name##.updateDuration(t_##name)

#define INSTR_START(name) name##.start()
#define INSTR_END(name) name##.updateDuration()

#define INSTR_NAME_START(name, descr) Instrumentation &t_##name = Instrumentation::NewCounter(descr); t_##name.start();
#define INSTR_NAME_END(name) t_##name.updateDuration()
#else
#define NEW_COUNTER(name, description)
#define EXTERN_NEW_COUNTER(name)
#define THREAD_INSTR_START(name)
#define THREAD_INSTR_END(name)
#define INSTR_START(name)
#define INSTR_END(name)
#define INSTR_NAME_START(name, descr)
#define INSTR_NAME_END(name, descr)
#endif
