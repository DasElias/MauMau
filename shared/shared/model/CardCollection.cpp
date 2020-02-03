#include "CardCollection.h"
#include <stdexcept>
#include <string>
#include "../utils/ThreadUtils.h"
#include "../utils/MathUtils.h"
namespace card {
	CardCollection::CardCollection(std::vector<Card> cards) :
			cards(cards) {

	}

	void CardCollection::addFromPlain(Card c, std::size_t amount) {
		for(int i = 0; i < amount; i++) {
			this->cards.push_back(c);
		}
		reorder();
	}

	void CardCollection::addFromPlain(std::vector<Card> cardVector) {
		for(auto& c : cardVector) {
			this->cards.push_back(c);
		}
		reorder();
	}

	void CardCollection::addFromPlain(std::initializer_list<Card> cardInitializerLIst) {
		for(auto& c : cardInitializerLIst) {
			this->cards.push_back(c);
		}
		reorder();
	}

	void CardCollection::addFromPlain(const std::vector<int>& cardNumbers) {
		for(auto& cn : cardNumbers) {
			this->cards.push_back(Card(cn));
		}
		reorder();
	}

	void CardCollection::addFromPlainAtPosition(std::size_t position, Card c, std::size_t amount) {
		for(int i = 0; i < amount; i++) {
			this->cards.insert(cards.begin() + position, c);
		}
		reorder();
	}

	Card CardCollection::get(std::size_t index) const {
		return cards.at(index);
	}

	Card CardCollection::getLast() const {
		return get(getSize() - 1);
	}

	CardCollection::iterator CardCollection::begin() {
		return cards.begin();
	}

	CardCollection::const_iterator CardCollection::begin() const {
		return cbegin();
	}

	CardCollection::const_iterator CardCollection::cbegin() const {
		return cards.cbegin();
	}

	CardCollection::iterator CardCollection::end() {
		return cards.end();
	}

	CardCollection::const_iterator CardCollection::end() const {
		return cend();
	}

	CardCollection::const_iterator CardCollection::cend() const {
		return cards.cend();
	}

	std::vector<int> CardCollection::getCardNumbers() const {
		std::vector<int> cardNumbers;
		for(auto& c : cards) {
			cardNumbers.push_back(c.getCardNumber());
		}
		return cardNumbers;
	}

	bool CardCollection::contains(Card c) const {
		return std::find(cards.begin(), cards.end(), c) != cards.end();
	}

	std::size_t CardCollection::getSize() const {
		return cards.size();
	}

	bool CardCollection::isEmpty() const {
		return getSize() == 0;
	}

	Card CardCollection::removeLast() {
		return remove(getSize() - 1);
	}

	Card CardCollection::removeFirst() {
		return remove(0);
	}

	Card CardCollection::remove(std::size_t index) {
		Card c = cards.at(index);
		cards.erase(cards.begin() + index);
		return c;
	}

	Card CardCollection::remove(Card card) {
		auto ptr = std::find(cards.begin(), cards.end(), card);
		if(ptr == cards.end()) throw std::runtime_error("Card " + std::to_string(card.getCardNumber()) + " is not part of this CardCollection.");
		cards.erase(ptr);

		return card;
	}

	void CardCollection::clear() {
		cards.clear();
	}

	void CardCollection::reorder() {
		// empty method body
	}

}