#include "../Packet.h"
#include "../../model/MauPunishmentCause.h"
namespace card {
	class MauPunishment_STCPacket : public Packet {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 205;

		private:
			static std::string const CONCERNED_USERNAME_KEY;
			static std::string const CARDS_TO_DRAW_KEY;
			static std::string const PUNISHMENT_CAUSE_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::string concernedUsername;
			std::vector<int> cardsToDraw;
			MauPunishmentCause cause;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			MauPunishment_STCPacket(std::string concernedUsername, std::vector<int>& cardsToDraw, MauPunishmentCause cause);
			MauPunishment_STCPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::vector<int> getCardsToDraw() const;
			std::string getConcernedUsername() const;
			MauPunishmentCause getCause() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}