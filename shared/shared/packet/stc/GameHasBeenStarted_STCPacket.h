#pragma once
#include "../Packet.h"
#include <vector>
#include <nlohmann/json.hpp>

namespace card {
	class GameHasBeenStarted_STCPacket : public Packet {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 105;

		private:
			static std::string const ALL_PARTICIPANTS_KEY;
			static std::string const HAND_CARDS_KEY;
			static std::string const START_CARD_KEY;


		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::vector<std::string> allParticipants;
			std::vector<int> handCards;
			int startCard;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			GameHasBeenStarted_STCPacket(std::vector<std::string> allParticipants, std::vector<int> handCards, int startCard);
			GameHasBeenStarted_STCPacket(nlohmann::json& json);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::vector<std::string> getUsernamesOfAllParticipants() const;
			std::vector<int> getHandCards() const;
			int getStartCard() const;
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}