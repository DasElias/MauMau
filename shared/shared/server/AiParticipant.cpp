#include "AiParticipant.h"

namespace card {
	AiParticipant::AiParticipant(std::string username, Avatar avatar) :
			ParticipantOnServer(username, avatar) {
	}

	bool AiParticipant::isRealPlayer() const {
		return false;
	}
}