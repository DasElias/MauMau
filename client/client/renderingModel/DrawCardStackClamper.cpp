#include "DrawCardStackClamper.h"

namespace card {
	std::size_t DrawCardStackClamper::getClampedSize(const CardAnimator& ca) {
		if(ca.getSize() > Card::MAX_CARDS) return Card::MAX_CARDS;
		return ca.getSize();
	}
}