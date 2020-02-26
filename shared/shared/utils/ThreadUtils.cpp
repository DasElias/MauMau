#include "ThreadUtils.h"
#include <vector>
#include <algorithm>
#include "DefaultThreadUtilsProvider.h"
#include <stdexcept>

namespace card {
	std::unique_ptr<ThreadUtilsProvider> provider = std::make_unique<DefaultThreadUtilsProvider>();

	void threadUtils_setProvider(std::unique_ptr<ThreadUtilsProvider> p) {
		if(! p) throw std::runtime_error("Can't set a nullptr-provider");
		provider = std::move(p);
	}

	void threadUtils_update() {
		provider->update();
	}
	void threadUtils_invokeIn(int delay, std::function<void(void)> callback) {
		static const int defaultKey = 0;
		provider->invokeIn(delay, &defaultKey, callback);
	}
	void threadUtils_invokeIn(int delay, const void* key, std::function<void(void)> callback) {
		provider->invokeIn(delay, key, callback);
	}
	void threadUtils_removeCallbacksWithKey(const void* key) {
		provider->removeCallbacks(key);
	}

}