#include "Player.h"
#include "../utils/TimeUtils.h"

namespace card {
	Player::Player(std::shared_ptr<ParticipantOnServer> wrappedParticipant, std::vector<Card> handCards) :
			handCardStack(handCards),
			wrappedParticipant(wrappedParticipant) {
	}
	std::vector<std::shared_ptr<ParticipantOnServer>> Player::getVectorWithWrappedParticipants(const std::vector<std::shared_ptr<Player>>& players) {
		std::vector<std::shared_ptr<ParticipantOnServer>> participants;

		for(auto& p : players) {
			participants.push_back(p->getWrappedParticipant());
		}

		return participants;
	}
	std::vector<std::string> Player::getVectorWithUsernames(const std::vector<std::shared_ptr<Player>>& players) {
		std::vector<std::string> usernames;

		for(auto& p : players) {
			usernames.push_back(p->getUsername());
		}
		return usernames;
	}
	void Player::addHandCard(Card c) {
		handCardStack.addFromPlain(c);
	}
	void Player::addHandCard(std::vector<Card> cards) {
		handCardStack.addFromPlain(cards);
	}
	void Player::removeHandCard(Card c) {
		handCardStack.remove(c);
	}
	bool Player::containsHandCard(Card c) const {
		return handCardStack.contains(c);
	}
	const HandCardStack& Player::getHandCards() const {
		return handCardStack;
	}
	std::string Player::getUsername() const {
		return wrappedParticipant->getUsername();
	}
	long long Player::getUnixTimeTurnStarted() const {
		return unixTimeTurnStarted;
	}
	bool Player::isOnTurn() const {
		return unixTimeTurnStarted != NOT_ON_TURN;
	}
	std::shared_ptr<ParticipantOnServer> Player::getWrappedParticipant() {
		return wrappedParticipant;
	}
	void Player::onStartTurn() {
		this->unixTimeTurnStarted = getMilliseconds();
	}
	void Player::onEndTurn() {
		this->unixTimeTurnStarted = NOT_ON_TURN;
	}
	bool Player::operator==(const Player& p2) const {
		return this->wrappedParticipant == p2.wrappedParticipant;
	}
}