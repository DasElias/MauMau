#include "EnteringRoomSuccessReport_STCAnswerPacket.h"

namespace card {
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::STATUS_CODE_KEY = "31A";
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::USERNAMES_OF_OTHER_PARTICIPANTS_KEY = "31B";
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::AVATARS_OF_OTHER_PARTICIPANTS_KEY = "31C";
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::ROOM_LEADER_KEY = "31D";
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::ROOM_CODE_KEY = "31E";
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::OPTIONS_KEY = "31F";
	std::string const EnteringRoomSuccessReport_STCAnswerPacket::ARE_OTHER_PARTICIPANTS_AI_PLAYERS_KEY = "31G";


	EnteringRoomSuccessReport_STCAnswerPacket::EnteringRoomSuccessReport_STCAnswerPacket(int statusCode, std::vector<std::string> usernamesOfOtherParticipants, std::vector<Avatar> avatarsOfOtherParticipants, std::vector<bool> areOtherParticipantsAiPlayers, std::string roomLeader, RoomCode roomCode, std::map<std::string, int> options) :
			Packet(PACKET_ID),
			statusCode(statusCode),
			usernamesOfOtherParticipants(usernamesOfOtherParticipants),
			avatarsOfOtherParticipants(avatarsOfOtherParticipants),
			areOtherParticipantsAiPlayers_field(areOtherParticipantsAiPlayers),
			roomLeader(roomLeader),
			roomCode(roomCode),
			options(options) {
	}

	EnteringRoomSuccessReport_STCAnswerPacket::EnteringRoomSuccessReport_STCAnswerPacket(nlohmann::json& jsonHandle) :
			Packet(PACKET_ID),
			statusCode(jsonHandle[STATUS_CODE_KEY]),
			usernamesOfOtherParticipants(jsonHandle.at(USERNAMES_OF_OTHER_PARTICIPANTS_KEY).get<std::vector<std::string>>()),
			avatarsOfOtherParticipants(jsonHandle.at(AVATARS_OF_OTHER_PARTICIPANTS_KEY).get<std::vector<Avatar>>()),
			areOtherParticipantsAiPlayers_field(jsonHandle.at(ARE_OTHER_PARTICIPANTS_AI_PLAYERS_KEY).get<std::vector<bool>>()),
			roomLeader(jsonHandle[ROOM_LEADER_KEY]),
			roomCode(jsonHandle[ROOM_CODE_KEY]),
			options(jsonHandle.at(OPTIONS_KEY).get<std::map<std::string, int>>()) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}

	int EnteringRoomSuccessReport_STCAnswerPacket::getStatusCode() const {
		return statusCode;
	}

	std::vector<std::string> EnteringRoomSuccessReport_STCAnswerPacket::getUsernamesOfOtherParticipants() const {
		return usernamesOfOtherParticipants;
	}

	std::vector<Avatar> EnteringRoomSuccessReport_STCAnswerPacket::getAvatarsOfOtherParticipants() const {
		return avatarsOfOtherParticipants;
	}

	std::vector<bool> EnteringRoomSuccessReport_STCAnswerPacket::areOtherParticipantsAiPlayers() const {
		return areOtherParticipantsAiPlayers_field;
	}

	std::string EnteringRoomSuccessReport_STCAnswerPacket::getRoomLeader() const {
		return roomLeader;
	}

	RoomCode EnteringRoomSuccessReport_STCAnswerPacket::getRoomCode() const {
		return roomCode;
	}

	std::map<std::string, int> EnteringRoomSuccessReport_STCAnswerPacket::getOptions() const {
		return options;
	}

	void EnteringRoomSuccessReport_STCAnswerPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);
		jsonHandle[STATUS_CODE_KEY] = statusCode;
		jsonHandle[USERNAMES_OF_OTHER_PARTICIPANTS_KEY] = usernamesOfOtherParticipants;
		jsonHandle[AVATARS_OF_OTHER_PARTICIPANTS_KEY] = avatarsOfOtherParticipants;
		jsonHandle[ARE_OTHER_PARTICIPANTS_AI_PLAYERS_KEY] = areOtherParticipantsAiPlayers_field;
		jsonHandle[ROOM_LEADER_KEY] = roomLeader;
		jsonHandle[ROOM_CODE_KEY] = roomCode;
		jsonHandle[OPTIONS_KEY] = options;
	}


}