#include "ProxyPlayer.h"
#include <shared/utils/TimeUtils.h>
#include <stdexcept>

namespace card {
	ProxyPlayer::ProxyPlayer(std::shared_ptr<ParticipantOnClient> wrappedParticipant, std::vector<Card> handCards) :
			handCardStack(std::make_unique<HandCardStack>(handCards)),
			wrappedParticipant(wrappedParticipant),
			unixTimeTurnStarted(NOT_ON_TURN) {
	}
	void ProxyPlayer::addHandCardFromPlainLocal(Card c) {
		handCardStack.addFromPlain(c);
	}
	void ProxyPlayer::addHandCardsFromPlainLocal(std::vector<Card> cards) {
		for(auto& c : cards) {
			addHandCardFromPlainLocal(c);
		}
	}
	void ProxyPlayer::pickLastCardFromCardStackLocal(Card mutatesTo, CardAnimator& source) {
		handCardStack.addLastCardFromImmediately(mutatesTo, source, DRAW_DURATION_MS);
	}
	void ProxyPlayer::pickLastCardsFromCardStackLocal(std::vector<Card> cards, CardAnimator& source) {
		if(cards.size() == 1) {
			// when there is only one card drawn, we have to call the virtual function since this one may be overridden
			pickLastCardFromCardStackLocal(cards[0], source);
			return;
		} 

		int delay = 0;
		for(Card& c : cards) {
			handCardStack.addLastCardFrom(c, source, DRAW_DURATION_MS, delay);
			delay += DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS;
		}
	}
	void ProxyPlayer::playCardLocal(Card card, CardAnimator& destination, bool isWaitingForColorPick) {
		destination.addRandomCardFromImmediately(card, handCardStack, PLAY_DURATION_MS);
	}
	void ProxyPlayer::removeHandCardLocal(Card c) {
		handCardStack.remove(c);
	}
	const CardAnimator& ProxyPlayer::getCardStack() const {
		return handCardStack;
	}
	std::string ProxyPlayer::getUsername() const {
		return wrappedParticipant->getUsername();
	}
	long long int ProxyPlayer::getUnixTimeTurnStarted() const {
		return unixTimeTurnStarted;
	}
	bool ProxyPlayer::isOnTurn() const {
		return unixTimeTurnStarted != NOT_ON_TURN;
	}
	std::shared_ptr<ParticipantOnClient> ProxyPlayer::getWrappedParticipiant() {
		return wrappedParticipant;
	}
	void ProxyPlayer::onStartTurn() {
		this->unixTimeTurnStarted = getMilliseconds();
	}
	void ProxyPlayer::onEndTurn() {
		this->unixTimeTurnStarted = NOT_ON_TURN;
	}
	bool ProxyPlayer::operator==(const ProxyPlayer& p2) const {
		return p2.wrappedParticipant == this->wrappedParticipant;
	}
}
