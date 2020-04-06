#include "MauMauPunishmentMessageGenerator.h"
#include <shared/utils/Logger.h>

namespace card {
	std::string MauMauPunishmentMessageGenerator::generateMessage(std::string punishedUsername, MauPunishmentCause cause) {
		switch(cause) {
			case MauPunishmentCause::TOO_EARLY:
				return punishedUsername + u8" rief zu früh \"Mau!\"";
			case MauPunishmentCause::NO_MAU_RECEIVED:
				return punishedUsername + u8" vergaß, \"Mau!\" zu rufen!";
				break;
			case MauPunishmentCause::DRAWED_EVEN_THOUGH_MAUED:
				return punishedUsername + " spielte keine Karte, obwohl er \"Mau!\" rief.";
			default:
				log(LogSeverity::WARNING, "Unknown MauPunishmentCause: " + std::to_string(static_cast<int>(cause)));
		}
		return "";
	}
	void MauMauPunishmentMessageGenerator::appendMessage(MessageQueue& messageQueue, std::string punishedUsername, MauPunishmentCause cause) {
		std::string message = generateMessage(punishedUsername, cause);
		messageQueue.appendMessage(message);
	}
}