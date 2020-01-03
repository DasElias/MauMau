#include "EnteringRoomSuccessReport_STCAnswerPacket.h"

namespace card {
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::STATUS_CODE_KEY = "statusCode";
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::USERNAMES_OF_OTHER_PARTICIPANTS_KEY = "usernamesOfOtherParticipants";
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::ROOM_LEADER_KEY = "roomLeader";
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::ROOM_CODE_KEY = "roomCode";
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::NON_DEFAULT_OPTIONS_KEY = "nonDefaultOptions";


	EnteringRoomSuccessReport_STCAnswerPacket::EnteringRoomSuccessReport_STCAnswerPacket(int statusCode, std::vector<std::string> usernamesOfOtherParticipants, std::string roomLeader, RoomCode roomCode, std::map<std::string, int> nonDefaultOptions) :
			Packet(PACKET_ID),
			statusCode(statusCode),
			usernamesOfOtherParticipants(usernamesOfOtherParticipants),
			roomLeader(roomLeader),
			roomCode(roomCode),
			nonDefaultOptions(nonDefaultOptions) {
	}

	EnteringRoomSuccessReport_STCAnswerPacket::EnteringRoomSuccessReport_STCAnswerPacket(nlohmann::json jsonHandle) :
			Packet(PACKET_ID),
			statusCode(jsonHandle[STATUS_CODE_KEY]),
			usernamesOfOtherParticipants(jsonHandle.at(USERNAMES_OF_OTHER_PARTICIPANTS_KEY).get<std::vector<std::string>>()),
			roomLeader(jsonHandle[ROOM_LEADER_KEY]),
			roomCode(jsonHandle[ROOM_CODE_KEY]),
			nonDefaultOptions(jsonHandle.at(NON_DEFAULT_OPTIONS_KEY).get<std::map<std::string, int>>()) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}

	int EnteringRoomSuccessReport_STCAnswerPacket::getStatusCode() const {
		return statusCode;
	}

	std::vector<std::string> EnteringRoomSuccessReport_STCAnswerPacket::getUsernamesOfOtherParticipants() const {
		return usernamesOfOtherParticipants;
	}

	std::string EnteringRoomSuccessReport_STCAnswerPacket::getRoomLeader() const {
		return roomLeader;
	}

	RoomCode EnteringRoomSuccessReport_STCAnswerPacket::getRoomCode() const {
		return roomCode;
	}

	std::map<std::string, int> EnteringRoomSuccessReport_STCAnswerPacket::getNonDefaultOptions() const {
		return nonDefaultOptions;
	}

	void EnteringRoomSuccessReport_STCAnswerPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);
		jsonHandle[STATUS_CODE_KEY] = statusCode;
		jsonHandle[USERNAMES_OF_OTHER_PARTICIPANTS_KEY] = usernamesOfOtherParticipants;
		jsonHandle[ROOM_LEADER_KEY] = roomLeader;
		jsonHandle[ROOM_CODE_KEY] = roomCode;
		jsonHandle[NON_DEFAULT_OPTIONS_KEY] = nonDefaultOptions;
	}


}