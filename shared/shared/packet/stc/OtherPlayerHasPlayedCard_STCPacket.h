#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class OtherPlayerHasPlayedCard_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 203;
		
		private:
			static std::string const CARD_NUMBER_KEY;
			static std::string const NEW_CARD_INDEX_KEY;
			static std::string const CARDS_TO_DRAW_KEY;
			static std::string const WAS_DRAWN_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int cardNumber;
			int newCardIndex;
			std::vector<int> cardsToDraw;
			bool wasDrawnBeforePlayed_field;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			OtherPlayerHasPlayedCard_STCPacket(std::string username, int cardNumber, int newCardIndex, std::vector<int> cardsToDraw, bool wasDrawnBeforePlayed);
			OtherPlayerHasPlayedCard_STCPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			int getCardNumber() const;
			int getNewCardIndex() const;
			std::vector<int> getCardsToDraw() const;
			bool wasDrawnBeforePlayed() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}