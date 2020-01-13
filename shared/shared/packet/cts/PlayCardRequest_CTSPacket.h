#pragma once
#include "ClientToServerPacket.h"

namespace card {
	class PlayCardRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 500;

		private:
			static std::string const CARD_NUMBER_KEY;
			static std::string const NEW_CARD_INDEX_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int cardNumber;
			int newCardIndex;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			PlayCardRequest_CTSPacket(int cardNumber, int newCardIndex = 0);
			PlayCardRequest_CTSPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			int getCardNumber() const;
			int getNewCardIndex() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;
	};
}