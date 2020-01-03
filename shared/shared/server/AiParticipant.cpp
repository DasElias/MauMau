#include "AiParticipant.h"

namespace card {
	AiParticipant::AiParticipant(std::string username) :
			ParticipantOnServer(username) {
	}

	bool AiParticipant::isRealPlayer() const {
		return false;
	}
}