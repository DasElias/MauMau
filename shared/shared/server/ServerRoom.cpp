#include "ServerRoom.h"
#include "../packet/stc/GameHasBeenStarted_STCPacket.h"
#include "../packet/stc/OtherPlayerHasJoinedRoom_STCPacket.h"
#include "../packet/stc/OtherPlayerHasLeavedRoom_STCPacket.h"
#include "../packet/stc/RoomLeaderHasChanged_STCPacket.h"
#include "../packet/stc/OptionsWereChanged_STCPacket.h"


#include "../packet/cts/ChangeRoomLeaderRequest_CTSPacket.h"
#include "../packet/cts/GameStartRequest_CTSPacket.h"
#include "../packet/cts/ChangeOptionsRequest_CTSPacket.h"
#include "../packet/cts/KickPlayerRequest_CTSPacket.h"
#include "../packet/cts/JoinAiPlayerRequest_CTSPacket.h"

#include "../model/PlayerNotFoundException.h"

#include "AiParticipant.h"
#include "RandomStringGenerator.h"
#include "../model/AvatarUtils.h"
#include "../model/MaxParticipants.h"

namespace card {
	ServerRoom::ServerRoom(RoomCode roomCode, std::shared_ptr<STCPacketTransmitter> packetTransmitter, RoomOptions roomOptions) :
			roomCode(roomCode),
			roomOptions(roomOptions),
			packetTransmitter(packetTransmitter),
			handler_onChangeOptions(std::bind(&ServerRoom::listener_onChangeOptions, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onChangeRoomLeader(std::bind(&ServerRoom::listener_onChangeRoomLeader, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onStartGame(std::bind(&ServerRoom::listener_onStartGame, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onKickPlayer(std::bind(&ServerRoom::listener_onKickPlayer, this, std::placeholders::_1, std::placeholders::_2)), 
			handler_onAiPlayerJoin(std::bind(&ServerRoom::listener_onAiPlayerJoin, this, std::placeholders::_1, std::placeholders::_2)) {

		packetTransmitter->addListenerForClientPkt(ChangeOptionsRequest_CTSPacket::PACKET_ID, handler_onChangeOptions);
		packetTransmitter->addListenerForClientPkt(ChangeRoomLeaderRequest_CTSPacket::PACKET_ID, handler_onChangeRoomLeader);
		packetTransmitter->addListenerForClientPkt(GameStartRequest_CTSPacket::PACKET_ID, handler_onStartGame);
		packetTransmitter->addListenerForClientPkt(KickPlayerRequest_CTSPacket::PACKET_ID, handler_onKickPlayer);
		packetTransmitter->addListenerForClientPkt(JoinAiPlayerRequest_CTSPacket::PACKET_ID, handler_onAiPlayerJoin);
	}
	ServerRoom::~ServerRoom() {
		packetTransmitter->removeListenerForClientPkt(ChangeOptionsRequest_CTSPacket::PACKET_ID, handler_onChangeOptions);
		packetTransmitter->removeListenerForClientPkt(ChangeRoomLeaderRequest_CTSPacket::PACKET_ID, handler_onChangeRoomLeader);
		packetTransmitter->removeListenerForClientPkt(GameStartRequest_CTSPacket::PACKET_ID, handler_onStartGame);
		packetTransmitter->removeListenerForClientPkt(KickPlayerRequest_CTSPacket::PACKET_ID, handler_onKickPlayer);
		packetTransmitter->removeListenerForClientPkt(JoinAiPlayerRequest_CTSPacket::PACKET_ID, handler_onAiPlayerJoin);
	}
	RoomCode card::ServerRoom::getRoomCode() const {
		return roomCode;
	}
	const RoomOptions& ServerRoom::getRoomOptions() const {
		return roomOptions;
	}
	bool ServerRoom::isGameRunning() const {
		return game != nullptr;
	}
	ServerMauMauGame& ServerRoom::getGame() {
		return *game;
	}
	bool ServerRoom::checkIfLeader(const std::shared_ptr<ParticipantOnServer>& participant) {
		return roomLeader == participant;
	}
	bool ServerRoom::checkIfLeaderByUsername(std::string username) {
		return roomLeader->getUsername() == username;
	}
	bool ServerRoom::isUsernameAvailable(std::string username) {
		return !checkIfParticipantByUsername(username);
	}
	bool ServerRoom::isRoomFull() {
		return allParticipants.size() >= MAX_PARTICIPANTS;
	}
	bool ServerRoom::shouldCloseRoom() const {
		unsigned int realParticipants = 0;
		for(auto& p : allParticipants) {
			if(p->isRealPlayer()) realParticipants++;
		}
		return realParticipants == 0;
	}
	bool ServerRoom::checkIfParticipant(const std::shared_ptr<ParticipantOnServer>& participant) {
		return std::find(allParticipants.begin(), allParticipants.end(), participant) != allParticipants.end();
	}
	bool ServerRoom::checkIfParticipantByUsername(std::string username) {
		for(auto& p : allParticipants) {
			if(p->getUsername() == username) return true;
		}
		return false;
	}
	std::shared_ptr<ParticipantOnServer> ServerRoom::lookupParticipantByUsername(std::string username) {
		for(auto& o : allParticipants) {
			if(o->getUsername() == username) return o;
		}
		throw PlayerNotFoundException("Player \"" + username + "\" is not an opponent.");
	}
	std::shared_ptr<ParticipantOnServer> ServerRoom::getRoomLeader() {
		return roomLeader;
	}
	std::shared_ptr<ParticipantOnServer> ServerRoom::getNextParticipant(std::shared_ptr<ParticipantOnServer> participant) {
		auto playerOnTurnIter = std::find(allParticipants.begin(), allParticipants.end(), participant);

		playerOnTurnIter++;
		if(playerOnTurnIter == allParticipants.end()) playerOnTurnIter = allParticipants.begin();

		return *playerOnTurnIter;
	}
	std::vector<std::shared_ptr<ParticipantOnServer>> ServerRoom::getParticipants() {
		return allParticipants;
	}
	std::vector<std::string> ServerRoom::getUsernamesOfParticipants() {
		return ParticipantOnServer::getVectorWithUsernames(allParticipants);
	}
	bool ServerRoom::changeOptions(const std::shared_ptr<ParticipantOnServer>& sender, std::map<std::string, int> options) {
		if(! checkIfLeader(sender) || isGameRunning()) return false;

		OptionsWereChanged_STCPacket packet(options);
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		this->roomOptions.setAllOptions(options);

		return true;
	}
	bool ServerRoom::joinRoom(const std::shared_ptr<ParticipantOnServer>& constructedParticipant) {
		auto username = constructedParticipant->getUsername();
		if(!isUsernameAvailable(username) || isRoomFull()) return false;

		OtherPlayerHasJoinedRoom_STCPacket packet(username, constructedParticipant->getAvatar(), false);
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		allParticipants.push_back(constructedParticipant);

		return true;
	}
	bool ServerRoom::joinRoomAiPlayer(std::string reservedUsername) {
		if(allParticipants.size() >= MAX_PARTICIPANTS) return false;

		auto allReservedUsernames = getUsernamesOfParticipants();
		if(! reservedUsername.empty()) allReservedUsernames.push_back(reservedUsername);

		bool isMale;
		std::string username = generateUsernameNotIn(allReservedUsernames, isMale);
		Avatar avatar = getRandomAvatar(isMale);
		OtherPlayerHasJoinedRoom_STCPacket packet(username, avatar, true);
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		auto newParticipant = std::make_shared<AiParticipant>(username, avatar);
		allParticipants.push_back(newParticipant);

		return true;
	}
	bool ServerRoom::leaveRoom(std::shared_ptr<ParticipantOnServer> participant, bool wasKickedByOtherPlayer) {
		if(! checkIfParticipant(participant)) return false;

		if(participant == roomLeader) {
			auto newRoomLeader = getNextParticipant(roomLeader);
			changeRoomLeader(newRoomLeader);
		}

		allParticipants.erase(std::remove(allParticipants.begin(), allParticipants.end(), participant), allParticipants.end());
		if(isGameRunning() && getGame().checkIfPlayerByParticipant(participant)) {
			auto& game = getGame();
			auto player = game.lookupPlayerByParticipant(participant);
			game.removePlayer(player);
		};

		OtherPlayerHasLeavedRoom_STCPacket packet(participant->getUsername(), wasKickedByOtherPlayer);
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		return true;
	}
	bool card::ServerRoom::changeRoomLeader(const std::shared_ptr<ParticipantOnServer>& sender, std::string usernameOfNewLeader) {
		if(! checkIfLeader(sender) || !checkIfParticipantByUsername(usernameOfNewLeader)) return false;

		auto newLeader = lookupParticipantByUsername(usernameOfNewLeader);
		changeRoomLeader(newLeader);

		return true;
	}
	void ServerRoom::changeRoomLeader(const std::shared_ptr<ParticipantOnServer>& newRoomLeader) {
		RoomLeaderHasChanged_STCPacket packet(newRoomLeader->getUsername());
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		initRoomLeaderWithoutPermissionsChecking(newRoomLeader);
	}
	void ServerRoom::initRoomLeaderWithoutPermissionsChecking(const std::shared_ptr<ParticipantOnServer>& newLeader) {
		this->roomLeader = newLeader;
	}
	bool ServerRoom::startGame(const std::shared_ptr<ParticipantOnServer>& sender) {
		if(! checkIfLeader(sender) || isGameRunning() || allParticipants.size() <= 1) return false;

		// create game
		this->game = std::make_unique<ServerMauMauGame>(packetTransmitter, *this, allParticipants, roomOptions);

		// send packets
		std::vector<std::string> usernameOfAllPlayers = getUsernamesOfParticipants();
		auto playerOnTurn = game->getPlayerOnTurn();
		std::string usernameOfPlayerOnTurn = playerOnTurn->getUsername();
		int cardNumberOfFirstCardOnPlayStack = game->getPlayCardStack().getLast().getCardNumber();

		for(auto& participant : allParticipants) {
			auto player = game->lookupPlayerByUsername(participant->getUsername());
			std::vector<int> handCards = player->getHandCards().getCardNumbers();
			Card nextCardOnDrawStack = (participant->getUsername() == game->getPlayerOnTurn()->getUsername()) ? game->getDrawCardStack().getLast() : Card::NULLCARD;

			GameHasBeenStarted_STCPacket packet(usernameOfAllPlayers, usernameOfPlayerOnTurn, handCards, cardNumberOfFirstCardOnPlayStack, nextCardOnDrawStack.getCardNumber());
			packetTransmitter->sendPacketToClient(packet, participant);
		}
		return true;
	}
	void ServerRoom::onGameEnd() {
		this->game = nullptr;
	}
	optionalSuccessAnswerPacket ServerRoom::listener_onChangeOptions(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<ChangeOptionsRequest_CTSPacket&>(p);

		bool wasSuccessful = changeOptions(participant, casted.getOptions());
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	optionalSuccessAnswerPacket ServerRoom::listener_onChangeRoomLeader(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(! checkIfParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<ChangeRoomLeaderRequest_CTSPacket&>(p);

		bool wasSuccessful = changeRoomLeader(participant, casted.getUsernameOfNewLeader());
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	optionalSuccessAnswerPacket ServerRoom::listener_onStartGame(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<GameStartRequest_CTSPacket&>(p);

		bool wasSuccessful = startGame(participant);
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	optionalSuccessAnswerPacket ServerRoom::listener_onKickPlayer(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& sender) {
		if(! checkIfParticipant(sender)) return std::nullopt;
		auto& casted = dynamic_cast<KickPlayerRequest_CTSPacket&>(p);
		auto participantToKick = lookupParticipantByUsername(casted.getUsernameOfPlayerToKick());

		bool wasSuccessful = leaveRoom(participantToKick, true);
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	optionalSuccessAnswerPacket ServerRoom::listener_onAiPlayerJoin(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(! checkIfParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<JoinAiPlayerRequest_CTSPacket&>(p);

		bool wasSuccessful = joinRoomAiPlayer();
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
}