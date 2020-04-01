#include "AbstractConnectionToClientDummy.h"

namespace card {
	void AbstractConnectionToClientDummy::send(std::string message) {
		// empty method body
	}
	void AbstractConnectionToClientDummy::close() {
		wasClosed_field = true;
	}
	bool AbstractConnectionToClientDummy::wasClosed() const {
		return wasClosed_field;
	}
}