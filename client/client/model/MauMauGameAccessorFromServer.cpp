#include "MauMauGameAccessorFromServer.h"
#include <shared/packet/stc/PlayerHasMauedSuccessfully_STCPacket.h>
#include <shared/packet/stc/MauPunishment_STCPacket.h>
#include <shared/packet/stc/TurnWasAborted_STCPacket.h>
#include <shared/packet/stc/OtherPlayerHasDrawnCards_STCPacket.h>
#include <shared/packet/stc/OtherPlayerHasPlayedCard_STCPacket.h>
#include <shared/packet/stc/OtherPlayerHasPassed_STCPacket.h>
#include <shared/packet/stc/LocalPlayerIsOnTurn_STCPacket.h>
#include <shared/packet/stc/InitialPlayerIsOnTurn_STCPacket.h>

#include <shared/model/CardAnimationDuration.h>


namespace card {
	MauMauGameAccessorFromServer::MauMauGameAccessorFromServer(ProxyMauMauGameData& gameData, std::shared_ptr<CTSPacketTransmitter> packetTransmitter) :
			gameData(gameData),
			packetTransmitter(packetTransmitter),
			handler_onOtherPlayerHasDrawnCard(std::bind(&MauMauGameAccessorFromServer::listener_onOtherPlayerHasDrawnCard, this, std::placeholders::_1)),
			handler_onOtherPlayerHasPlayedCard(std::bind(&MauMauGameAccessorFromServer::listener_onOtherPlayerHasPlayedCard, this, std::placeholders::_1)),
			handler_onOtherPlayerHasPassed(std::bind(&MauMauGameAccessorFromServer::listener_onOtherPlayerHasPassed, this, std::placeholders::_1)),
			handler_onLocalPlayerIsOnTurn(std::bind(&MauMauGameAccessorFromServer::listener_onLocalPlayerIsOnTurn, this, std::placeholders::_1)),
			handler_onTimeExpires(std::bind(&MauMauGameAccessorFromServer::listener_onTimeExpires, this, std::placeholders::_1)),
			handler_onMauPunishment(std::bind(&MauMauGameAccessorFromServer::listener_onMauPunishment, this, std::placeholders::_1)),
			handler_onOtherPlayerHasSuccessfullyMaued(std::bind(&MauMauGameAccessorFromServer::listener_onOtherPlayerHasSuccessfullyMaued, this, std::placeholders::_1)),
			handler_initialPlayerIsOnTurn(std::bind(&MauMauGameAccessorFromServer::listener_initialPlayerIsOnTurn, this, std::placeholders::_1)) {
	
		packetTransmitter->addListenerForServerPkt(OtherPlayerHasDrawnCards_STCPacket::PACKET_ID, handler_onOtherPlayerHasDrawnCard);
		packetTransmitter->addListenerForServerPkt(OtherPlayerHasPlayedCard_STCPacket::PACKET_ID, handler_onOtherPlayerHasPlayedCard);
		packetTransmitter->addListenerForServerPkt(OtherPlayerHasPassed_STCPacket::PACKET_ID, handler_onOtherPlayerHasPassed);
		packetTransmitter->addListenerForServerPkt(LocalPlayerIsOnTurn_STCPacket::PACKET_ID, handler_onLocalPlayerIsOnTurn);
		packetTransmitter->addListenerForServerPkt(TurnWasAborted_STCPacket::PACKET_ID, handler_onTimeExpires);
		packetTransmitter->addListenerForServerPkt(MauPunishment_STCPacket::PACKET_ID, handler_onMauPunishment);
		packetTransmitter->addListenerForServerPkt(PlayerHasMauedSuccessfully_STCPacket::PACKET_ID, handler_onOtherPlayerHasSuccessfullyMaued);
		packetTransmitter->addListenerForServerPkt(InitialPlayerIsOnTurn_STCPacket::PACKET_ID, handler_initialPlayerIsOnTurn);

	}
	MauMauGameAccessorFromServer::~MauMauGameAccessorFromServer() {
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasDrawnCards_STCPacket::PACKET_ID, handler_onOtherPlayerHasDrawnCard);
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasPlayedCard_STCPacket::PACKET_ID, handler_onOtherPlayerHasPlayedCard);
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasPassed_STCPacket::PACKET_ID, handler_onOtherPlayerHasPassed);
		packetTransmitter->removeListenerForServerPkt(LocalPlayerIsOnTurn_STCPacket::PACKET_ID, handler_onLocalPlayerIsOnTurn);
		packetTransmitter->removeListenerForServerPkt(TurnWasAborted_STCPacket::PACKET_ID, handler_onTimeExpires);
		packetTransmitter->removeListenerForServerPkt(MauPunishment_STCPacket::PACKET_ID, handler_onMauPunishment);
		packetTransmitter->removeListenerForServerPkt(PlayerHasMauedSuccessfully_STCPacket::PACKET_ID, handler_onOtherPlayerHasSuccessfullyMaued);
		packetTransmitter->removeListenerForServerPkt(InitialPlayerIsOnTurn_STCPacket::PACKET_ID, handler_initialPlayerIsOnTurn);

	}
	void MauMauGameAccessorFromServer::playCardAndSetNextPlayerOnTurnLocal(std::string username, Card card, CardIndex newCardIndex, std::vector<Card> cardsToDraw, bool wasDrawedJustBefore) {
		auto player = gameData.lookupOpponent(username);

		int delayMs = 0;
		if(wasDrawedJustBefore) {
			gameData.drawInHandCardsFromCardStack(player, card);
			delayMs += DRAW_DURATION_MS + DELAY_BETWEEN_DRAW_AND_PLAY;
		}

		if(card != Card::NULLCARD) gameData.playCardFromHandCards(player, card, newCardIndex, delayMs);
		gameData.setNextPlayerOnTurnAndUpdateSkipState(card);

		auto newPlayerOnTurn = gameData.getPlayerOnTurn();
		gameData.setCardsToDrawOnPassDueToPlusTwo(cardsToDraw);
	}
	void MauMauGameAccessorFromServer::drawCardAndSetNextPlayerOnTurnLocal(std::string username) {
		auto player = gameData.lookupOpponent(username);
		gameData.drawInHandCardsFromCardStack(player, Card::NULLCARD);
		gameData.setNextPlayerOnTurnLocal();
	}

	void MauMauGameAccessorFromServer::listener_onOtherPlayerHasDrawnCard(Packet& p) {
		auto& casted = dynamic_cast<OtherPlayerHasDrawnCards_STCPacket&>(p);
		drawCardAndSetNextPlayerOnTurnLocal(casted.getUsername());
	}
	void MauMauGameAccessorFromServer::listener_onOtherPlayerHasPlayedCard(Packet& p) {
		auto& casted = dynamic_cast<OtherPlayerHasPlayedCard_STCPacket&>(p);
		playCardAndSetNextPlayerOnTurnLocal(casted.getUsername(), Card(casted.getCardNumber()), static_cast<CardIndex>(casted.getNewCardIndex()), Card::getVectorFromCardNumber(casted.getCardsToDraw()), casted.wasDrawnBeforePlayed());
	}
	void MauMauGameAccessorFromServer::listener_onOtherPlayerHasPassed(Packet& p) {
		auto& casted = dynamic_cast<OtherPlayerHasPassed_STCPacket&>(p);
		gameData.setNextPlayerOnTurnLocal();
		gameData.appendMessage(casted.getUsername() + " musste passen.");
	}
	void MauMauGameAccessorFromServer::listener_onLocalPlayerIsOnTurn(Packet& p) {
		auto& casted = dynamic_cast<LocalPlayerIsOnTurn_STCPacket&>(p);
		Card nextCardOnDrawStack(casted.getNextCardOnDrawStack());
		if(nextCardOnDrawStack == Card::NULLCARD) throw std::runtime_error("Can't set NULLCARD as draw card for next player.");

		gameData.setDrawCardForNextPlayer(nextCardOnDrawStack);
	}
	void MauMauGameAccessorFromServer::listener_onTimeExpires(Packet& p) {
		auto& casted = dynamic_cast<TurnWasAborted_STCPacket&>(p);
		std::vector<int> cardNumbersToDraw = casted.getCardsToDraw();
		std::vector<Card> cardsToDraw = Card::getVectorFromCardNumber(cardNumbersToDraw);

		gameData.abortTurnOnTimeExpires(cardsToDraw);
	}
	void MauMauGameAccessorFromServer::listener_onMauPunishment(Packet& p) {
		auto& casted = dynamic_cast<MauPunishment_STCPacket&>(p);
		std::vector<int> cardNumbersToDraw = casted.getCardsToDraw();
		std::vector<Card> cardsToDraw = Card::getVectorFromCardNumber(cardNumbersToDraw);

		auto player = gameData.lookupPlayer(casted.getConcernedUsername());
		gameData.onMauPunishment(player, cardsToDraw, casted.getCause());
	}
	void MauMauGameAccessorFromServer::listener_onOtherPlayerHasSuccessfullyMaued(Packet& p) {
		auto& casted = dynamic_cast<PlayerHasMauedSuccessfully_STCPacket&>(p);

		auto player = gameData.lookupPlayer(casted.getUsername());
		gameData.onSuccessfulMau(player);
	}
	void MauMauGameAccessorFromServer::listener_initialPlayerIsOnTurn(Packet& p) {
		auto& casted = dynamic_cast<InitialPlayerIsOnTurn_STCPacket&>(p);

		auto player = gameData.lookupPlayer(casted.getUsername());
		gameData.setInitialPlayerOnTurnLocal(player, Card(casted.getNextCardOnDrawStack()));
	}
}