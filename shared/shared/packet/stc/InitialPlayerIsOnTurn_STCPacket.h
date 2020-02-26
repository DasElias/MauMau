#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class InitialPlayerIsOnTurn_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 207;

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
			InitialPlayerIsOnTurn_STCPacket(std::string username, int nextCardOnDrawStack);
			InitialPlayerIsOnTurn_STCPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			int getNextCardOnDrawStack() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;
	};
}