#include "ParticipantOnClient.h"

namespace card {
	ParticipantOnClient::ParticipantOnClient(std::string username, Avatar avatar, bool isAiPlayer) :
			username(username),
			avatar(avatar),
			isAiPlayer_field(isAiPlayer) {
	}

	std::string ParticipantOnClient::getUsername() const {
		return username;
	}

	Avatar ParticipantOnClient::getAvatar() const {
		return avatar;
	}

	bool ParticipantOnClient::isAiPlayer() const {
		return isAiPlayer_field;
	}

}
