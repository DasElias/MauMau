#pragma once
#include <shared/model/Card.h>
#include <memory>

namespace card {
	class CardAnimator;

	struct CardAnimation {
		long long int animationStartTime;
		int duration;	
		std::reference_wrapper<CardAnimator> source;
		Card mutatesTo;

		CardAnimation(long long animationStartTime, int duration, CardAnimator& source, Card mutatesTo);
		bool operator<(const CardAnimation&) const;
		bool operator==(const CardAnimation&) const;
	};	
}