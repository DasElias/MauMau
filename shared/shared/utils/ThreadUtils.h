#pragma once
#include <functional>
#include "TimeUtils.h"
#include "ThreadUtilsProvider.h"
#include <memory>

namespace card {
	void threadUtils_setProvider(std::unique_ptr<ThreadUtilsProvider> provider);
	void threadUtils_update();
	void threadUtils_invokeIn(int delay, std::function<void(void)> callback);
	void threadUtils_invokeIn(int delay, const void* key, std::function<void(void)> callback);
	void threadUtils_removeCallbacksWithKey(const void* key);

}