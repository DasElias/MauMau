#pragma once
#include "ClientToServerPacket.h"
#include "../../model/Avatar.h"
#include <map>
namespace card {
	class RoomCreationRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 401;

		private:
			static std::string const USERNAME_KEY;
			static std::string const AVATAR_KEY;
			static std::string const OPTIONS_KEY;
			static std::string const CLIENT_PROTOCOL_VERSION_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::string ownUsername;
			Avatar avatar;
			std::map<std::string, int> options;
			std::string clientProtocolVersion;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			RoomCreationRequest_CTSPacket(std::string ownUsername, Avatar avatar, std::map<std::string, int> options, std::string clientProtocolVersion);
			RoomCreationRequest_CTSPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getOwnUsername() const;
			Avatar getAvatar() const;
			std::map<std::string, int> getOptions() const;
			std::string getClientProtocolVersion() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}