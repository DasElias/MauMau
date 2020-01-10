#include "ProxyPlayer.h"
#include <shared/utils/TimeUtils.h>
#include <stdexcept>
#include <shared/model/CardAnimationDuration.h>
#include <shared/utils/MathUtils.h>

namespace card {
	ProxyPlayer::ProxyPlayer(std::shared_ptr<ParticipantOnClient> wrappedParticipant, std::vector<Card> handCards) :
			handCardStack(std::make_unique<HandCardStack>(handCards)),
			wrappedParticipant(wrappedParticipant),
			unixTimeTurnStarted(NOT_ON_TURN),
			unixTimePlayerSkipped(0) {
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
	void ProxyPlayer::drawCardsAfterTimeCardWasPlayed(std::vector<Card> cards, CardAnimator& drawCardStack) {
		int delay = PLAY_DURATION_MS;
		for(Card& c : cards) {
			handCardStack.addLastCardFrom(c, drawCardStack, DRAW_DURATION_MS, delay);
			delay += DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS;
		}
	}
	void ProxyPlayer::playCardLocal(Card card, CardAnimator& playCardStack, bool isWaitingForColorPick) {
		playCardStack.addRandomCardFromImmediately(card, handCardStack, PLAY_DURATION_MS);
	}
	void ProxyPlayer::playCardAfterTimeCardWasDrawn(Card card, CardAnimator& playCardStack, bool isWaitingForColorPick) {
		playCardStack.addRandomCardFrom(card, handCardStack, PLAY_DURATION_MS, DRAW_DURATION_MS);
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
	bool ProxyPlayer::isSkipAnimationActive() const {
		float x = getMilliseconds() - unixTimePlayerSkipped;
		return x >= 0 && x <= SKIP_ANIMATION_DURATION_MS;
	}
	float ProxyPlayer::getPercentOfSkipAnimation() const {
		float const x = getMilliseconds() - unixTimePlayerSkipped;
		return interpolateLinear(x, 0, 0, SKIP_ANIMATION_DURATION_MS, 1);
	}
	std::optional<float> ProxyPlayer::getPercentOfSkipAnimationOrNone() const {
		if(isSkipAnimationActive()) return getPercentOfSkipAnimation();
		return std::nullopt;
	}
	bool ProxyPlayer::isOnTurn() const {
		return unixTimeTurnStarted != NOT_ON_TURN;
	}
	std::shared_ptr<ParticipantOnClient> ProxyPlayer::getWrappedParticipiant() {
		return wrappedParticipant;
	}
	void ProxyPlayer::onSkip() {
		this->unixTimePlayerSkipped = getMilliseconds();
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
