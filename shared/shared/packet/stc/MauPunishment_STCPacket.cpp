#include "MauPunishment_STCPacket.h"

namespace card {
	std::string const MauPunishment_STCPacket::CARDS_TO_DRAW_KEY = "25A";
	std::string const MauPunishment_STCPacket::PUNISHMENT_CAUSE_KEY = "25B";
	std::string const MauPunishment_STCPacket::CONCERNED_USERNAME_KEY = "25C";

	MauPunishment_STCPacket::MauPunishment_STCPacket(std::string concernedUsername, std::vector<int>& cardsToDraw, MauPunishmentCause punishmentCause) :
			Packet(PACKET_ID),
			concernedUsername(concernedUsername),
			cardsToDraw(cardsToDraw),
			cause(punishmentCause) {
	}
	MauPunishment_STCPacket::MauPunishment_STCPacket(nlohmann::json& jsonHandle) :
			Packet(PACKET_ID),
			concernedUsername(jsonHandle[CONCERNED_USERNAME_KEY]),
			cardsToDraw(jsonHandle.at(CARDS_TO_DRAW_KEY).get<std::vector<int>>()),
			cause(jsonHandle[PUNISHMENT_CAUSE_KEY]) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
	std::vector<int> MauPunishment_STCPacket::getCardsToDraw() const {
		return cardsToDraw;
	}
	MauPunishmentCause MauPunishment_STCPacket::getCause() const {
		return cause;
	}
	std::string MauPunishment_STCPacket::getConcernedUsername() const {
		return concernedUsername;
	}
	void MauPunishment_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);
		jsonHandle[CONCERNED_USERNAME_KEY] = concernedUsername;
		jsonHandle[CARDS_TO_DRAW_KEY] = cardsToDraw;
		jsonHandle[PUNISHMENT_CAUSE_KEY] = cause;
	} 
}