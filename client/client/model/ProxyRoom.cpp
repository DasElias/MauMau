#include "ProxyRoom.h"
#include <stdexcept>
#include <shared/model/PlayerNotFoundException.h>


#include <shared/packet/stc/OtherPlayerHasJoinedRoom_STCPacket.h>
#include <shared/packet/stc/OtherPlayerHasLeavedRoom_STCPacket.h>
#include <shared/packet/stc/PlayerWantsJoinNextGame_STCPacket.h>
#include <shared/packet/stc/PlayerWantsLeaveNextGame_STCPacket.h>
#include <shared/packet/stc/OptionsWereChanged_STCPacket.h>
#include <shared/packet/stc/RoomLeaderHasChanged_STCPacket.h>
#include <shared/packet/stc/GameHasBeenStarted_STCPacket.h>
#include <shared/packet/cts/GameStartRequest_CTSPacket.h>

#include <shared/packet/cts/JoinNextGameRequest_CTSPacket.h>
#include <shared/packet/cts/LeaveNextGameRequest_CTSPacket.h>
#include <shared/packet/cts/ChangeRoomLeaderRequest_CTSPacket.h>

namespace card {
	ProxyRoom::ProxyRoom(std::shared_ptr<CTSPacketTransmitter> packetTransmitter, std::vector<std::string> opponentsUsernames, std::vector<Avatar> opponentAvatars, std::string localPlayerUsername, Avatar localPlayerAvatar, std::string usernameOfLeader, std::map<std::string, int> options) :
			packetTransmitter(packetTransmitter),
			options(options),

			handler_onPlayerJoinsRoom(std::bind(&ProxyRoom::listener_onPlayerJoinsRoom, this, std::placeholders::_1)),
			handler_onPlayerLeavesRoom(std::bind(&ProxyRoom::listener_onPlayerLeavesRoom, this, std::placeholders::_1)),
			handler_onPlayerJoinsNextRound(std::bind(&ProxyRoom::listener_onPlayerJoinsNextGame, this, std::placeholders::_1)),
			handler_onPlayerLeavesNextRound(std::bind(&ProxyRoom::listener_onPlayerLeavesNextGame, this, std::placeholders::_1)),
			handler_onOptionsWereChanged(std::bind(&ProxyRoom::listener_onOptionsWereChanged, this, std::placeholders::_1)),
			handler_onRoomLeaderChange(std::bind(&ProxyRoom::listener_onRoomLeaderChange, this, std::placeholders::_1)),
			handler_onGameStart(std::bind(&ProxyRoom::listener_onGameStart, this, std::placeholders::_1)) {

		packetTransmitter->addListenerForServerPkt(OtherPlayerHasJoinedRoom_STCPacket::PACKET_ID, handler_onPlayerJoinsRoom);
		packetTransmitter->addListenerForServerPkt(OtherPlayerHasLeavedRoom_STCPacket::PACKET_ID, handler_onPlayerLeavesRoom);
		packetTransmitter->addListenerForServerPkt(PlayerWantsJoinNextGame_STCPacket::PACKET_ID, handler_onPlayerJoinsNextRound);
		packetTransmitter->addListenerForServerPkt(PlayerWantsLeaveNextGame_STCPacket::PACKET_ID, handler_onPlayerLeavesNextRound);
		packetTransmitter->addListenerForServerPkt(OptionsWereChanged_STCPacket::PACKET_ID, handler_onOptionsWereChanged);
		packetTransmitter->addListenerForServerPkt(RoomLeaderHasChanged_STCPacket::PACKET_ID, handler_onRoomLeaderChange);
		packetTransmitter->addListenerForServerPkt(GameHasBeenStarted_STCPacket::PACKET_ID, handler_onGameStart);

		// init opponents
		if(opponentsUsernames.size() != opponentAvatars.size()) {
			throw std::runtime_error("Size of opponent usernames and avatars is not equal");
		}
		for(int i = 0; i < opponentsUsernames.size(); i++) {
			auto name = opponentsUsernames[i];
			auto avatar = opponentAvatars[i];

			this->joinPlayerLocal(name, avatar);
		}

		// init local player
		this->localParticipant = std::make_shared<ParticipantOnClient>(localPlayerUsername, localPlayerAvatar);
		allParticipants.push_back(localParticipant);
		participantsForNextGame.push_back(localParticipant);

		// init leader
		this->roomLeader = lookupParticipant(usernameOfLeader);
	}

	ProxyRoom::~ProxyRoom() {
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasJoinedRoom_STCPacket::PACKET_ID, handler_onPlayerJoinsRoom);
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasLeavedRoom_STCPacket::PACKET_ID, handler_onPlayerLeavesRoom);
		packetTransmitter->removeListenerForServerPkt(PlayerWantsJoinNextGame_STCPacket::PACKET_ID, handler_onPlayerJoinsNextRound);
		packetTransmitter->removeListenerForServerPkt(PlayerWantsLeaveNextGame_STCPacket::PACKET_ID, handler_onPlayerLeavesNextRound);
		packetTransmitter->removeListenerForServerPkt(OptionsWereChanged_STCPacket::PACKET_ID, handler_onOptionsWereChanged);
		packetTransmitter->removeListenerForServerPkt(RoomLeaderHasChanged_STCPacket::PACKET_ID, handler_onRoomLeaderChange);
		packetTransmitter->removeListenerForServerPkt(GameHasBeenStarted_STCPacket::PACKET_ID, handler_onGameStart);
	}

	bool ProxyRoom::isGameRunning() const {
		return game != nullptr;
	}

	ProxyMauMauGame& ProxyRoom::getGame() {
		if(! isGameRunning()) throw std::runtime_error("There is no game running at the moment!");
		return *game;
	}

	bool ProxyRoom::isLocalUserLeader() const {
		return roomLeader == localParticipant;
	}

	bool ProxyRoom::isLocalUserParticipantInNextGame() const {
		return std::find(participantsForNextGame.begin(), participantsForNextGame.end(), localParticipant) != participantsForNextGame.end();
	}

	bool ProxyRoom::isUsernameAvailable(std::string username) const {
		for(auto& p : allParticipants) {
			if(p->getUsername() == username) return false;
		}
		return true;
	}

	bool ProxyRoom::isParticipantInNextGame(std::string username) const {
		for(auto& p : participantsForNextGame) {
			if(p->getUsername() == username) return true;
		}

		return false;
	}

	bool ProxyRoom::isParticipantInNextGame(const std::shared_ptr<ParticipantOnClient>& participant) {
		return std::find(allParticipants.begin(), allParticipants.end(), participant) != allParticipants.end();
	}

	std::shared_ptr<ParticipantOnClient> ProxyRoom::lookupParticipant(std::string username) {
		for(auto& o : allParticipants) {
			if(o->getUsername() == username) return o;
		}
		throw PlayerNotFoundException("Player \"" + username + "\" is not an opponent.");

	}

	bool ProxyRoom::canStartGame() {
		return isLocalUserLeader() && ! isGameRunning() && participantsForNextGame.size() > 1 && !wasGameStartRequested;
	}

	void ProxyRoom::requestGameStart() {
		if(! canStartGame()) throw std::runtime_error("Can't start game!");
		wasGameStartRequested = true;

		GameStartRequest_CTSPacket packet;
		packetTransmitter->sendPacketToServer(packet);
	}

	bool ProxyRoom::canJoinQueueForNextGame() {
		return !isLocalUserParticipantInNextGame() && !wasParticipanceInNextGameRequested;
	}

	void ProxyRoom::requestJoinQueueForNextGame() {
		if(! canJoinQueueForNextGame()) throw std::runtime_error("Can't participate in next game.");
		wasParticipanceInNextGameRequested = true;

		JoinNextGameRequest_CTSPacket packet;
		packetTransmitter->sendPacketToServer(packet);
	}

	bool ProxyRoom::canLeaveQueueForNextGame() {
		return isLocalUserParticipantInNextGame() && !wasAbsenceInNextGameRequested;
	}

	void ProxyRoom::requestLeaveQueueForNextGame() {
		if(! canLeaveQueueForNextGame()) throw std::runtime_error("Can't leave queue for next game.");
		wasAbsenceInNextGameRequested = true;

		LeaveNextGameRequest_CTSPacket packet;
		packetTransmitter->sendPacketToServer(packet);
		
	}

	bool ProxyRoom::canBeRoomLeader(std::string username) {
		return isLocalUserLeader() && lookupParticipant(username) != roomLeader && !wasRoomLeaderChangeRequested;
	}

	void ProxyRoom::requestRoomLeaderChange(std::string newRoomLeaderUsername) {
		if(! canBeRoomLeader(newRoomLeaderUsername)) throw std::runtime_error("Can't change room leader to this player.");
		wasRoomLeaderChangeRequested = true;

		ChangeRoomLeaderRequest_CTSPacket packet(newRoomLeaderUsername);
		packetTransmitter->sendPacketToServer(packet);
	}

	void ProxyRoom::joinPlayerLocal(std::string username, Avatar avatar) {
		auto& newParticipant = std::make_shared<ParticipantOnClient>(username, avatar);
		allParticipants.push_back(newParticipant);
		participantsForNextGame.push_back(newParticipant);
	}

	void ProxyRoom::kickPlayerLocal(std::string username) {
		std::shared_ptr<ParticipantOnClient> player = lookupParticipant(username);
		allParticipants.erase(std::find(allParticipants.begin(), allParticipants.end(), player));
		participantsForNextGame.erase(std::find(participantsForNextGame.begin(), participantsForNextGame.end(), player));

		if(isGameRunning() && getGame().checkIfIsOpponent(username)) getGame().removeOpponentLocal(player);
	}

	void ProxyRoom::joinQueueForNextGameLocal(std::string username) {
		std::shared_ptr<ParticipantOnClient> participant = lookupParticipant(username);
		if(! isParticipantInNextGame(participant)) {
			participantsForNextGame.push_back(participant);
		}

		wasParticipanceInNextGameRequested = false;
	}

	void ProxyRoom::leaveQueueForNextGameLocal(std::string username) {
		std::shared_ptr<ParticipantOnClient> participant = lookupParticipant(username);

		auto& ptrInList = std::find(participantsForNextGame.begin(), participantsForNextGame.end(), participant);
		if(ptrInList != participantsForNextGame.end()) {
			participantsForNextGame.erase(ptrInList);
		}

		wasAbsenceInNextGameRequested = false;
	}

	void ProxyRoom::changeRoomLeaderLocal(std::string newLeaderUsername) {
		std::shared_ptr<ParticipantOnClient> player = lookupParticipant(newLeaderUsername);
		this->roomLeader = player;

		wasRoomLeaderChangeRequested = false;
	}

	void ProxyRoom::startGameLocal(std::vector<std::string> usernamesOfAllPlayers, std::string usernameOnTurn, std::vector<int> handCards, int startCard, int nextCardOnDrawStack) {
		std::vector<std::shared_ptr<ParticipantOnClient>> opponents;
		for(auto& username : usernamesOfAllPlayers) {
			opponents.push_back(lookupParticipant(username));
		}

		this->game = std::make_unique<ProxyMauMauGame>(packetTransmitter, opponents, localParticipant, usernameOnTurn, handCards, startCard, nextCardOnDrawStack, options);
		
		wasGameStartRequested = false;
	}

	void ProxyRoom::listener_onPlayerJoinsRoom(Packet& p) {
		auto& casted = dynamic_cast<OtherPlayerHasJoinedRoom_STCPacket&>(p);
		joinPlayerLocal(casted.getUsername(), casted.getAvatar());
	}

	void ProxyRoom::listener_onPlayerLeavesRoom(Packet& p) {
		auto& casted = dynamic_cast<OtherPlayerHasLeavedRoom_STCPacket&>(p);
		kickPlayerLocal(casted.getUsername());
	}

	void ProxyRoom::listener_onPlayerJoinsNextGame(Packet& p) {
		auto& casted = dynamic_cast<PlayerWantsJoinNextGame_STCPacket&>(p);
		joinQueueForNextGameLocal(casted.getUsername());
	}

	void ProxyRoom::listener_onPlayerLeavesNextGame(Packet& p) {
		auto& casted = dynamic_cast<PlayerWantsLeaveNextGame_STCPacket&>(p);
		leaveQueueForNextGameLocal(casted.getUsername());
	}

	void ProxyRoom::listener_onOptionsWereChanged(Packet& p) {
		auto& casted = dynamic_cast<OptionsWereChanged_STCPacket&>(p);
		options.setAllOptions(casted.getOptions());
	}

	void ProxyRoom::listener_onRoomLeaderChange(Packet& p) {
		auto& casted = dynamic_cast<RoomLeaderHasChanged_STCPacket&>(p);
		changeRoomLeaderLocal(casted.getUsername());
	}

	void ProxyRoom::listener_onGameStart(Packet& p) {
		auto& casted = dynamic_cast<GameHasBeenStarted_STCPacket&>(p);
		startGameLocal(casted.getUsernamesOfAllParticipants(), casted.getUsernameOnTurn(), casted.getHandCards(), casted.getStartCard(), casted.getNextCardOnDrawStack());
	}

}