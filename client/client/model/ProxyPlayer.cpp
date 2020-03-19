#include "ProxyPlayer.h"
#include <shared/utils/TimeUtils.h>
#include <stdexcept>
#include <shared/model/CardAnimationDuration.h>
#include <shared/utils/MathUtils.h>
#include <shared/model/MaxTurnDuration.h>

namespace card {
	int const ProxyPlayer::SKIP_ANIMATION_DURATION_MS = 2 * PLAY_DURATION_MS;
	int const ProxyPlayer::MAU_ANIMATION_DURATION_MS = 2 * PLAY_DURATION_MS;

	ProxyPlayer::ProxyPlayer(std::shared_ptr<ParticipantOnClient> wrappedParticipant) :
			handCardStack(std::make_unique<HandCardStack>()),
			wrappedParticipant(wrappedParticipant),
			unixTimeOnTurnAnimationStarted(NOT_ON_TURN),
			unixTimeOnTurnAnimationFreezed(ANIMATION_NOT_FREEZED),
			unixTimePlayerSkipped(0),
			unixTimePlayerMaued(0),
			isInSkipState_field(false) {
	}
	void ProxyPlayer::initHandCards(std::vector<Card> handCards, CardAnimator& drawCardStack, std::size_t playerIndex) {
		int delay = int(playerIndex * handCards.size() * INITIAL_DRAW_DELAY_BETWEEN_CARDS_MS);
		for(auto& card : handCards) {
			handCardStack.addLastCardFrom(card, drawCardStack, INITIAL_DRAW_DURATION_PER_CARD_MS, delay);
			delay += INITIAL_DRAW_DELAY_BETWEEN_CARDS_MS;
		}
	}
	void ProxyPlayer::drawCardInHandCardsDueToUserAction(Card mutatesTo, CardAnimator& drawCardStack, int delayMs) {
		handCardStack.addLastCardFrom(mutatesTo, drawCardStack, DRAW_DURATION_MS, delayMs);
	}
	void ProxyPlayer::drawCardInHandCardsAsPunishment(Card mutatesTo, CardAnimator& drawCardStack, int delayMs) {
		handCardStack.addLastCardFrom(mutatesTo, drawCardStack, DRAW_DURATION_MS, delayMs);
	}
	void ProxyPlayer::playCardFromHandCardsAfterDelay(Card card, CardAnimator& playCardStack, int delayMs) {
		playCardStack.addRandomCardFrom(card, handCardStack, PLAY_DURATION_MS, delayMs);
	}
	const CardAnimator& ProxyPlayer::getCardStack() const {
		return handCardStack;
	}
	std::string ProxyPlayer::getUsername() const {
		return wrappedParticipant->getUsername();
	}
	Avatar ProxyPlayer::getAvatar() const {
		return wrappedParticipant->getAvatar();
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
	bool ProxyPlayer::isMauAnimationActive() const {
		float x = getMilliseconds() - unixTimePlayerMaued;
		return x >= 0 && x <= MAU_ANIMATION_DURATION_MS;
	}
	float ProxyPlayer::getPercentOfMauAnimation() const {
		float x = getMilliseconds() - unixTimePlayerMaued;
		return interpolateLinear(x, 0, 0, MAU_ANIMATION_DURATION_MS, 1);
	}
	std::optional<float> ProxyPlayer::getPercentOfMauAnimationOrNone() const {
		if(isMauAnimationActive()) return getPercentOfMauAnimation();
		return std::nullopt;
	}
	bool ProxyPlayer::hasTimeExpired() const {
		if(unixTimeOnTurnAnimationStarted == NOT_ON_TURN) return true;
		else return getMilliseconds() - unixTimeOnTurnAnimationStarted > MAX_TURN_DURATION;
	}
	bool ProxyPlayer::isRemainingTimeAnimationActive() const {
		return getPercentOfRemainingTime().has_value();
	}
	std::optional<float> ProxyPlayer::getPercentOfRemainingTime() const {
		if(unixTimeOnTurnAnimationStarted == NOT_ON_TURN) return std::nullopt;

		long long int currentTime = (unixTimeOnTurnAnimationFreezed == ANIMATION_NOT_FREEZED) ? getMilliseconds() : unixTimeOnTurnAnimationFreezed;
		float x = float(currentTime - unixTimeOnTurnAnimationStarted);
		float x1 = 0;
		float x2 = MAX_TURN_DURATION;

		return interpolateLinear(x, x1, 0, x2, 1);
	}
	std::shared_ptr<ParticipantOnClient> ProxyPlayer::getWrappedParticipiant() {
		return wrappedParticipant;
	}
	bool ProxyPlayer::isInSkipState() const {
		return isInSkipState_field;
	}
	void ProxyPlayer::endRemainingTimeAnimation() {
		this->unixTimeOnTurnAnimationStarted = NOT_ON_TURN;
		this->unixTimeOnTurnAnimationFreezed = ANIMATION_NOT_FREEZED;
	}
	void ProxyPlayer::freezeRemainingTimeAnimation() {
		this->unixTimeOnTurnAnimationFreezed = getMilliseconds();
	}
	void ProxyPlayer::setSkipState() {
		isInSkipState_field = true;
	}
	void ProxyPlayer::startSkippedAnimation() {
		this->unixTimePlayerSkipped = getMilliseconds();
	}
	void ProxyPlayer::onSuccessfulMau() {
		this->unixTimePlayerMaued = getMilliseconds();
	}
	void ProxyPlayer::onStartTurn() {
		this->unixTimeOnTurnAnimationStarted = getMilliseconds();
		this->unixTimeOnTurnAnimationFreezed = ANIMATION_NOT_FREEZED;
	}
	void ProxyPlayer::onEndTurn() {
		isInSkipState_field = false;
	}
	bool ProxyPlayer::operator==(const ProxyPlayer& p2) const {
		return p2.wrappedParticipant == this->wrappedParticipant;
	}
}
