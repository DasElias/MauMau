#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class LocalPlayerIsOnTurn_STCPacket : public Packet {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 200;

		private:
			static std::string const NEXT_CARD_ON_DRAW_STCK;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int nextCardOnDrawStack;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LocalPlayerIsOnTurn_STCPacket(int nextCardOnDrawStack);
			LocalPlayerIsOnTurn_STCPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			int getNextCardOnDrawStack() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;
	};
}