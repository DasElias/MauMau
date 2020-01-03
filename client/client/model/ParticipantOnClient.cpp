#include "ParticipantOnClient.h"

namespace card {
	ParticipantOnClient::ParticipantOnClient(std::string username) :
			username(username) {
	}

	std::string ParticipantOnClient::getUsername() const {
		return username;
	}
}
