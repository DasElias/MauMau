#include "Cleanable.h"
#include <stdexcept>

namespace card {
	bool Cleanable::isAlreadyCleaned() const {
		return field_isAlreadyCleaned;
	}

	void Cleanable::cleanUp() {
		if(field_isAlreadyCleaned) throw std::runtime_error("Object has already been cleaned up!");

		this->field_isAlreadyCleaned = true;
	}

}

