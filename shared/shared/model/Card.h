#pragma once
#include "CardIndex.h"
#include "CardValue.h"
#include <vector>

namespace card {
	class Card {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const CARDS_PER_CARD_INDEX = 13;
			static int const MAX_CARDS = CARDS_PER_CARD_INDEX * 4;

			static Card const	NULLCARD,

								HEART_DEUCE, 
								HEART_THREE, 
								HEART_FOUR, 
								HEART_FIVE, 
								HEART_SIX, 
								HEART_SEVEN, 
								HEART_EIGHT, 
								HEART_NINE,
								HEART_TEN,
								HEART_JACK,
								HEART_QUEEN,
								HEART_KING,
								HEART_ACE,
								
								SPADE_DEUCE,
								SPADE_THREE,
								SPADE_FOUR,
								SPADE_FIVE,
								SPADE_SIX,
								SPADE_SEVEN,
								SPADE_EIGHT,
								SPADE_NINE,
								SPADE_TEN,
								SPADE_JACK,
								SPADE_QUEEN,
								SPADE_KING,
								SPADE_ACE, 
								
								DIAMOND_DEUCE,
								DIAMOND_THREE,
								DIAMOND_FOUR,
								DIAMOND_FIVE,
								DIAMOND_SIX,
								DIAMOND_SEVEN,
								DIAMOND_EIGHT,
								DIAMOND_NINE,
								DIAMOND_TEN,
								DIAMOND_JACK,
								DIAMOND_QUEEN,
								DIAMOND_KING,
								DIAMOND_ACE, 
				
								CLUB_DEUCE,
								CLUB_THREE,
								CLUB_FOUR,
								CLUB_FIVE,
								CLUB_SIX,
								CLUB_SEVEN,
								CLUB_EIGHT,
								CLUB_NINE,
								CLUB_TEN,
								CLUB_JACK,
								CLUB_QUEEN,
								CLUB_KING,
								CLUB_ACE;


		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int cardNumber;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Card(int cardNumber = 0);

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static std::vector<Card> getVectorWithCards(Card card, std::size_t length);
			static std::vector<Card> getVectorFromCardNumber(const std::vector<int>& cardNumbers);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			int getCardNumber() const;
			CardIndex getCardIndex() const;
			CardValue getValue() const;

			bool operator==(const Card&) const;
			bool operator<(const Card&) const;
			bool operator>(const Card&) const;
			bool operator<=(const Card&) const;
			bool operator>=(const Card&) const;

	};
}