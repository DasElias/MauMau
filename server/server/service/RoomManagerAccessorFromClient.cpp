#include "RoomManagerAccessorFromClient.h"
#include <shared/packet/cts/RoomJoinRequest_CTSPacket.h>
#include <shared/packet/cts/RoomCreationRequest_CTSPacket.h>
#include <shared/packet/cts/KickPlayerRequest_CTSPacket.h>
#include <shared/packet/stc/EnteringRoomSuccessReport_STCAnswerPacket.h>
#include "InvalidInputException.h"
#include <shared/model/EnteringRoomErrorCause.h>
#include <shared/utils/Logger.h>
#include <shared/packet/ProtocolVersion.h>
#include <shared/model/EnteringRoomErrorCause.h>

namespace card {
	RoomManagerAccessorFromClient::RoomManagerAccessorFromClient(std::shared_ptr<ServerPacketTransmitter> packetTransmitter) :
			roomManager(packetTransmitter),
			packetTransmitter(packetTransmitter),
			handler_onJoinRoom(std::bind(&RoomManagerAccessorFromClient::listener_onJoinRoom, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onCreateRoom(std::bind(&RoomManagerAccessorFromClient::listener_onCreateRoom, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onKickPlayer(std::bind(&RoomManagerAccessorFromClient::listener_onKickPlayer, this, std::placeholders::_1, std::placeholders::_2)) {

		packetTransmitter->addListenerForClientPkt(RoomJoinRequest_CTSPacket::PACKET_ID, handler_onJoinRoom);
		packetTransmitter->addListenerForClientPkt(RoomCreationRequest_CTSPacket::PACKET_ID, handler_onCreateRoom);
		packetTransmitter->addListenerForClientPkt(KickPlayerRequest_CTSPacket::PACKET_ID, handler_onKickPlayer);
	}
	RoomManagerAccessorFromClient::~RoomManagerAccessorFromClient() {
		packetTransmitter->removeListenerForClientPkt(RoomJoinRequest_CTSPacket::PACKET_ID, handler_onJoinRoom);
		packetTransmitter->removeListenerForClientPkt(RoomCreationRequest_CTSPacket::PACKET_ID, handler_onCreateRoom);
		packetTransmitter->removeListenerForClientPkt(KickPlayerRequest_CTSPacket::PACKET_ID, handler_onKickPlayer);
	}
	void RoomManagerAccessorFromClient::leave(std::shared_ptr<AbstractConnectionToClient> conn) {
		roomManager.leave(conn);
	}
	std::size_t RoomManagerAccessorFromClient::getAmountOfRooms() const {
		return roomManager.getAmountOfRooms();
	}
	std::size_t RoomManagerAccessorFromClient::getAmountOfUsersInRooms() const {
		return roomManager.getAmountOfUsersInRooms();
	}
	optionalSuccessAnswerPacket RoomManagerAccessorFromClient::listener_onJoinRoom(ClientToServerPacket& p, const std::shared_ptr<AbstractConnectionToClient>& conn) {
		auto& casted = dynamic_cast<RoomJoinRequest_CTSPacket&>(p);

		EnteringRoomErrorCause_t statusCode = EnteringRoomErrorCause::SUCCESS_STATUS;
		std::vector<std::string> usernamesOfOtherParticipants;
		std::vector<Avatar> avatarsOfOtherParticipants;
		std::vector<bool> areOtherParticipantsAiPlayers;
		std::string roomLeaderUsername = "";
		RoomCode roomCode = 0;
		std::map<std::string, int> options;

		if(casted.getClientProtocolVersion() == PROTOCOL_VERSION) {
			try {
				std::string usernameOfPlayerToJoin = casted.getOwnUsername();
				RoomManager::RoomJoinReturnValue rv = roomManager.join(casted.getRoomCode(), usernameOfPlayerToJoin, casted.getAvatar(), conn);

				usernamesOfOtherParticipants = getUsernamesOfOtherParticipants(rv.room, usernameOfPlayerToJoin);
				avatarsOfOtherParticipants = getAvatarsOfOtherParticipants(rv.room, usernameOfPlayerToJoin);
				areOtherParticipantsAiPlayers = this->areOtherParticipantsAiPlayers(rv.room, usernameOfPlayerToJoin);
				roomLeaderUsername = rv.room.getRoomLeader()->getUsername();
				roomCode = rv.room.getRoomCode();
				options = rv.room.getRoomOptions().getAllOptions();
			} catch(const InvalidInputException& e) {
				statusCode = e.getStatusCode();
			}
		} else {
			statusCode = EnteringRoomErrorCause::PROTOCOL_VERSION_MISMATCH_STATUS;
		}

		EnteringRoomSuccessReport_STCAnswerPacket answerPacket(statusCode, usernamesOfOtherParticipants, avatarsOfOtherParticipants, areOtherParticipantsAiPlayers, roomLeaderUsername, roomCode, options);
		packetTransmitter->sendPacketToClient(answerPacket, conn);

		return OperationSuccessful_STCAnswerPacket(true);
	}
	optionalSuccessAnswerPacket RoomManagerAccessorFromClient::listener_onCreateRoom(ClientToServerPacket& p, const std::shared_ptr<AbstractConnectionToClient>& conn) {
		auto& casted = dynamic_cast<RoomCreationRequest_CTSPacket&>(p);
		std::map<std::string, int> options = casted.getOptions();
		if(options.size() != RoomOptions::OPTIONS_SIZE) return OperationSuccessful_STCAnswerPacket(false);

		EnteringRoomErrorCause_t statusCode = EnteringRoomErrorCause::SUCCESS_STATUS;
		std::vector<std::string> usernamesOfOtherParticipants;
		std::vector<Avatar> avatarsOfOtherParticipants;
		std::vector<bool> areOtherParticipantsAiPlayers;
		std::string roomLeaderUsername = "";
		RoomCode roomCode = 0;

		if(casted.getClientProtocolVersion() == PROTOCOL_VERSION) {
			try {
				RoomManager::RoomJoinReturnValue rv = roomManager.createAndJoin(casted.getOwnUsername(), casted.getAvatar(), options, conn);

				roomCode = rv.room.getRoomCode();
				roomLeaderUsername = casted.getOwnUsername();
			} catch(const InvalidInputException & e) {
				statusCode = e.getStatusCode();
			}
		} else {
			statusCode = EnteringRoomErrorCause::PROTOCOL_VERSION_MISMATCH_STATUS;
		}

		EnteringRoomSuccessReport_STCAnswerPacket answerPacket(statusCode, usernamesOfOtherParticipants, avatarsOfOtherParticipants, areOtherParticipantsAiPlayers, roomLeaderUsername, roomCode, options);
		packetTransmitter->sendPacketToClient(answerPacket, conn);

		return OperationSuccessful_STCAnswerPacket(true);
	}
	optionalSuccessAnswerPacket RoomManagerAccessorFromClient::listener_onKickPlayer(ClientToServerPacket& p, const std::shared_ptr<AbstractConnectionToClient>& requesterConn) {
		auto& casted = dynamic_cast<KickPlayerRequest_CTSPacket&>(p);
		bool wasSuccessful = roomManager.kick(casted.getUsernameOfPlayerToKick(), requesterConn);
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	std::vector<std::string> RoomManagerAccessorFromClient::getUsernamesOfOtherParticipants(ServerRoom& room, std::string joinedParticipant) {
		auto usernamesOfOtherParticipants = room.getUsernamesOfParticipants();
		usernamesOfOtherParticipants.erase(std::find(usernamesOfOtherParticipants.begin(), usernamesOfOtherParticipants.end(), joinedParticipant));
		return usernamesOfOtherParticipants;
	}
	std::vector<Avatar> RoomManagerAccessorFromClient::getAvatarsOfOtherParticipants(ServerRoom& room, std::string joinedParticipant) {
		std::vector<Avatar> avatarsOfOtherParticipants;
		for(auto& participant : room.getParticipants()) {
			if(participant->getUsername() != joinedParticipant) {
				avatarsOfOtherParticipants.push_back(participant->getAvatar());
			}
		}
		return avatarsOfOtherParticipants;
	}
	std::vector<bool> RoomManagerAccessorFromClient::areOtherParticipantsAiPlayers(ServerRoom& room, std::string joinedParticipant) {
		std::vector<bool> areOtherParticipantsAiPlayers;
		for(auto& participant : room.getParticipants()) {
			if(participant->getUsername() != joinedParticipant) {
				areOtherParticipantsAiPlayers.push_back(! participant->isRealPlayer());
			}
		}
		return areOtherParticipantsAiPlayers;
	}
}