#include "ProxyPlayer.h"
#include <shared/utils/TimeUtils.h>
#include <stdexcept>
#include <shared/model/CardAnimationDuration.h>

namespace card {
	ProxyPlayer::ProxyPlayer(std::shared_ptr<ParticipantOnClient> wrappedParticipant, std::vector<Card> handCards) :
			handCardStack(std::make_unique<HandCardStack>(handCards)),
			wrappedParticipant(wrappedParticipant),
			unixTimeTurnStarted(NOT_ON_TURN) {
	}
	void ProxyPlayer::initHandCards(std::vector<Card> handCards, CardAnimator& drawCardStack, std::size_t playerIndex) {
		int delay = int(playerIndex * handCards.size() * INITIAL_DRAW_DELAY_BETWEEN_CARDS_MS);
		for(auto& card : handCards) {
			handCardStack.addLastCardFrom(card, drawCardStack, INITIAL_DRAW_DURATION_PER_CARD_MS, delay);
			delay += INITIAL_DRAW_DELAY_BETWEEN_CARDS_MS;
		}
	}
	void ProxyPlayer::drawCardLocal(Card mutatesTo, CardAnimator& source) {
		handCardStack.addLastCardFromImmediately(mutatesTo, source, DRAW_DURATION_MS);
	}
	void ProxyPlayer::drawCardsLocal(std::vector<Card> cards, CardAnimator& source) {
		if(cards.size() == 1) {
			// when there is only one card drawn, we have to call the virtual function since this one may be overridden
			drawCardLocal(cards[0], source);
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
