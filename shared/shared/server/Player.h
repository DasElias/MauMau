#pragma once
#include "../packet/ParticipantOnServer.h"
#include "../model/HandCardStack.h"

namespace card {
	class Player {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static long long int const NOT_ON_TURN = -1;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			HandCardStack handCardStack;
			std::shared_ptr<ParticipantOnServer> const wrappedParticipant;
			long long unixTimeTurnStarted = NOT_ON_TURN;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Player(std::shared_ptr<ParticipantOnServer> wrappedParticipant, std::vector<Card> handCards = {});
			Player(const Player&) = delete;
			virtual ~Player() = default;

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static std::vector<std::shared_ptr<ParticipantOnServer>> getVectorWithWrappedParticipants(const std::vector<std::shared_ptr<Player>>& players);
			static std::vector<std::string> getVectorWithUsernames(const std::vector<std::shared_ptr<Player>>& players);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void addHandCard(Card c);
			void addHandCards(const std::vector<Card>& cards);
			void addHandCards(const std::vector<int>& cardNumbers);
			void removeHandCard(Card c);
			bool containsHandCard(Card c) const;
			const HandCardStack& getHandCards() const;

			std::string getUsername() const;
			long long getUnixTimeTurnStarted() const;
			bool isOnTurn() const;
			std::shared_ptr<ParticipantOnServer> getWrappedParticipant();

			virtual void onStartTurn();
			virtual void onEndTurn();

			bool operator==(const Player& p2) const;
			bool operator!=(const Player& p2) const;
	};
}