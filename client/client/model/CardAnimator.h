#pragma once
#include "CardAnimation.h"
#include <shared/model/CardCollection.h>
#include <memory>
#include "CardAnimationCollection.h"
#include <map>
#include <optional>

namespace card {
	class CardAnimator {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static long long idCounter;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			long long id;
			int cardsToRemoveAfterDelayCounter = 0;
			int numberOfIncomingAnimations = 0;
			CardAnimationCollection animations;
			std::unique_ptr<CardCollection> wrappedCardCollection;
			std::optional<Card> lastRegisteredAnimation;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CardAnimator(std::unique_ptr<CardCollection> wrappedCardCollection);
			~CardAnimator();
			CardAnimator(const CardAnimator&) = delete;
			CardAnimator& operator=(const CardAnimator&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			//
			// METHODS FROM AnimatedCardCollection
			//
			void addFirstCardFrom(Card mutatesTo, CardAnimator& source, int durationMs, int delayMs);
			void addLastCardFrom(Card mutatesTo, CardAnimator& source, int durationMs, int delayMs);
			void addDeterminedCardFrom(std::size_t indexOfCardToAddInSource, CardAnimator& source, int durationMs, int delayMs);
			void addRandomCardFrom(Card mutatesTo, CardAnimator& source, int durationMs, int delayMs);

			void addFirstCardFromImmediately(Card mutatesTo, CardAnimator& source, int durationMs);
			void addLastCardFromImmediately(Card mutatesTo, CardAnimator& source, int durationMs);
			void addDeterminedCardFromImmediately(std::size_t indexOfCardToAddInSource, CardAnimator& source, int durationMs);
			void addRandomCardFromImmediately(Card mutatesTo, CardAnimator& source, int durationMs);

			std::vector<CardAnimation> getCardAnimations() const;

			std::size_t getSizeOfPendingTransactions() const;
			std::size_t getSizeInclPendingTransactions() const;
			bool isEmptyAndNoPendingTransactions() const;
			bool equalsId(const CardAnimator& other);

			CardCollection& getWrappedCardCollection();
			CardCollection& operator*();

			Card getLastInclAnimations() const;
			void clearInclAnimations();

			int getAvailableCardsSize() const;
			int getNumberOfIncomingAnimations() const;

			//
			// METHODS FROM CardCollection
			//
			void addFromPlain(Card c, std::size_t amount = 1);
			void addFromPlain(std::vector<Card> cards);
			void addFromPlain(std::initializer_list<Card> cards);
			void addFromPlainAtPosition(std::size_t position, Card c, std::size_t amount = 1);

			Card get(std::size_t index) const;
			Card getLast() const;
			CardCollection::iterator begin();
			CardCollection::const_iterator begin() const;
			CardCollection::const_iterator cbegin() const;
			CardCollection::iterator end();
			CardCollection::const_iterator end() const;
			CardCollection::const_iterator cend() const;
			std::vector<int> getCardNumbers() const;
			const std::vector<Card>& getCards() const;
			bool contains(Card c) const;

			std::size_t find(Card c) const;
			std::size_t getSize() const;
			bool isEmpty() const;

			Card removeLast();
			Card remove(std::size_t index);
			Card remove(Card card);
			void clear();

		private:
			void onAnimationStart();
			void onAnimationEnd();
			void registerCardAnimation(Card c, CardAnimator& source);
			void unregisterCardAnimation(CardAnimator& source);
	};
}