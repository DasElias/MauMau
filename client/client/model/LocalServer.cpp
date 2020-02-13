#include "LocalServer.h"


namespace card {
	LocalServer::LocalServer(std::shared_ptr<STCPacketTransmitter> packetTransmitter, std::size_t amountOfOpponents, const std::shared_ptr<ParticipantOnServer>& localParticipantOnServer, RoomOptions roomOptions) :
			packetTransmitter(packetTransmitter),
			localParticipantOnServer(localParticipantOnServer),
			room(0, packetTransmitter, roomOptions) {		
	
		initJoinAiPlayers(amountOfOpponents);
		room.joinRoom(localParticipantOnServer);
		room.initRoomLeaderWithoutPermissionsChecking(localParticipantOnServer);
	}
	void LocalServer::initJoinAiPlayers(std::size_t amountOfOpponents) {
		for(int i = 0; i < amountOfOpponents; i++) {
			// we pass the username of the local participant as reserved username
			room.joinRoomAiPlayer(localParticipantOnServer->getUsername());
		}
	}
	ServerRoom& LocalServer::getRoom() {
		return room;
	}
	void LocalServer::joinAiPlayer() {
		try {
			room.joinRoomAiPlayer();
		} catch(...) {
			throw std::runtime_error("Too many players!");
		}
	}
	std::vector<std::string> LocalServer::getOpponentUsernames() {
		std::vector<std::string> opponentUsernames;
		for(auto& p : room.getParticipants()) {
			if(p != localParticipantOnServer) opponentUsernames.push_back(p->getUsername());
		}

		return opponentUsernames;
	}

	std::vector<Avatar> LocalServer::getOpponentAvatars() {
		std::vector<Avatar> opponentAvatars;
		for(auto& p : room.getParticipants()) {
			if(p != localParticipantOnServer) opponentAvatars.push_back(p->getAvatar());

		}
		
		return opponentAvatars;
	}
	
}