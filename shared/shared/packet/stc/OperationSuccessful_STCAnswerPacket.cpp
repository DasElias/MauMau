#include "OperationSuccessful_STCAnswerPacket.h"

namespace card {
	std::string const OperationSuccessful_STCAnswerPacket::STATUS_CODE_KEY = "statusCode";

	OperationSuccessful_STCAnswerPacket::OperationSuccessful_STCAnswerPacket(bool wasSuccessful_field) :
			Packet(PACKET_ID),
			wasSuccessful_field(wasSuccessful_field) {
	}

	OperationSuccessful_STCAnswerPacket::OperationSuccessful_STCAnswerPacket(nlohmann::json jsonHandle) :
			Packet(PACKET_ID),
			wasSuccessful_field(jsonHandle[STATUS_CODE_KEY]) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}


	bool OperationSuccessful_STCAnswerPacket::wasSuccessful() const {
		return wasSuccessful_field;
	}

	void OperationSuccessful_STCAnswerPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);
		jsonHandle[STATUS_CODE_KEY] = wasSuccessful_field;
	}


}