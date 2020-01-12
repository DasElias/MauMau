#include "ParticipantOnClient.h"

namespace card {
	ParticipantOnClient::ParticipantOnClient(std::string username, Avatar avatar) :
			username(username),
			avatar(avatar) {
	}

	std::string ParticipantOnClient::getUsername() const {
		return username;
	}

	Avatar ParticipantOnClient::getAvatar() const {
		return avatar;
	}

}
