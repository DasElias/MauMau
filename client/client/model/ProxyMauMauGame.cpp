#include "ProxyMauMauGame.h"

namespace card {
	ProxyMauMauGame::ProxyMauMauGame(std::shared_ptr<CTSPacketTransmitter> packetTransmitter, std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, 
									std::shared_ptr<ParticipantOnClient> localParticipant, std::vector<int> handCards, 
									int startCard, RoomOptions& roomOptions) :
											gameData(allParticipantsInclLocal, localParticipant, handCards, startCard, roomOptions,
												[this](std::shared_ptr<ProxyPlayer> turnEndPlayer) {
													accessorFromClient.onTurnEnd();
												}
											),
											accessorFromClient(gameData, packetTransmitter),
											accessorFromServer(gameData, packetTransmitter) {
	}
	const ProxyMauMauGameData& ProxyMauMauGame::getGameData() const {
		return gameData;
	}
	MauMauGameAccessorFromClient& ProxyMauMauGame::getAccessorFromClient() {
		return accessorFromClient;
	}
	const CardAnimator& ProxyMauMauGame::getDrawStack() const {
		return gameData.getDrawStack();
	}
	const CardAnimator& ProxyMauMauGame::getPlayStack() const {
		return gameData.getPlayStack();
	}
	std::shared_ptr<LocalPlayer> ProxyMauMauGame::getLocalPlayer() {
		return gameData.getLocalPlayer();
	}
	std::vector<std::shared_ptr<ProxyPlayer>> ProxyMauMauGame::getOpponents() {
		return gameData.getOpponents();
	}
	void ProxyMauMauGame::removeOpponentLocal(std::shared_ptr<ParticipantOnClient> player) {
		gameData.removeOpponentLocal(player);
	}
	bool ProxyMauMauGame::checkIfIsOpponent(std::string username) const {
		return gameData.checkIfIsOpponent(username);
	}
	bool ProxyMauMauGame::hasGameEnded() const {
		return gameData.hasGameEnded();
	}
	bool ProxyMauMauGame::hasInitialCardBeenDistributed() const {
		return gameData.hasInitialCardBeenDistributed();
	}
	std::shared_ptr<ProxyPlayer> ProxyMauMauGame::getWinnerOrNull() {
		return gameData.getWinnerOrNull();
	}
}