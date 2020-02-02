#include "PlayerHasMauedSuccessfully_STCPacket.h"

namespace card {	
	PlayerHasMauedSuccessfully_STCPacket::PlayerHasMauedSuccessfully_STCPacket(std::string username) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID) {
	
	}
	PlayerHasMauedSuccessfully_STCPacket::PlayerHasMauedSuccessfully_STCPacket(nlohmann::json& jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}

}