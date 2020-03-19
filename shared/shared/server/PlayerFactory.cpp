#include "PlayerFactory.h"
#include "AiPlayer.h"

namespace card {
	std::shared_ptr<Player> PlayerFactory::constructPlayer(const std::shared_ptr<ParticipantOnServer>& participant) {
		return (participant->isRealPlayer()) ? std::make_shared<Player>(p) : std::make_shared<AiPlayer>(p, *this);
	}
}


