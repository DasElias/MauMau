#include "ServerRoom.h"
#include "../packet/stc/GameHasBeenStarted_STCPacket.h"
#include "../packet/stc/GameHasStartedButNotParticipant_STCPacket.h"
#include "../packet/stc/OtherPlayerHasJoinedRoom_STCPacket.h"
#include "../packet/stc/OtherPlayerHasLeavedRoom_STCPacket.h"
#include "../packet/stc/PlayerWantsJoinNextGame_STCPacket.h"
#include "../packet/stc/PlayerWantsLeaveNextGame_STCPacket.h"
#include "../packet/stc/RoomLeaderHasChanged_STCPacket.h"
#include "../packet/stc/OptionsWereChanged_STCPacket.h"


#include "../packet/cts/ChangeRoomLeaderRequest_CTSPacket.h"
#include "../packet/cts/GameStartRequest_CTSPacket.h"
#include "../packet/cts/JoinNextGameRequest_CTSPacket.h"
#include "../packet/cts/LeaveNextGameRequest_CTSPacket.h"
#include "../packet/cts/ChangeOptionsRequest_CTSPacket.h"

#include "../model/PlayerNotFoundException.h"

#include "AiParticipant.h"
#include "RandomStringGenerator.h"
#include "../model/AvatarUtils.h"

namespace card {
	ServerRoom::ServerRoom(RoomCode roomCode, std::shared_ptr<STCPacketTransmitter> packetTransmitter, RoomOptions roomOptions) :
			roomCode(roomCode),
			roomOptions(roomOptions),
			packetTransmitter(packetTransmitter),
			handler_onChangeOptions(std::bind(&ServerRoom::listener_onChangeOptions, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onChangeRoomLeader(std::bind(&ServerRoom::listener_onChangeRoomLeader, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onStartGame(std::bind(&ServerRoom::listener_onStartGame, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onJoinNextGame(std::bind(&ServerRoom::listener_onJoinNextGame, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onLeaveNextGame(std::bind(&ServerRoom::listener_onLeaveNextGame, this, std::placeholders::_1, std::placeholders::_2)) {

		packetTransmitter->addListenerForClientPkt(ChangeOptionsRequest_CTSPacket::PACKET_ID, handler_onChangeOptions);
		packetTransmitter->addListenerForClientPkt(ChangeRoomLeaderRequest_CTSPacket::PACKET_ID, handler_onChangeRoomLeader);
		packetTransmitter->addListenerForClientPkt(GameStartRequest_CTSPacket::PACKET_ID, handler_onStartGame);
		packetTransmitter->addListenerForClientPkt(JoinNextGameRequest_CTSPacket::PACKET_ID, handler_onJoinNextGame);
		packetTransmitter->addListenerForClientPkt(LeaveNextGameRequest_CTSPacket::PACKET_ID, handler_onLeaveNextGame);
	}
	ServerRoom::~ServerRoom() {
		packetTransmitter->removeListenerForClientPkt(ChangeOptionsRequest_CTSPacket::PACKET_ID, handler_onChangeOptions);
		packetTransmitter->removeListenerForClientPkt(ChangeRoomLeaderRequest_CTSPacket::PACKET_ID, handler_onChangeRoomLeader);
		packetTransmitter->removeListenerForClientPkt(GameStartRequest_CTSPacket::PACKET_ID, handler_onStartGame);
		packetTransmitter->removeListenerForClientPkt(JoinNextGameRequest_CTSPacket::PACKET_ID, handler_onJoinNextGame);
		packetTransmitter->removeListenerForClientPkt(LeaveNextGameRequest_CTSPacket::PACKET_ID, handler_onLeaveNextGame);
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
	bool ServerRoom::checkIfParticipant(const std::shared_ptr<ParticipantOnServer>& participant) {
		return std::find(allParticipants.begin(), allParticipants.end(), participant) != allParticipants.end();
	}
	bool ServerRoom::checkIfParticipantByUsername(std::string username) {
		for(auto& p : allParticipants) {
			if(p->getUsername() == username) return true;
		}
		return false;
	}
	bool ServerRoom::checkIfParticipantInNextGame(const std::shared_ptr<ParticipantOnServer>& player) {
		return std::find(participantsForNextGame.begin(), participantsForNextGame.end(), player) != participantsForNextGame.end();
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
	std::vector<std::shared_ptr<ParticipantOnServer>> ServerRoom::getParticipants() {
		return allParticipants;
	}
	std::vector<std::string> ServerRoom::getUsernamesOfParticipants() {
		return ParticipantOnServer::getVectorWithUsernames(allParticipants);
	}
	bool ServerRoom::changeOptions(const std::shared_ptr<ParticipantOnServer>& sender, std::map<std::string, int> newNonDefaultOptions) {
		if(! checkIfLeader(sender) || isGameRunning()) return false;

		OptionsWereChanged_STCPacket packet(newNonDefaultOptions);
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		this->roomOptions.setAllOptions(newNonDefaultOptions);

		return true;
	}
	bool ServerRoom::joinRoom(const std::shared_ptr<ParticipantOnServer>& constructedParticipant) {
		auto username = constructedParticipant->getUsername();
		if(!isUsernameAvailable(username) || isRoomFull()) return false;

		OtherPlayerHasJoinedRoom_STCPacket packet(username, constructedParticipant->getAvatar());
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		allParticipants.push_back(constructedParticipant);
		participantsForNextGame.push_back(constructedParticipant);

		return true;
	}
	bool ServerRoom::joinRoomAiPlayer(std::string reservedUsername) {
		if(allParticipants.size() >= MAX_PARTICIPANTS) return false;

		auto allReservedUsernames = getUsernamesOfParticipants();
		if(! reservedUsername.empty()) allReservedUsernames.push_back(reservedUsername);

		bool isMale;
		std::string username = generateUsernameNotIn(allReservedUsernames, isMale);
		Avatar avatar = getRandomAvatar(isMale);
		OtherPlayerHasJoinedRoom_STCPacket packet(username, avatar);
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		auto newParticipant = std::make_shared<AiParticipant>(username, avatar);
		allParticipants.push_back(newParticipant);
		participantsForNextGame.push_back(newParticipant);

		return true;
	}
	bool ServerRoom::leaveRoom(std::shared_ptr<ParticipantOnServer> participant) {
		if(! checkIfParticipant(participant)) return false;

		allParticipants.erase(std::remove(allParticipants.begin(), allParticipants.end(), participant), allParticipants.end());
		participantsForNextGame.erase(std::remove(participantsForNextGame.begin(), participantsForNextGame.end(), participant), participantsForNextGame.end());
		if(isGameRunning() && getGame().checkIfPlayerByParticipant(participant)) {
			auto& game = getGame();
			auto player = game.lookupPlayerByParticipant(participant);
			game.removePlayer(player);
		};

		// packet must be send after the player has been removed
		OtherPlayerHasLeavedRoom_STCPacket packet(participant->getUsername());
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		return true;
	}
	bool ServerRoom::joinNextGame(const std::shared_ptr<ParticipantOnServer>& participant) {
		if(! checkIfParticipant(participant) || checkIfParticipantInNextGame(participant)) return false;

		PlayerWantsJoinNextGame_STCPacket packet(participant->getUsername());
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		participantsForNextGame.push_back(participant);

		return true;
	}
	bool ServerRoom::leaveNextGame(const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfParticipant(participant) || !checkIfParticipantInNextGame(participant)) return false;

		PlayerWantsLeaveNextGame_STCPacket packet(participant->getUsername());
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		participantsForNextGame.erase(std::remove(participantsForNextGame.begin(), participantsForNextGame.end(), participant), participantsForNextGame.end());

		return true;
	}
	bool card::ServerRoom::changeRoomLeader(const std::shared_ptr<ParticipantOnServer>& sender, std::string usernameOfNewLeader) {
		if(! checkIfLeader(sender) || !checkIfParticipantByUsername(usernameOfNewLeader)) return false;

		RoomLeaderHasChanged_STCPacket packet(usernameOfNewLeader);
		packetTransmitter->sendPacketToClients(packet, allParticipants);

		auto newLeader = lookupParticipantByUsername(usernameOfNewLeader);
		initRoomLeaderWithoutPermissionsChecking(newLeader);

		return true;
	}
	void ServerRoom::initRoomLeaderWithoutPermissionsChecking(const std::shared_ptr<ParticipantOnServer>& newLeader) {
		this->roomLeader = newLeader;
	}
	bool ServerRoom::startGame(const std::shared_ptr<ParticipantOnServer>& sender) {
		if(! checkIfLeader(sender) || isGameRunning() || participantsForNextGame.size() <= 1) return false;

		// create game
		this->game = std::make_unique<ServerMauMauGame>(packetTransmitter, *this, participantsForNextGame, roomOptions);

		// send packets
		std::vector<std::string> usernameOfAllPlayers = getUsernamesOfParticipants();
		auto playerOnTurn = game->getPlayerOnTurn();
		std::string usernameOfPlayerOnTurn = playerOnTurn->getUsername();
		int cardNumberOfFirstCardOnPlayStack = game->getPlayCardStack().getLast().getCardNumber();

		for(auto& participant : allParticipants) {
			if(checkIfParticipantInNextGame(participant)) {
				auto player = game->lookupPlayerByUsername(participant->getUsername());
				std::vector<int> handCards = player->getHandCards().getCardNumbers();
				Card nextCardOnDrawStack = (participant->getUsername() == game->getPlayerOnTurn()->getUsername()) ? game->getDrawCardStack().getLast() : Card::NULLCARD;

				GameHasBeenStarted_STCPacket packet(usernameOfAllPlayers, usernameOfPlayerOnTurn, handCards, cardNumberOfFirstCardOnPlayStack, nextCardOnDrawStack.getCardNumber());
				packetTransmitter->sendPacketToClient(packet, participant);
			} else {
				GameHasStartedButNotParticipant_STCPacket packet;
				packetTransmitter->sendPacketToClient(packet, participant);
			}

			
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
	optionalSuccessAnswerPacket ServerRoom::listener_onJoinNextGame(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<JoinNextGameRequest_CTSPacket&>(p);

		bool wasSuccessful = joinNextGame(participant);
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	optionalSuccessAnswerPacket ServerRoom::listener_onLeaveNextGame(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<LeaveNextGameRequest_CTSPacket&>(p);

		bool wasSuccessful = leaveNextGame(participant);
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	
}