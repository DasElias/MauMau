#include <catch2/catch.hpp>
#include <model/ProxyPlayer.h>
#include <shared/model/CardStack.h>
#include <shared/utils/ThreadUtils.h>
#include <shared/utils/InstantExecutionThreadUtilsProvider.h>
#include <iostream>

using namespace card;

TEST_CASE("ProxyPlayer", "[ProxyPlayer]") {
	threadUtils_setProvider(std::make_unique<InstantExecutionThreadUtilsProvider>());

	const std::string username = "Testnutzer";
	auto wrappedParticipant = std::make_shared<ParticipantOnClient>(username, 0, false);
	ProxyPlayer p(wrappedParticipant);
	CardAnimator drawCardStack(std::make_unique<CardStack>());
	drawCardStack.addFromPlain(Card::NULLCARD, Card::MAX_CARDS);

	CardAnimator playCardStack(std::make_unique<CardStack>());

	SECTION("test general methods after initialisation") {
		REQUIRE(p.getUsername() == username);
		REQUIRE(p.isRemainingTimeAnimationActive() == false);
		REQUIRE(p.isSkipAnimationActive() == false);
		REQUIRE(! p.getPercentOfSkipAnimationOrNone().has_value());
		REQUIRE(p.getWrappedParticipiant() == wrappedParticipant);
		REQUIRE(p.getCardStack().getSize() == 0);
	}
	SECTION("check initialisation of hand cards") {
		std::vector<Card> handCards = {Card::CLUB_ACE, Card::DIAMOND_SEVEN};
		p.initHandCards(handCards, drawCardStack, 0);

		REQUIRE(drawCardStack.getSize() == (Card::MAX_CARDS - handCards.size()));
		REQUIRE(p.getCardStack().getSize() == handCards.size());
	}
	SECTION("draw card") {
		p.drawCardInHandCards(Card::CLUB_FIVE, drawCardStack);
		
		REQUIRE(drawCardStack.getSize() == (Card::MAX_CARDS - 1));
		REQUIRE(p.getCardStack().getSize() == 1);
	}
	SECTION("play card from hand card") {
		std::vector<Card> handCards = {Card::CLUB_ACE, Card::DIAMOND_SEVEN};
		p.initHandCards(handCards, drawCardStack, 0);

		p.playCardFromHandCardsAfterDelay(Card::CLUB_EIGHT, playCardStack, 0);

		REQUIRE(playCardStack.getSize() == 1);
		REQUIRE(playCardStack.get(0) == Card::CLUB_EIGHT);
		REQUIRE(p.getCardStack().getSize() == handCards.size() - 1);
	}
	p.onEndTurn();
}