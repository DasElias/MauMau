#pragma once
#include <mutex>

namespace card {
	std::mutex& threadSyncronizer_getMutex();
}