#pragma once
#include "../model/CardAnimator.h"

namespace card {
	namespace DrawCardStackClamper {
		std::size_t getClampedSize(const CardAnimator& ca);
	}
}