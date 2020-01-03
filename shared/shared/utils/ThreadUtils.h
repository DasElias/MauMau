#pragma once
#include <functional>
#include "TimeUtils.h"
#include "ThreadUtilsProvider.h"

namespace card {
	void threadUtils_setProvider(std::unique_ptr<ThreadUtilsProvider> provider);
	void threadUtils_update();
	void threadUtils_invokeIn(int delay, std::function<void(void)> callback);
}