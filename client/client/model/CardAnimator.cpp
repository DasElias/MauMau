#include "CardAnimator.h"
#include <shared/utils/ThreadUtils.h>
#include <shared/utils/MathUtils.h>
#include <stdexcept>

namespace card {
	long long CardAnimator::idCounter = 0;
	int CardAnimator::pendingAnimationsCounter = 0;

	CardAnimator::CardAnimator(std::unique_ptr<CardCollection> wrappedCardCollection) :
			id(idCounter++),
			wrappedCardCollection(std::move(wrappedCardCollection)) {
	}
	bool CardAnimator::arePendingAnimations() {
		return pendingAnimationsCounter > 0;
	}

	void card::CardAnimator::addFirstCardFrom(Card mutatesTo, CardAnimator& source, int durationMs, int delayMs) {
		if(delayMs == 0) {
			this->addFirstCardFromImmediately(mutatesTo, source, durationMs);
			return;
		}

		threadUtils_invokeIn(delayMs, [this, mutatesTo, &source, durationMs]() {
			this->addFirstCardFromImmediately(mutatesTo, source, durationMs);
		});
	}

	void card::CardAnimator::addFirstCardFromImmediately(Card mutatesTo, CardAnimator& source, int durationMs) {
		onAnimationStart();
		source.remove(0);

		CardAnimation a(getMilliseconds(), durationMs, source, mutatesTo);
		addCardAnimation(a);

		threadUtils_invokeIn(durationMs, [this, a, mutatesTo] {
			removeCardAnimation(a);
			this->addFromPlain(mutatesTo);
			onAnimationEnd();
		});
	}

	void CardAnimator::addLastCardFrom(Card mutatesTo, CardAnimator& source, int durationMs, int delayMs) {
		if(delayMs == 0) {
			this->addLastCardFromImmediately(mutatesTo, source, durationMs);
			return;
		}

		threadUtils_invokeIn(delayMs, [this, mutatesTo, &source, durationMs]() {
			this->addLastCardFromImmediately(mutatesTo, source, durationMs);
		});
	}

	void CardAnimator::addLastCardFromImmediately(Card mutatesTo, CardAnimator& source, int durationMs) {
		onAnimationStart();
		source.removeLast();

		CardAnimation a(getMilliseconds(), durationMs, source, mutatesTo);
		addCardAnimation(a);

		threadUtils_invokeIn(durationMs, [this, a, mutatesTo] {
			removeCardAnimation(a);
			this->addFromPlain(mutatesTo);
			onAnimationEnd();
		});
	}

	void CardAnimator::addDeterminedCardFrom(Card card, CardAnimator& source, int durationMs, int delayMs) {
		if(delayMs == 0) {
			this->addDeterminedCardFromImmediately(card, source, durationMs);
			return;
		}

		threadUtils_invokeIn(delayMs, [this, card, &source, durationMs]() {
			this->addDeterminedCardFromImmediately(card, source, durationMs);
		});
	}

	void CardAnimator::addDeterminedCardFromImmediately(Card card, CardAnimator& source, int durationMs) {
		onAnimationStart();
		source.remove(card);

		CardAnimation a(getMilliseconds(), durationMs, source, card);
		addCardAnimation(a);

		threadUtils_invokeIn(durationMs, [this, a, card] {
			onAnimationEnd();
			removeCardAnimation(a);
			this->addFromPlain(card);
		});
	}

	void CardAnimator::addRandomCardFrom(Card mutatesTo, CardAnimator& source, int durationMs, int delayMs) {
		if(delayMs == 0) {
			addRandomCardFromImmediately(mutatesTo, source, durationMs);
			return;
		}
	
		threadUtils_invokeIn(delayMs, [this, mutatesTo, &source, durationMs]() {
			addRandomCardFromImmediately(mutatesTo, source, durationMs);
		});
	}

	void CardAnimator::addRandomCardFromImmediately(Card mutatesTo, CardAnimator& source, int durationMs) {
		if(source.isEmpty()) throw std::runtime_error("Can't add card from an empty card stack.");
		onAnimationStart();
		source.remove(randomInRange<std::size_t>(0, source.getSize() - 1));

		CardAnimation a(getMilliseconds(), durationMs, source, mutatesTo);
		addCardAnimation(a);

		threadUtils_invokeIn(durationMs, [this, a, mutatesTo] {	
			removeCardAnimation(a);
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

	void CardAnimator::addCardAnimation(CardAnimation ca) {
		animations.push_back(ca);
	}

	void CardAnimator::removeCardAnimation(CardAnimation ca) {
		animations.erase(std::find(animations.begin(), animations.end(), ca));
	}

	std::vector<CardAnimation> CardAnimator::getCardAnimations() const {
		return animations;
	}

	std::size_t CardAnimator::getSizeInclPendingTransactions() const {
		return getSize() + animations.size();
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
		if(animations.empty()) {
			return getLast();
		} else {
			return animations[animations.size() - 1].mutatesTo;
		}
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