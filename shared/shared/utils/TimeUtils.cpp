#include "TimeUtils.h"

#include <chrono>
namespace card {
	long long getMilliseconds() {
		namespace sc = std::chrono;

		auto time = sc::system_clock::now(); // get the current time

		auto since_epoch = time.time_since_epoch(); // get the duration since epoch

		// I don't know what system_clock returns
		// I think it's uint64_t nanoseconds since epoch
		// Either way this duration_cast will do the right thing
		auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);

		return millis.count();
	}
}


