#pragma once
#include "MessageQueue.h"
#include <shared/model/MauPunishmentCause.h>

namespace card {
	namespace MauMauPunishmentMessageGenerator {
		std::string generateMessage(std::string punishedUsername, MauPunishmentCause cause);
		void appendMessage(MessageQueue& messageQueue, std::string punishedUsername, MauPunishmentCause cause);
	}
}