#pragma once
#include <shared/packet/CTSPacketTransmitter.h>
#include <shared/model/CardIndex.h>
#include "PlayVerifier.h"
#include <shared/model/Direction.h>
#include <memory>
#include <shared/model/CardStack.h>
#include "LocalPlayer.h"
#include <shared\model\RoomOptions.h>

namespace card {
	class ProxyMauMauGame : public PlayVerifier {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<CTSPacketTransmitter> packetTransmitter;
			CardAnimator drawCardStack;
			CardAnimator playCardStack;
			CardIndex indexForNextCard;
			Direction direction = Direction::CW;
			Card drawCardForNextPlayer;
			RoomOptions& roomOptions;

			std::shared_ptr<ProxyPlayer> userOnTurn;
			std::vector<std::shared_ptr<ProxyPlayer>> allPlayers;
			std::vector<std::shared_ptr<ProxyPlayer>> opponents;
			std::shared_ptr<LocalPlayer> localPlayer;
			std::vector<std::shared_ptr<ProxyPlayer>> skippedPlayers;

			ServerPacketListenerCallback handler_onOtherPlayerHasDrawnCard;
			ServerPacketListenerCallback handler_onOtherPlayerHasPlayedCard;
			ServerPacketListenerCallback handler_onLocalPlayerIsOnTurn;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ProxyMauMauGame(std::shared_ptr<CTSPacketTransmitter> packetTransmitter, std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, 
				std::shared_ptr<ParticipantOnClient> localParticipant, std::string usernameOnTurn,
				std::vector<int> handCards, int startCard, int nextCardOnDrawStack, RoomOptions& roomOptions);
			ProxyMauMauGame(const ProxyMauMauGame&) = delete;
			~ProxyMauMauGame();
			ProxyMauMauGame& operator=(const ProxyMauMauGame&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool isLocalPlayerOnTurn() const;
			bool isPreviousTurnCompleted() const;
			bool canPlay(std::size_t indexInLocalPlayerHandCards) const;
			bool canPlay(Card card) const;
			bool canDraw() const;
			bool canChangeColor(Card playedCard) const override;
			bool canMau() const;
			bool canPlayDrawnCard() const;
			bool canTakeDrawnCardIntoHandCards() const;
			bool canSkipPlayer(Card card) const;
			bool isWaitingForColorChoose() const;			
			std::size_t getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const;


			void drawCard();
			void takeDrawnCardIntoHandCards();
			void playDrawnCard();
			void playCard(Card card);
			void playCard(std::size_t index);
			void chooseColor(CardIndex color);

			const CardAnimator& getDrawStack() const;
			const CardAnimator& getPlayStack() const;
			std::shared_ptr<LocalPlayer> getLocalPlayer();
			std::vector<std::shared_ptr<ProxyPlayer>> getOpponents();
			void removeOpponentLocal(std::shared_ptr<ParticipantOnClient> player);
			bool checkIfIsOpponent(std::string username) const;
			std::shared_ptr<ProxyPlayer> lookupPlayer(std::string username);
			std::shared_ptr<ProxyPlayer> lookupOpponent(std::string username);

			void playCardAndSetNextPlayerOnTurnLocal(std::string username, Card card, CardIndex newCardIndex, std::vector<Card> cardsToDraw, bool wasDrawedJustBefore);
			void drawCardAndSetNextPlayerOnTurnLocal(std::string username);
			void setNextOrNextButOneOnTurnLocal(Card playedCard);
			void setNextPlayerOnTurnLocal();
			void setNextButOnePlayerOnTurnLocal();
			void setOnTurnLocal(std::shared_ptr<ProxyPlayer> player);
			void localPlayerGetCardsLocal(std::vector<Card> cards);
			void proxyPlayerGetCardLocal(std::string username, std::size_t amountOfCards);

		private:
			void initStartCards(std::size_t amountOfStartCardsPerPlayer);
			void tryRebalanceCardStacks();
			void setLocalPlayerAtTheBeginOfPlayersVector();

			void sendPlayCardPacket(CardIndex newCardIndex = CardIndex::NULLINDEX);

			void listener_onOtherPlayerHasDrawnCard(Packet& p);
			void listener_onOtherPlayerHasPlayedCard(Packet& p);
			void listener_onLocalPlayerIsOnTurn(Packet& p);
	};
}