#include "ProxyPlayer.h"
#include <shared/utils/TimeUtils.h>
#include <stdexcept>
#include <shared/model/CardAnimationDuration.h>
#include <shared/utils/MathUtils.h>

namespace card {
	bool ProxyPlayer::wasSingleCardDrawedInHandCardsThisTurn = false;

	ProxyPlayer::ProxyPlayer(std::shared_ptr<ParticipantOnClient> wrappedParticipant) :
			handCardStack(std::make_unique<HandCardStack>()),
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
	void ProxyPlayer::drawSingleCardInHandCardsLocal(Card mutatesTo, CardAnimator& drawCardStack) {
		handCardStack.addLastCardFromImmediately(mutatesTo, drawCardStack, DRAW_DURATION_MS);
		this->wasSingleCardDrawedInHandCardsThisTurn = true;
	}
	void ProxyPlayer::drawMultipleCardsInHandCardsLocal(std::vector<Card> mutatesTo, CardAnimator& drawCardStack) {
		int delay = 0;
		for(Card& c : mutatesTo) {
			handCardStack.addLastCardFrom(c, drawCardStack, DRAW_DURATION_MS, delay);
			delay += DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS;
		}
	}
	void ProxyPlayer::drawMultipleCardsInHandCardsAfterCardPlayTimeLocal(std::vector<Card> mutatesTo, CardAnimator& drawCardStack) {
		int delay = PLAY_DURATION_MS + DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS;
		for(Card& c : mutatesTo) {
			handCardStack.addLastCardFrom(c, drawCardStack, DRAW_DURATION_MS, delay);
			delay += DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS;
		}
	}
	void ProxyPlayer::playCardFromHandCards(Card card, CardAnimator& playCardStack, bool isWaitingForColorPick) {
		playCardStack.addRandomCardFromImmediately(card, handCardStack, PLAY_DURATION_MS);
	}
	void ProxyPlayer::playCardFromHandCardsAfterDrawTime(Card card, CardAnimator& playCardStack, bool isWaitingForColorPick) {
		playCardStack.addRandomCardFrom(card, handCardStack, PLAY_DURATION_MS, DRAW_DURATION_MS + DELAY_BETWEEN_DRAW_AND_PLAY);
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
		if(wasSingleCardDrawedInHandCardsThisTurn) this->unixTimePlayerSkipped += DRAW_DURATION_MS + DELAY_BETWEEN_DRAW_AND_PLAY;
	}
	void ProxyPlayer::onStartTurn() {
		this->unixTimeTurnStarted = getMilliseconds();
	}
	void ProxyPlayer::onEndTurn() {
		this->unixTimeTurnStarted = NOT_ON_TURN;
		this->wasSingleCardDrawedInHandCardsThisTurn = false;
	}
	bool ProxyPlayer::operator==(const ProxyPlayer& p2) const {
		return p2.wrappedParticipant == this->wrappedParticipant;
	}
}
