#include "RoomManager.h"
#include "RoomManager.h"
#include <shared/packet/cts/RoomJoinRequest_CTSPacket.h>
#include <shared/packet/cts/RoomCreationRequest_CTSPacket.h>
#include <shared/packet/stc/EnteringRoomSuccessReport_STCAnswerPacket.h>
#include <shared/utils/MathUtils.h>
#include <iostream>

namespace card {
	RoomManager::RoomManager(std::shared_ptr<ServerPacketTransmitter> packetTransmitter) :
			packetTransmitter(packetTransmitter),
			handler_onJoinRoom(std::bind(&RoomManager::listener_onJoinRoom, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onCreateRoom(std::bind(&RoomManager::listener_onCreateRoom, this, std::placeholders::_1, std::placeholders::_2)) {

		packetTransmitter->addListenerForClientPkt(RoomJoinRequest_CTSPacket::PACKET_ID, handler_onJoinRoom);
		packetTransmitter->addListenerForClientPkt(RoomCreationRequest_CTSPacket::PACKET_ID, handler_onCreateRoom);
	}
	RoomManager::~RoomManager() {
		packetTransmitter->removeListenerForClientPkt(RoomJoinRequest_CTSPacket::PACKET_ID, handler_onJoinRoom);
		packetTransmitter->removeListenerForClientPkt(RoomCreationRequest_CTSPacket::PACKET_ID, handler_onCreateRoom);
	}
	void RoomManager::join(RoomCode roomCode, std::string username, Avatar avatar, const std::shared_ptr<ConnectionToClient>& conn) {
		typedef EnteringRoomSuccessReport_STCAnswerPacket SuccessReport;

		if(! doesRoomExist(roomCode)) {
			sendEnteringRoomSuccessReport(conn, SuccessReport::ROOM_NOT_FOUND_STATUS);
			return;
		}
		auto& concernedRoom = rooms.at(roomCode);

		if(concernedRoom->isRoomFull()) {
			sendEnteringRoomSuccessReport(conn, SuccessReport::ROOM_FULL_STATUS);
			return;
		}
		if(! concernedRoom->isUsernameAvailable(username)) {
			sendEnteringRoomSuccessReport(conn, SuccessReport::USERNAME_TAKEN_STATUS);
			return;
		}

		auto constructedParticipant = std::make_shared<ParticipantOnServer>(username, avatar);
		bool wasJoinSuccessful = concernedRoom->joinRoom(constructedParticipant);
		if(! wasJoinSuccessful) {
			sendEnteringRoomSuccessReport(conn, SuccessReport::UNKNOWN_ERROR_STATUS);
			return;
		}
		packetTransmitter->registerParticipant(conn, constructedParticipant);

		auto usernamesOfOtherParticipants = getUsernamesOfOtherParticipants(concernedRoom, username);
		auto areOtherParticipantsAiPlayers = this->areOtherParticipantsAiPlayers(concernedRoom, username);
		auto avatarsOfOtherParticipants = getAvatarsOfOtherParticipants(concernedRoom, username);
		auto usernameOfRoomLeader = concernedRoom->getRoomLeader()->getUsername();
		auto allRoomOptions = concernedRoom->getRoomOptions().getAllOptions();
		sendEnteringRoomSuccessReport(conn, SuccessReport::SUCCESS_STATUS, usernamesOfOtherParticipants, avatarsOfOtherParticipants, areOtherParticipantsAiPlayers, usernameOfRoomLeader, roomCode, allRoomOptions);
	}
	void RoomManager::createAndJoin(std::string username, Avatar avatar, std::map<std::string, int> roomOptions, const std::shared_ptr<ConnectionToClient>& conn) {
		typedef EnteringRoomSuccessReport_STCAnswerPacket SuccessReport;
		
		auto newRoomCode = getNewRoomCode();
		this->rooms.insert(std::make_pair(newRoomCode, std::make_unique<ServerRoom>(newRoomCode, packetTransmitter, roomOptions)));
		auto& room = this->rooms.at(newRoomCode);

		auto constructedParticipant = std::make_shared<ParticipantOnServer>(username, avatar);
		bool wasJoinSuccessful = room->joinRoom(constructedParticipant);
		if(! wasJoinSuccessful) {
			sendEnteringRoomSuccessReport(conn, SuccessReport::UNKNOWN_ERROR_STATUS);
		}

		room->initRoomLeaderWithoutPermissionsChecking(constructedParticipant);
		packetTransmitter->registerParticipant(conn, constructedParticipant);

		auto allRoomOptions = room->getRoomOptions().getAllOptions();
		sendEnteringRoomSuccessReport(conn, SuccessReport::SUCCESS_STATUS, {}, {}, {}, username, newRoomCode, allRoomOptions);
	}
	void RoomManager::leave(std::shared_ptr<ConnectionToClient> conn) {
		if(packetTransmitter->wasParticipantRegistered(conn)) {
			std::shared_ptr<ParticipantOnServer> participant = packetTransmitter->getRegisteredParticipant(conn);

			for(auto& pair : this->rooms) {
				auto& singleRoom = pair.second;

				if(singleRoom->checkIfParticipant(participant)) {
					singleRoom->leaveRoom(participant, false);
					closeRoomIfNoParticipants(singleRoom);
					break;
				}
			}

			packetTransmitter->unregisterParticipant(conn);
		}
	}
	void RoomManager::closeRoomIfNoParticipants(const std::unique_ptr<ServerRoom>& room) {
		if(room->getParticipants().size() == 0) {
			this->rooms.erase(room->getRoomCode());
		}
	}
	bool RoomManager::doesRoomExist(RoomCode roomCode) {
		return rooms.find(roomCode) != rooms.end();
	}
	std::vector<std::string> RoomManager::getUsernamesOfOtherParticipants(const std::unique_ptr<ServerRoom>& room, std::string participantToFilter) {
		auto usernamesOfOtherParticipants = room->getUsernamesOfParticipants();
		usernamesOfOtherParticipants.erase(std::find(usernamesOfOtherParticipants.begin(), usernamesOfOtherParticipants.end(), participantToFilter));
		return usernamesOfOtherParticipants;
	}
	std::vector<Avatar> RoomManager::getAvatarsOfOtherParticipants(const std::unique_ptr<ServerRoom>& room, std::string participantToFilter) {
		std::vector<Avatar> avatarsOfOtherParticipants;
		for(auto& participant : room->getParticipants()) {
			if(participant->getUsername() != participantToFilter) {
				avatarsOfOtherParticipants.push_back(participant->getAvatar());
			}
		}
		return avatarsOfOtherParticipants;
	}
	std::vector<bool> RoomManager::areOtherParticipantsAiPlayers(const std::unique_ptr<ServerRoom>& room, std::string participantToFilter) {
		std::vector<bool> areOtherParticipantsAiPlayers;
		for(auto& participant : room->getParticipants()) {
			if(participant->getUsername() != participantToFilter) {
				areOtherParticipantsAiPlayers.push_back(! participant->isRealPlayer());
			}
		}
		return areOtherParticipantsAiPlayers;
	}
	RoomCode RoomManager::getNewRoomCode() {
		/*RoomCode roomCode;
		do {
			roomCode = randomInRange<RoomCode>(MIN_ROOM_CODE, MAX_ROOM_CODE);
		} while(doesRoomExist(roomCode));
		return roomCode;*/
		std::cout << "TODO: fix getNewroomCode()" << std::endl;
		return 0;
	}
	void RoomManager::sendEnteringRoomSuccessReport(const std::shared_ptr<ConnectionToClient>& conn, int statusCode, std::vector<std::string> usernamesOfOtherParticipants, std::vector<Avatar> avatarsOfOtherParticipants, std::vector<bool> areOtherParticipantsAiPlayers, std::string roomLeader, RoomCode roomCode, std::map<std::string, int> nonDefaultOptions) {
		EnteringRoomSuccessReport_STCAnswerPacket packet(statusCode, usernamesOfOtherParticipants, avatarsOfOtherParticipants, areOtherParticipantsAiPlayers, roomLeader, roomCode, nonDefaultOptions);
		packetTransmitter->sendPacketToClient(packet, conn);
	}
	optionalSuccessAnswerPacket RoomManager::listener_onJoinRoom(ClientToServerPacket& p, const std::shared_ptr<ConnectionToClient>& conn) {
		auto& casted = dynamic_cast<RoomJoinRequest_CTSPacket&>(p);

		join(casted.getRoomCode(), casted.getOwnUsername(), casted.getAvatar(), conn);
		return OperationSuccessful_STCAnswerPacket(true);
	}
	optionalSuccessAnswerPacket RoomManager::listener_onCreateRoom(ClientToServerPacket& p, const std::shared_ptr<ConnectionToClient>& conn) {
		auto& casted = dynamic_cast<RoomCreationRequest_CTSPacket&>(p);

		createAndJoin(casted.getOwnUsername(), casted.getAvatar(), casted.getOptions(), conn);
		return OperationSuccessful_STCAnswerPacket(true);
	}
}
