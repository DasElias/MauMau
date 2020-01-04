#include "Card.h"
#include <stdexcept>

namespace card {
	const Card Card::NULLCARD(0);

	const Card Card::HEART_DEUCE(1);
	const Card Card::HEART_THREE(2);
	const Card Card::HEART_FOUR(3);
	const Card Card::HEART_FIVE(4);
	const Card Card::HEART_SIX(5);
	const Card Card::HEART_SÈVEN(6);
	const Card Card::HEART_EIGHT(7);
	const Card Card::HEART_NINE(8);
	const Card Card::HEART_TEN(9);
	const Card Card::HEART_JACK(10);
	const Card Card::HEART_QUEEN(11);
	const Card Card::HEART_KING(12);
	const Card Card::HEART_ACE(13);

	const Card Card::SPADE_DEUCE(14);
	const Card Card::SPADE_THREE(15);
	const Card Card::SPADE_FOUR(16);
	const Card Card::SPADE_FIVE(17);
	const Card Card::SPADE_SIX(18);
	const Card Card::SPADE_SÈVEN(19);
	const Card Card::SPADE_EIGHT(20);
	const Card Card::SPADE_NINE(21);
	const Card Card::SPADE_TEN(22);
	const Card Card::SPADE_JACK(23);
	const Card Card::SPADE_QUEEN(24);
	const Card Card::SPADE_KING(25);
	const Card Card::SPADE_ACE(26);

	const Card Card::DIAMOND_DEUCE(27);
	const Card Card::DIAMOND_THREE(28);
	const Card Card::DIAMOND_FOUR(29);
	const Card Card::DIAMOND_FIVE(30);
	const Card Card::DIAMOND_SIX(31);
	const Card Card::DIAMOND_SÈVEN(32);
	const Card Card::DIAMOND_EIGHT(33);
	const Card Card::DIAMOND_NINE(34);
	const Card Card::DIAMOND_TEN(35);
	const Card Card::DIAMOND_JACK(36);
	const Card Card::DIAMOND_QUEEN(37);
	const Card Card::DIAMOND_KING(38);
	const Card Card::DIAMOND_ACE(39);

	const Card Card::CLUB_DEUCE(40);
	const Card Card::CLUB_THREE(41);
	const Card Card::CLUB_FOUR(42);
	const Card Card::CLUB_FIVE(43);
	const Card Card::CLUB_SIX(44);
	const Card Card::CLUB_SÈVEN(45);
	const Card Card::CLUB_EIGHT(46);
	const Card Card::CLUB_NINE(47);
	const Card Card::CLUB_TEN(48);
	const Card Card::CLUB_JACK(49);
	const Card Card::CLUB_QUEEN(50);
	const Card Card::CLUB_KING(51);
	const Card Card::CLUB_ACE(52);


	Card::Card(int cardNumber) :
			cardNumber(cardNumber) {
	}

	std::vector<Card> Card::getVectorWithCards(Card card, std::size_t length) {
		std::vector<Card> c;
		for(std::size_t i = 0; i < length; i++) {
			c.push_back(card);
		}
		return c;
	}

	std::vector<Card> Card::getVectorFromCardNumber(const std::vector<int>& cardNumbers) {
		std::vector<Card> c;
		for(auto& singleCard : cardNumbers) {
			c.push_back(Card(singleCard));
		}
		return c;
	}

	int Card::getCardNumber() const {
		return cardNumber;
	}

	CardIndex Card::getCardIndex() const {
		int cardIndexNumber = int((this->cardNumber - 1) / CARDS_PER_CARD_INDEX);
		if(this->cardNumber != 0) cardIndexNumber++;

		return static_cast<CardIndex>(cardIndexNumber);
	}

	CardValue Card::getValue() const {
		int cardValue = (this->cardNumber - 1) % CARDS_PER_CARD_INDEX;
		return static_cast<CardValue>(cardValue);
	}

	bool Card::operator==(const Card& other) const {
		return cardNumber == other.cardNumber;
	}

	bool Card::operator<(const Card& other) const {
		return cardNumber < other.cardNumber;
	}

	bool Card::operator>(const Card& other) const {
		return cardNumber > other.cardNumber;
	}

	bool Card::operator<=(const Card& other) const {
		return cardNumber <= other.cardNumber;
	}

	bool Card::operator>=(const Card& other) const {
		return cardNumber >= other.cardNumber;
	}

}