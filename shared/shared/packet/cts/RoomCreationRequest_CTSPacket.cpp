#include "RoomCreationRequest_CTSPacket.h"

namespace card {
	std::string const RoomCreationRequest_CTSPacket::USERNAME_KEY = "41A";
	std::string const RoomCreationRequest_CTSPacket::AVATAR_KEY = "41B";
	std::string const RoomCreationRequest_CTSPacket::OPTIONS_KEY = "41C";
	std::string const RoomCreationRequest_CTSPacket::CLIENT_PROTOCOL_VERSION_KEY = "41D";


	RoomCreationRequest_CTSPacket::RoomCreationRequest_CTSPacket(std::string ownUsername, Avatar avatar, std::map<std::string, int> options, std::string clientProtocolVersion) :
			ClientToServerPacket(PACKET_ID),
			ownUsername(ownUsername),
			avatar(avatar),
			options(options),
			clientProtocolVersion(clientProtocolVersion) {
	}
	RoomCreationRequest_CTSPacket::RoomCreationRequest_CTSPacket(nlohmann::json& jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID),
			ownUsername(jsonHandle[USERNAME_KEY]),
			avatar(jsonHandle[AVATAR_KEY]),
			options(jsonHandle.at(OPTIONS_KEY).get<std::map<std::string, int>>()),
			clientProtocolVersion(jsonHandle[CLIENT_PROTOCOL_VERSION_KEY]) {
	}
	std::string RoomCreationRequest_CTSPacket::getOwnUsername() const {
		return ownUsername;
	}
	Avatar RoomCreationRequest_CTSPacket::getAvatar() const {
		return avatar; 
	}
	std::map<std::string, int> RoomCreationRequest_CTSPacket::getOptions() const {
		return options;
	}
	std::string RoomCreationRequest_CTSPacket::getClientProtocolVersion() const {
		return clientProtocolVersion;
	}
	void RoomCreationRequest_CTSPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		ClientToServerPacket::addJsonProperties(jsonHandle);
		jsonHandle[USERNAME_KEY] = ownUsername;
		jsonHandle[AVATAR_KEY] = avatar;
		jsonHandle[OPTIONS_KEY] = options;
		jsonHandle[CLIENT_PROTOCOL_VERSION_KEY] = clientProtocolVersion;
	}
}