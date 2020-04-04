#include "../Packet.h"

namespace card {
	class TurnWasAborted_STCPacket : public Packet {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 204;

		private:
			static std::string const CARDS_TO_DRAW_KEY;
			static std::string const AMOUNT_OF_CARDS_TO_DRAW_BEFORE_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::vector<int> cardsToDraw;
			int amountOfCardsToDrawBefore;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			TurnWasAborted_STCPacket(std::vector<int>& cardsToDraw, int amountOfCardsToDrawBefore);
			TurnWasAborted_STCPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::vector<int> getCardsToDraw() const;
			int getAmountOfCardsToDrawBefore() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}