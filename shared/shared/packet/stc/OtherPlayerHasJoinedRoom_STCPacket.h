#pragma once
#include "AbstractUsernamePacket_STCPacket.h"
#include "../../model/Avatar.h"

namespace card {
	class OtherPlayerHasJoinedRoom_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 100;

		private:
			static std::string const AVATAR_KEY;
			static std::string const IS_AI_PLAYER_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			Avatar avatar;
			bool field_isAiPlayer;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			OtherPlayerHasJoinedRoom_STCPacket(std::string username, Avatar avatar, bool isAiPlayer);
			OtherPlayerHasJoinedRoom_STCPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			Avatar getAvatar() const;
			bool isAiPlayer() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}