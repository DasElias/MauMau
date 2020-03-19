#pragma once
#include "Player.h"

namespace card {
	namespace PlayerFactory {
		std::shared_ptr<Player> constructPlayer(const std::shared_ptr<ParticipantOnServer>& participant);
	}
}