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

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			Avatar avatar;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			OtherPlayerHasJoinedRoom_STCPacket(std::string username, Avatar avatar);
			OtherPlayerHasJoinedRoom_STCPacket(nlohmann::json jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			Avatar getAvatar() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}