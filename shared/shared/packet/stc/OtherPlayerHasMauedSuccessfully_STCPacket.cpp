#include "OtherPlayerHasMauedSuccessfully_STCPacket.h"

namespace card {	
	OtherPlayerHasMauedSuccessfully_STCPacket::OtherPlayerHasMauedSuccessfully_STCPacket(std::string username) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID) {
	
	}
	OtherPlayerHasMauedSuccessfully_STCPacket::OtherPlayerHasMauedSuccessfully_STCPacket(nlohmann::json& jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}

}