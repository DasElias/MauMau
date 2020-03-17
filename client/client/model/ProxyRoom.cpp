#include "ProxyRoom.h"
#include <stdexcept>
#include <shared/model/PlayerNotFoundException.h>
#include <shared/model/MaxParticipants.h>

#include <shared/packet/stc/OtherPlayerHasJoinedRoom_STCPacket.h>
#include <shared/packet/stc/OtherPlayerHasLeavedRoom_STCPacket.h>
#include <shared/packet/stc/OptionsWereChanged_STCPacket.h>
#include <shared/packet/stc/RoomLeaderHasChanged_STCPacket.h>
#include <shared/packet/stc/GameHasBeenStarted_STCPacket.h>
#include <shared/packet/stc/GameWasAborted_STCPacket.h>

#include <shared/packet/cts/GameStartRequest_CTSPacket.h>
#include <shared/packet/cts/ChangeRoomLeaderRequest_CTSPacket.h>
#include <shared/packet/cts/ChangeOptionsRequest_CTSPacket.h>
#include <shared/packet/cts/KickPlayerRequest_CTSPacket.h>
#include <shared/packet/cts/JoinAiPlayerRequest_CTSPacket.h>

namespace card {
	ProxyRoom::ProxyRoom(std::shared_ptr<CTSPacketTransmitter> packetTransmitter, AbstractRoomLeaveHandler& roomLeaveHandler, AbstractReturnBackToMenuHandler& gameEndHandler, RoomCode roomCode, std::vector<std::string> opponentUsernames, std::vector<Avatar> opponentAvatars, std::vector<bool> areOpponentsAiPlayers, std::string localPlayerUsername, Avatar localPlayerAvatar, std::string usernameOfLeader, RoomOptions options) :
			packetTransmitter(packetTransmitter),
			roomLeaveHandler(roomLeaveHandler),
			gameEndHandler(gameEndHandler),
			roomCode(roomCode),
			options(options),

			handler_onPlayerJoinsRoom(std::bind(&ProxyRoom::listener_onPlayerJoinsRoom, this, std::placeholders::_1)),
			handler_onOtherPlayerLeavesRoom(std::bind(&ProxyRoom::listener_onOtherPlayerLeavesRoom, this, std::placeholders::_1)), 
			handler_onOptionsWereChanged(std::bind(&ProxyRoom::listener_onOptionsWereChanged, this, std::placeholders::_1)),
			handler_onRoomLeaderChange(std::bind(&ProxyRoom::listener_onRoomLeaderChange, this, std::placeholders::_1)),
			handler_onGameStart(std::bind(&ProxyRoom::listener_onGameStart, this, std::placeholders::_1)),
			handler_onGameAbort(std::bind(&ProxyRoom::listener_onGameAbort, this, std::placeholders::_1)) {

		packetTransmitter->addListenerForServerPkt(OtherPlayerHasJoinedRoom_STCPacket::PACKET_ID, handler_onPlayerJoinsRoom);
		packetTransmitter->addListenerForServerPkt(OtherPlayerHasLeavedRoom_STCPacket::PACKET_ID, handler_onOtherPlayerLeavesRoom);
		packetTransmitter->addListenerForServerPkt(OptionsWereChanged_STCPacket::PACKET_ID, handler_onOptionsWereChanged);
		packetTransmitter->addListenerForServerPkt(RoomLeaderHasChanged_STCPacket::PACKET_ID, handler_onRoomLeaderChange);
		packetTransmitter->addListenerForServerPkt(GameHasBeenStarted_STCPacket::PACKET_ID, handler_onGameStart);
		packetTransmitter->addListenerForServerPkt(GameWasAborted_STCPacket::PACKET_ID, handler_onGameAbort);

		if(opponentUsernames.size() != opponentAvatars.size() || opponentUsernames.size() != areOpponentsAiPlayers.size()) {
			throw std::runtime_error("The vectors as arguments for opponentUsernames, opponentAvatars and areOpponentsAiPlayers must be of the same length.");
		}
		for(int i = 0; i < opponentUsernames.size(); i++) {
			joinPlayerLocal(opponentUsernames[i], opponentAvatars[i], areOpponentsAiPlayers[i]);
		}

		// init local player
		this->localParticipant = std::make_shared<ParticipantOnClient>(localPlayerUsername, localPlayerAvatar, false);
		allParticipants.push_back(localParticipant);

		// init leader
		this->roomLeader = lookupParticipant(usernameOfLeader);
	}

	ProxyRoom::~ProxyRoom() {
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasJoinedRoom_STCPacket::PACKET_ID, handler_onPlayerJoinsRoom);
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasLeavedRoom_STCPacket::PACKET_ID, handler_onOtherPlayerLeavesRoom);
		packetTransmitter->removeListenerForServerPkt(OptionsWereChanged_STCPacket::PACKET_ID, handler_onOptionsWereChanged);
		packetTransmitter->removeListenerForServerPkt(RoomLeaderHasChanged_STCPacket::PACKET_ID, handler_onRoomLeaderChange);
		packetTransmitter->removeListenerForServerPkt(GameHasBeenStarted_STCPacket::PACKET_ID, handler_onGameStart);
		packetTransmitter->removeListenerForServerPkt(GameWasAborted_STCPacket::PACKET_ID, handler_onGameAbort);

	}

	bool ProxyRoom::isGameRunning() const {
		return game != nullptr;
	}

	ProxyMauMauGame& ProxyRoom::getGame() {
		if(! isGameRunning()) throw std::runtime_error("There is no game running at the moment!");
		return *game;
	}

	bool ProxyRoom::isWaitingForResponse() const {
		return isWaitingForResponse_field;
	}

	bool ProxyRoom::isLocalUserLeader() const {
		return roomLeader == localParticipant;
	}

	bool ProxyRoom::isLeader(std::string username) {
		auto user = lookupParticipant(username);
		return isLeader(user);
	}

	bool ProxyRoom::isLeader(std::shared_ptr<ParticipantOnClient> user) {
		return user == roomLeader;
	}

	bool ProxyRoom::isLocalUser(std::shared_ptr<ParticipantOnClient> user) {
		return localParticipant == user;
	}

	bool ProxyRoom::isUsernameAvailable(std::string username) const {
		for(auto& p : allParticipants) {
			if(p->getUsername() == username) return false;
		}
		return true;
	}

	std::shared_ptr<ParticipantOnClient> ProxyRoom::lookupParticipant(std::string username) {
		for(auto& o : allParticipants) {
			if(o->getUsername() == username) return o;
		}
		throw PlayerNotFoundException("Player \"" + username + "\" is not an opponent.");

	}

	const std::vector<std::shared_ptr<ParticipantOnClient>>& ProxyRoom::getAllParticipants() {
		return allParticipants;
	}

	std::shared_ptr<ParticipantOnClient> ProxyRoom::getLocalParticipant() {
		return localParticipant;
	}

	std::shared_ptr<ParticipantOnClient> ProxyRoom::getRoomLeader() {
		return roomLeader;
	}

	const RoomOptions& ProxyRoom::getOptions() const {
		return options;
	}

	RoomCode ProxyRoom::getRoomCode() const {
		return roomCode;
	}

	bool ProxyRoom::canStartGame() {
		return isLocalUserLeader() && ! isGameRunning() && allParticipants.size() > 1;
	}

	void ProxyRoom::requestGameStart() {
		if(! canStartGame()) throw std::runtime_error("Can't start game!");
		isWaitingForResponse_field = true;

		GameStartRequest_CTSPacket packet;
		packetTransmitter->sendPacketToServer(packet);
	}

	bool ProxyRoom::canBeRoomLeader(std::shared_ptr<ParticipantOnClient> user) {
		return isLocalUserLeader() && user != roomLeader && !user->isAiPlayer();
	}

	bool ProxyRoom::canBeKicked(std::shared_ptr<ParticipantOnClient> user) {
		return isLocalUserLeader() && user != localParticipant;
	}

	void ProxyRoom::requestKickPlayer(std::shared_ptr<ParticipantOnClient> playerToKick) {
		std::string username = playerToKick->getUsername();
		if(! canBeKicked(playerToKick)) throw std::runtime_error("Can't kick player: '" + username + "'");
		isWaitingForResponse_field = true;

		KickPlayerRequest_CTSPacket packet(username);
		packetTransmitter->sendPacketToServer(packet);
	}

	void ProxyRoom::requestOptionChange(RoomOptions options) {
		if(! isLocalUserLeader()) throw std::runtime_error("Can't change options since the local player is not the room leader.");
		isWaitingForResponse_field = true;

		ChangeOptionsRequest_CTSPacket packet(options.getAllOptions());
		packetTransmitter->sendPacketToServer(packet);
	}

	bool ProxyRoom::canAiPlayerJoin() {
		return isLocalUserLeader() && allParticipants.size() < MAX_PARTICIPANTS;
	}

	void ProxyRoom::requestAiPlayerJoin() {
		if(! canAiPlayerJoin()) throw std::runtime_error("Can't add AI player to room!");
		isWaitingForResponse_field = true;

		JoinAiPlayerRequest_CTSPacket packet;
		packetTransmitter->sendPacketToServer(packet);
	}

	void ProxyRoom::requestLeave() {
		isWaitingForResponse_field = true;
		roomLeaveHandler();
/*		RoomLeaveRequest_CTSPacket packet;
		packetTransmitter->sendPacketToServer(packet);*/
	}

	void ProxyRoom::requestRoomLeaderChange(std::shared_ptr<ParticipantOnClient> newRoomLeader) {
		if(! canBeRoomLeader(newRoomLeader)) throw std::runtime_error("Can't change room leader to this player.");
		isWaitingForResponse_field = true;

		ChangeRoomLeaderRequest_CTSPacket packet(newRoomLeader->getUsername());
		packetTransmitter->sendPacketToServer(packet);
	}

	void ProxyRoom::joinPlayerLocal(std::string username, Avatar avatar, bool isAiPlayer) {
		auto& newParticipant = std::make_shared<ParticipantOnClient>(username, avatar, isAiPlayer);
		allParticipants.push_back(newParticipant);

		// if an ai player has joined the game, we have requested to add it to the game
		if(isAiPlayer) isWaitingForResponse_field = false;
	}

	void ProxyRoom::kickOtherPlayerLocal(std::string username) {
		std::shared_ptr<ParticipantOnClient> player = lookupParticipant(username);
		allParticipants.erase(std::find(allParticipants.begin(), allParticipants.end(), player));

		if(isGameRunning() && getGame().checkIfIsOpponent(username)) getGame().removeOpponentLocal(player);
	}

	void ProxyRoom::changeOptionsLocal(RoomOptions newOptions) {
		this->options.setAllOptions(newOptions.getAllOptions());
		isWaitingForResponse_field = false;
	}

	void ProxyRoom::changeRoomLeaderLocal(std::string newLeaderUsername) {
		std::shared_ptr<ParticipantOnClient> player = lookupParticipant(newLeaderUsername);
		this->roomLeader = player;

		isWaitingForResponse_field = false;
	}

	void ProxyRoom::startGameLocal(std::vector<std::string> usernamesOfAllPlayers, std::vector<int> handCards, int startCard) {
		std::vector<std::shared_ptr<ParticipantOnClient>> opponents;
		for(auto& username : usernamesOfAllPlayers) {
			opponents.push_back(lookupParticipant(username));
		}

		this->game = std::make_unique<ProxyMauMauGame>(packetTransmitter, opponents, localParticipant, handCards, startCard, options);
		
		isWaitingForResponse_field = false;
	}

	void ProxyRoom::returnBackToMenu(ReturnBackToMenuCause cause) {
		gameEndHandler.returnBackToMenu(cause);
		game = nullptr;
	}

	void ProxyRoom::listener_onPlayerJoinsRoom(Packet& p) {
		auto& casted = dynamic_cast<OtherPlayerHasJoinedRoom_STCPacket&>(p);
		joinPlayerLocal(casted.getUsername(), casted.getAvatar(), casted.isAiPlayer());
	}

	void ProxyRoom::listener_onOtherPlayerLeavesRoom(Packet& p) {
		auto& casted = dynamic_cast<OtherPlayerHasLeavedRoom_STCPacket&>(p);
		kickOtherPlayerLocal(casted.getUsername());
		if(casted.wasKicked()) isWaitingForResponse_field = false;
	}

	void ProxyRoom::listener_onOptionsWereChanged(Packet& p) {
		auto& casted = dynamic_cast<OptionsWereChanged_STCPacket&>(p);
		changeOptionsLocal({casted.getOptions()});
	}

	void ProxyRoom::listener_onRoomLeaderChange(Packet& p) {
		auto& casted = dynamic_cast<RoomLeaderHasChanged_STCPacket&>(p);		
		isWaitingForResponse_field = false;

		changeRoomLeaderLocal(casted.getUsername());
	}

	void ProxyRoom::listener_onGameStart(Packet& p) {
		auto& casted = dynamic_cast<GameHasBeenStarted_STCPacket&>(p);
		isWaitingForResponse_field = false;

		startGameLocal(casted.getUsernamesOfAllParticipants(), casted.getHandCards(), casted.getStartCard());
	}

	void ProxyRoom::listener_onGameAbort(Packet& p) {
		auto& casted = dynamic_cast<GameWasAborted_STCPacket&>(p);
		returnBackToMenu(ReturnBackToMenuCause::GAME_WAS_ABORTED);
	}

}