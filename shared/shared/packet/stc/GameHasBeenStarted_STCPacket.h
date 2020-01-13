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
			static std::string const USERNAME_ON_TURN_KEY;
			static std::string const HAND_CARDS_KEY;
			static std::string const START_CARD_KEY;
			static std::string const NEXT_CARD_ON_DRAW_STCK;


		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::vector<std::string> allParticipants;
			std::string usernameOnTurn;
			std::vector<int> handCards;
			int startCard;
			int nextCardOnDrawStack;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			GameHasBeenStarted_STCPacket(std::vector<std::string> allParticipants, std::string usernameOnTurn, std::vector<int> handCards, int startCard, int nextCardOnDrawStack);
			GameHasBeenStarted_STCPacket(nlohmann::json& json);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::vector<std::string> getUsernamesOfAllParticipants() const;
			std::string getUsernameOnTurn() const;
			std::vector<int> getHandCards() const;
			int getStartCard() const;
			int getNextCardOnDrawStack() const;
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}