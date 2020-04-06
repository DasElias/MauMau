#include "RoomManager.h"
#include <shared/utils/MathUtils.h>
#include <iostream>
#include "InvalidInputException.h"
#include <shared/model/EnteringRoomErrorCause.h>

namespace card {
	RoomManager::RoomManager(std::shared_ptr<AbstractServerPacketTransmitter> abstractPacketTransmitter) :
			packetTransmitter(abstractPacketTransmitter) {
	}
	RoomManager::RoomJoinReturnValue RoomManager::join(RoomCode roomCode, std::string username, Avatar avatar, const std::shared_ptr<AbstractConnectionToClient>& conn) {
		if(isConnectionInRoom(conn)) throw InvalidInputException(EnteringRoomErrorCause::ALREADY_IN_ROOM_STATUS);
		if(! doesRoomExist(roomCode)) throw InvalidInputException(EnteringRoomErrorCause::ROOM_NOT_FOUND_STATUS);
		
		auto& concernedRoom = rooms.at(roomCode);

		if(concernedRoom->isRoomFull()) throw InvalidInputException(EnteringRoomErrorCause::ROOM_FULL_STATUS);
		if(! concernedRoom->isUsernameAvailable(username)) throw InvalidInputException(EnteringRoomErrorCause::USERNAME_TAKEN_STATUS);

		auto constructedParticipant = std::make_shared<ParticipantOnServer>(username, avatar);
		bool wasJoinSuccessful = concernedRoom->joinRoom(constructedParticipant);
		if(! wasJoinSuccessful) throw InvalidInputException(EnteringRoomErrorCause::UNKNOWN_ERROR_STATUS);
		
		users.insert({conn, {constructedParticipant, *concernedRoom}});
		packetTransmitter->registerParticipant(conn, constructedParticipant);

		return {*concernedRoom, constructedParticipant};
	}
	RoomManager::RoomJoinReturnValue RoomManager::createAndJoin(std::string username, Avatar avatar, std::map<std::string, int> roomOptions, const std::shared_ptr<AbstractConnectionToClient>& conn) {
		if(isConnectionInRoom(conn)) throw InvalidInputException(EnteringRoomErrorCause::ALREADY_IN_ROOM_STATUS);
		
		auto newRoomCode = getNewRoomCode();
		this->rooms.insert(std::make_pair(newRoomCode, std::make_unique<ServerRoom>(newRoomCode, packetTransmitter, roomOptions)));
		auto& room = this->rooms.at(newRoomCode);
		
		auto constructedParticipant = std::make_shared<ParticipantOnServer>(username, avatar);
		bool wasJoinSuccessful = room->joinRoom(constructedParticipant);
		if(! wasJoinSuccessful) throw InvalidInputException(EnteringRoomErrorCause::UNKNOWN_ERROR_STATUS);


		room->initRoomLeaderWithoutPermissionsChecking(constructedParticipant);
		users.insert({conn, {constructedParticipant, *room}});
		packetTransmitter->registerParticipant(conn, constructedParticipant);

		return {*room, constructedParticipant};
	}
	void RoomManager::leave(std::shared_ptr<AbstractConnectionToClient> conn) {
		if(isConnectionInRoom(conn) && packetTransmitter->wasParticipantRegistered(conn)) {
			auto& room = getRoomToConnection(conn);
			auto participant = getParticipantToConnection(conn);

			room.leaveRoom(participant, false);
			users.erase(users.find(conn));
			closeRoomIfNecessary(room);

			packetTransmitter->unregisterParticipant(conn);
		}
	}
	bool RoomManager::kick(std::string usernameOfPlayerToKick, std::shared_ptr<AbstractConnectionToClient> requesterConn) {
		if(! isConnectionInRoom(requesterConn)) return false;
		auto& room = getRoomToConnection(requesterConn);
		std::shared_ptr<ParticipantOnServer> requesterParticipant = getParticipantToConnection(requesterConn);

		if(! room.checkIfLeader(requesterParticipant)) return false;
		if(! room.checkIfParticipantByUsername(usernameOfPlayerToKick)) return false;

		std::shared_ptr<ParticipantOnServer> participantToKick = room.lookupParticipantByUsername(usernameOfPlayerToKick);
		if(participantToKick->isRealPlayer()) {
			std::shared_ptr<AbstractConnectionToClient> connOfParticipantToKick = getConnectionToParticipant(participantToKick);

			room.leaveRoom(participantToKick, true);
			closeRoomIfNecessary(room);

			users.erase(users.find(connOfParticipantToKick));
			connOfParticipantToKick->close();
			packetTransmitter->unregisterParticipant(connOfParticipantToKick);
		} else {
			room.leaveRoom(participantToKick, true);
		}
		return true;
	}
	bool RoomManager::isConnectionInRoom(const std::shared_ptr<AbstractConnectionToClient>& conn) const {
		return users.find(conn) != users.end();
	}
	ServerRoom& RoomManager::getRoomToConnection(const std::shared_ptr<AbstractConnectionToClient>& conn) {
		if(! isConnectionInRoom(conn)) throw std::runtime_error("No room was registered to connection.");
		return (*users.find(conn)).second.second;
	}
	std::shared_ptr<ParticipantOnServer> RoomManager::getParticipantToConnection(const std::shared_ptr<AbstractConnectionToClient>& conn) {
		if(!isConnectionInRoom(conn)) throw std::runtime_error("No participant was registered to connection.");
		return (*users.find(conn)).second.first;
	}
	std::shared_ptr<AbstractConnectionToClient> RoomManager::getConnectionToParticipant(const std::shared_ptr<ParticipantOnServer>& participant) {
		for(auto& val : users) {
			auto valParticipant = val.second.first;
			if(valParticipant == participant) {
				return val.first;
			}
		}
		throw std::runtime_error("No connection was registered to participant.");
	}
	void RoomManager::closeRoomIfNecessary(const ServerRoom& room) {
		if(room.shouldCloseRoom()) closeRoom(room);
	}
	void RoomManager::closeRoom(const ServerRoom& r) {
		this->rooms.erase(r.getRoomCode());
	}
	bool RoomManager::doesRoomExist(RoomCode roomCode) {
		return rooms.find(roomCode) != rooms.end();
	}
	RoomCode RoomManager::getNewRoomCode() {
		RoomCode roomCode;
		do {
			roomCode = randomInRange<RoomCode>(MIN_ROOM_CODE, MAX_ROOM_CODE);
		} while(doesRoomExist(roomCode));
		return roomCode;
	}
}
