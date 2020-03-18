#include "CardAnimator.h"
#include <shared/utils/ThreadUtils.h>
#include <shared/utils/MathUtils.h>
#include <stdexcept>
#include <optional>

namespace card {
	long long CardAnimator::idCounter = 0;
	int CardAnimator::pendingAnimationsCounter = 0;

	CardAnimator::CardAnimator(std::unique_ptr<CardCollection> wrappedCardCollection) :
			id(idCounter++),
			wrappedCardCollection(std::move(wrappedCardCollection)),
			lastRegisteredAnimation() {
	}
	CardAnimator::~CardAnimator() {
		threadUtils_removeCallbacksWithKey(this);
	}
	bool CardAnimator::arePendingAnimations() {
		return pendingAnimationsCounter > 0;
	}

	void card::CardAnimator::addFirstCardFrom(Card mutatesTo, CardAnimator& source, int durationMs, int delayMs) {
		if(delayMs == 0) {
			this->addFirstCardFromImmediately(mutatesTo, source, durationMs);
			return;
		}

		registerCardAnimation(mutatesTo);
		threadUtils_invokeIn(delayMs, this, [this, mutatesTo, &source, durationMs]() {
			unregisterCardAnimation();
			this->addFirstCardFromImmediately(mutatesTo, source, durationMs);
		});
	}

	void card::CardAnimator::addFirstCardFromImmediately(Card mutatesTo, CardAnimator& source, int durationMs) {
		std::size_t const indexToRemove = 0;
		onAnimationStart();
		source.remove(indexToRemove);

		CardAnimation a(getMilliseconds(), durationMs, source, mutatesTo, indexToRemove);
		animations.insertAnimation(a);

		threadUtils_invokeIn(durationMs, this, [this, a, mutatesTo] {
			animations.removeAnimation(a);
			this->addFromPlain(mutatesTo);
			onAnimationEnd();
		});
	}

	void CardAnimator::addLastCardFrom(Card mutatesTo, CardAnimator& source, int durationMs, int delayMs) {
		if(delayMs == 0) {
			this->addLastCardFromImmediately(mutatesTo, source, durationMs);
			return;
		}

		registerCardAnimation(mutatesTo);
		threadUtils_invokeIn(delayMs, this, [this, mutatesTo, &source, durationMs]() {
			unregisterCardAnimation();
			this->addLastCardFromImmediately(mutatesTo, source, durationMs);
		});
	}

	void CardAnimator::addLastCardFromImmediately(Card mutatesTo, CardAnimator& source, int durationMs) {
		std::size_t const indexToRemove = source.getSize() - 1;
		onAnimationStart();
		source.removeLast();

		CardAnimation a(getMilliseconds(), durationMs, source, mutatesTo, indexToRemove);
		animations.insertAnimation(a);

		threadUtils_invokeIn(durationMs, this, [this, a, mutatesTo] {
			animations.removeAnimation(a);
			this->addFromPlain(mutatesTo);
			onAnimationEnd();
		});
	}

	void CardAnimator::addDeterminedCardFrom(std::size_t indexOfCardToAddInSource, CardAnimator& source, int durationMs, int delayMs) {
		if(delayMs == 0) {
			this->addDeterminedCardFromImmediately(indexOfCardToAddInSource, source, durationMs);
			return;
		}

		Card card = source.get(indexOfCardToAddInSource);
		registerCardAnimation(card);
		threadUtils_invokeIn(delayMs, this, [this, indexOfCardToAddInSource, &source, durationMs]() {
			unregisterCardAnimation();
			this->addDeterminedCardFromImmediately(indexOfCardToAddInSource, source, durationMs);
		});
	}

	void CardAnimator::addDeterminedCardFromImmediately(std::size_t indexOfCardToAddInSource, CardAnimator& source, int durationMs) {
		onAnimationStart();
		Card card = source.get(indexOfCardToAddInSource);
		source.remove(indexOfCardToAddInSource);

		CardAnimation a(getMilliseconds(), durationMs, source, card, indexOfCardToAddInSource);
		animations.insertAnimation(a);

		threadUtils_invokeIn(durationMs, this, [this, a, card] {
			onAnimationEnd();
			animations.removeAnimation(a);
			this->addFromPlain(card);
		});
	}

	void CardAnimator::addRandomCardFrom(Card mutatesTo, CardAnimator& source, int durationMs, int delayMs) {
		if(delayMs == 0) {
			addRandomCardFromImmediately(mutatesTo, source, durationMs);
			return;
		}
	
		registerCardAnimation(mutatesTo);
		threadUtils_invokeIn(delayMs, this, [this, mutatesTo, &source, durationMs]() {
			unregisterCardAnimation();
			addRandomCardFromImmediately(mutatesTo, source, durationMs);
		});
	}

	void CardAnimator::addRandomCardFromImmediately(Card mutatesTo, CardAnimator& source, int durationMs) {
		if(source.isEmpty()) throw std::runtime_error("Can't add card from an empty card stack.");
		std::size_t const indexToRemove = randomInRange<std::size_t>(0, source.getSize() - 1);
		onAnimationStart();
		source.remove(indexToRemove);

		CardAnimation a(getMilliseconds(), durationMs, source, mutatesTo, indexToRemove);
		animations.insertAnimation(a);

		threadUtils_invokeIn(durationMs, this, [this, a, mutatesTo] {	
			animations.removeAnimation(a);
			this->addFromPlain(mutatesTo);
			onAnimationEnd();
		});
	}

	void card::CardAnimator::onAnimationStart() {
		pendingAnimationsCounter++;
	}

	void card::CardAnimator::onAnimationEnd() {
		pendingAnimationsCounter--;
	}

	void CardAnimator::registerCardAnimation(Card c) {
		lastRegisteredAnimation = c;
	}

	void CardAnimator::unregisterCardAnimation() {
		lastRegisteredAnimation = std::nullopt;
	}

	std::vector<CardAnimation> CardAnimator::getCardAnimations() const {
		return animations;
	}

	std::size_t CardAnimator::getSizeOfPendingTransactions() const {
		return animations.size() + ((lastRegisteredAnimation.has_value()) ? 1 : 0);
	}

	std::size_t CardAnimator::getSizeInclPendingTransactions() const {
		return getSize() + getSizeOfPendingTransactions();
	}

	bool CardAnimator::isEmptyAndNoPendingTransactions() const {
		return isEmpty() && animations.empty();
	}

	bool CardAnimator::equalsId(const CardAnimator& other) {
		return this->id == other.id;
	}

	CardCollection& CardAnimator::getWrappedCardCollection() {
		return *wrappedCardCollection;
	}

	CardCollection& CardAnimator::operator*() {
		return getWrappedCardCollection();
	}

	Card CardAnimator::getLastInclAnimations() const {
		if(lastRegisteredAnimation.has_value()) return *lastRegisteredAnimation;
		if(! animations.empty()) return animations.getLast().mutatesTo;
		return getLast();

	}

	void CardAnimator::clearInclAnimations() {
		clear();
		animations.clear();
		lastRegisteredAnimation = std::nullopt;

		// callbacks for animations shouldn't be executed
		threadUtils_removeCallbacksWithKey(this);
	}





	void CardAnimator::addFromPlain(Card c, std::size_t amount) {
		wrappedCardCollection->addFromPlain(c, amount);
	}
	void CardAnimator::addFromPlain(std::vector<Card> cards) {
		wrappedCardCollection->addFromPlain(cards);
	}
	void CardAnimator::addFromPlain(std::initializer_list<Card> cards) {
		wrappedCardCollection->addFromPlain(cards);
	}
	void CardAnimator::addFromPlainAtPosition(std::size_t position, Card c, std::size_t amount) {
		wrappedCardCollection->addFromPlainAtPosition(position, c, amount);
	}
	Card CardAnimator::get(std::size_t index) const {
		return wrappedCardCollection->get(index);
	}
	Card CardAnimator::getLast() const {
		return wrappedCardCollection->getLast();
	}
	CardCollection::iterator CardAnimator::begin() {
		return wrappedCardCollection->begin();
	}
	CardCollection::const_iterator CardAnimator::begin() const {
		return wrappedCardCollection->begin();
	}
	CardCollection::const_iterator CardAnimator::cbegin() const {
		return wrappedCardCollection->cbegin();
	}
	CardCollection::iterator CardAnimator::end() {
		return wrappedCardCollection->end();
	}
	CardCollection::const_iterator CardAnimator::end() const {
		return wrappedCardCollection->end();
	}
	CardCollection::const_iterator CardAnimator::cend() const {
		return wrappedCardCollection->cend();
	}
	std::vector<int> CardAnimator::getCardNumbers() const {
		return wrappedCardCollection->getCardNumbers();
	}
	bool CardAnimator::contains(Card c) const {
		return wrappedCardCollection->contains(c);
	}
	std::size_t CardAnimator::find(Card c) const {
		return wrappedCardCollection->find(c);
	}
	std::size_t CardAnimator::getSize() const {
		return wrappedCardCollection->getSize();
	}
	bool CardAnimator::isEmpty() const {
		return wrappedCardCollection->isEmpty();
	}
	Card CardAnimator::removeLast() {
		return wrappedCardCollection->removeLast();
	}
	Card CardAnimator::remove(std::size_t index) {
		return wrappedCardCollection->remove(index);
	}
	Card CardAnimator::remove(Card card) {
		return wrappedCardCollection->remove(card);
	}
	void CardAnimator::clear() {
		wrappedCardCollection->clear();
	}
}