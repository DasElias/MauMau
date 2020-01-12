#include <catch/catch.hpp>
#include <model/ProxyPlayer.h>
#include <shared/model/CardStack.h>
#include <shared/utils/ThreadUtils.h>
#include <shared/utils/InstantExecutionThreadUtilsProvider.h>
#include <iostream>

using namespace card;

TEST_CASE("ProxyPlayer can draw and play cards", "[ProxyPlayer]") {
	threadUtils_setProvider(std::make_unique<InstantExecutionThreadUtilsProvider>());

	const std::string username = "Testnutzer";
	auto wrappedParticipant = std::make_shared<ParticipantOnClient>(username, 0);
	ProxyPlayerGameInformation gameInformation;
	ProxyPlayer p(wrappedParticipant, gameInformation);
	CardAnimator drawCardStack(std::make_unique<CardStack>());
	drawCardStack.addFromPlain(Card::NULLCARD, Card::MAX_CARDS);

	CardAnimator playCardStack(std::make_unique<CardStack>());

	SECTION("test general methods after initialisation") {
		REQUIRE(p.getUsername() == username);
		REQUIRE(p.isOnTurn() == false);
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
	SECTION("draw single card") {
		p.drawSingleCardInHandCardsLocal(Card::CLUB_FIVE, drawCardStack);
		
		REQUIRE(drawCardStack.getSize() == (Card::MAX_CARDS - 1));
		REQUIRE(p.getCardStack().getSize() == 1);
	}
	SECTION("draw multiple cards") {
		std::vector<Card> cardsToDraw = {Card::CLUB_ACE, Card::DIAMOND_SEVEN};

		p.drawMultipleCardsInHandCardsLocal(cardsToDraw, drawCardStack);

		REQUIRE(drawCardStack.getSize() == (Card::MAX_CARDS - cardsToDraw.size()));
		REQUIRE(p.getCardStack().getSize() == cardsToDraw.size());
	}
	SECTION("draw multiple cards after card play time") {
		std::vector<Card> cardsToDraw = {Card::CLUB_ACE, Card::DIAMOND_SEVEN};

		p.drawMultipleCardsInHandCardsAfterCardPlayTimeLocal(cardsToDraw, drawCardStack);

		REQUIRE(drawCardStack.getSize() == (Card::MAX_CARDS - cardsToDraw.size()));
		REQUIRE(p.getCardStack().getSize() == cardsToDraw.size());
	}
	SECTION("play card from hand card") {
		std::vector<Card> handCards = {Card::CLUB_ACE, Card::DIAMOND_SEVEN};
		p.initHandCards(handCards, drawCardStack, 0);

		p.playCardFromHandCards(Card::CLUB_EIGHT, playCardStack, false);

		REQUIRE(playCardStack.getSize() == 1);
		REQUIRE(playCardStack.get(0) == Card::CLUB_EIGHT);
		REQUIRE(p.getCardStack().getSize() == handCards.size() - 1);
	}
	SECTION("play card from hand card after draw time") {
		std::vector<Card> handCards = {Card::CLUB_ACE, Card::DIAMOND_SEVEN};
		p.initHandCards(handCards, drawCardStack, 0);

		p.playCardFromHandCardsAfterDrawTime(Card::CLUB_EIGHT, playCardStack, false);

		REQUIRE(playCardStack.getSize() == 1);
		REQUIRE(playCardStack.get(0) == Card::CLUB_EIGHT);
		REQUIRE(p.getCardStack().getSize() == handCards.size() - 1);
	}
	SECTION("on skip") {
		p.onStartTurn();
		p.onSkip();

		REQUIRE(p.getPercentOfSkipAnimationOrNone().has_value());
		REQUIRE(p.getPercentOfSkipAnimation() >= 0);
		REQUIRE(p.getPercentOfSkipAnimation() <= 1);
		REQUIRE(p.isSkipAnimationActive());

	}

	p.onEndTurn();
}