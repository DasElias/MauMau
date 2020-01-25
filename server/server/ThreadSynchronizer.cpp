#include "ThreadSynchronizer.h"

namespace card {
	std::mutex& threadSyncronizer_getMutex() {
		static std::mutex mutex;
		return mutex;
	}

}