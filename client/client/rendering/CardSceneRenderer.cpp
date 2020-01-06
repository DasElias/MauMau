#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <egui/input/IOHandler.h>
#include "CardSceneRenderer.h"
#include <iostream>
#include "../utils/VectorMathUtils.h"
#include <shared/utils/MathUtils.h>

#include <shared/utils/TimeUtils.h>
#include <shared/utils/Logger.h>

namespace card {
	const glm::vec3 CardSceneRenderer::DRAW_CARDS_POSITION = {-0.5, 0, 0};
	const glm::vec3 CardSceneRenderer::PLAY_CARDS_POSITION = {0.5, 0, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_LOCAL_POSITION = {0, -CardRenderer::HEIGHT / 2, CardSceneBackgroundRenderer::TABLE_MAX_Z};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_VISAVIS_POSITION = {0, CardRenderer::HEIGHT / 2, -CardSceneBackgroundRenderer::TABLE_MAX_Z};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_RIGHT_POSITION = {CardSceneBackgroundRenderer::TABLE_MAX_X, CardRenderer::HEIGHT / 2, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_LEFT_POSITION = {-CardSceneBackgroundRenderer::TABLE_MAX_X, CardRenderer::HEIGHT / 2, 0};

	const glm::vec3 CardSceneRenderer::DRAW_CARDS_ROTATION = {PI / 2, 0, 0};
	const glm::vec3 CardSceneRenderer::PLAY_CARDS_ROTATION = {PI / 2, PI, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_LOCAL_ROTATION = {0, 0, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_VISAVIS_ROTATION = {PI, 0, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_RIGHT_ROTATION = {PI, PI / 2, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_LEFT_ROTATION = {PI, -PI / 2, 0};

	CardSceneRenderer::CardSceneRenderer(ProjectionMatrix& projectionMatrix, Viewport& viewport, egui::MasterRenderer& eguiRenderer) :
			projectionMatrix(projectionMatrix),
			viewport(viewport),
			eguiRenderer(eguiRenderer),
			cardRenderer(renderer3d, cardTextures),
			cardStackRenderer(cardRenderer),
			handCardRenderer(cardRenderer),
			bgRenderer(renderer2d, renderer3d),
			drawnCardRenderer(cardRenderer, eguiRenderer, projectionMatrix, viewport, 
				[this]() {
					chooseCardRenderer.resetScene();
					game->playDrawnCard();
				}, 
				[this](){
					game->takeDrawnCardIntoHandCards();
				}
			),
			chooseCardRenderer(eguiRenderer, 
				[this](CardIndex c) {
					game->chooseColor(c);
				}
			),
			playerLabelRenderer(eguiRenderer, renderer2d),
			cardStackIntersectionChecker(projectionMatrix, viewport),
			handCardIntersectionChecker(projectionMatrix, viewport),
			onMouseClicked(genOnMouseClickedHandler()) {
	}

	egui::FunctionWrapper<egui::MouseEvent> CardSceneRenderer::genOnMouseClickedHandler() {
		return {[this](egui::MouseEvent& evt) {
			static bool wasMouseBtnPressedLastTime = false;

			if(evt.getMouseButton() == MOUSE_BUTTON_1) {
				if(!evt.isBtnDown() && wasMouseBtnPressedLastTime) {
					handleInput();
				}

				wasMouseBtnPressedLastTime = evt.isBtnDown();
			}
		}};
	}

	CardSceneRenderer::~CardSceneRenderer() {
		cardTextures.cleanUp();
	}

	void CardSceneRenderer::onSceneEnter(ProxyMauMauGame& game) {
		this->game = game;
		egui::getInputHandler().getMouseBtnEventManager().addEventHandler(onMouseClicked);

	}

	void CardSceneRenderer::onSceneExit() {
		egui::getInputHandler().getMouseBtnEventManager().removeEventHandler(onMouseClicked);
	}

	void CardSceneRenderer::setGame(ProxyMauMauGame& game) {
		this->game = game;
	}

	void CardSceneRenderer::render() {
		bgRenderer.render(projectionMatrix, viewport);

		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		auto opponents = game->getOpponents();
		auto opponentsOrNoneInCwOrder = mapOpponentsToTablePositionsInCwOrder(opponents);

		// render opponents
		renderOpponentIfHasValue(0, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_LEFT_POSITION, HAND_CARDS_OPPONENT_LEFT_ROTATION, 2 * CardSceneBackgroundRenderer::TABLE_MAX_Z, false);
		renderOpponentIfHasValue(1, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_VISAVIS_POSITION, HAND_CARDS_OPPONENT_VISAVIS_ROTATION, 2 * CardSceneBackgroundRenderer::TABLE_MAX_X, true);
		renderOpponentIfHasValue(2, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_RIGHT_POSITION, HAND_CARDS_OPPONENT_RIGHT_ROTATION, 2 * CardSceneBackgroundRenderer::TABLE_MAX_Z, false);

		cardRenderer.flush();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
		glDepthMask(GL_TRUE);

		// render animations of the opponents
		renderDrawedCardAnimationsOfOpponentIfHasValue(0, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_LEFT_POSITION, HAND_CARDS_OPPONENT_LEFT_ROTATION);
		renderDrawedCardAnimationsOfOpponentIfHasValue(1, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_VISAVIS_POSITION, HAND_CARDS_OPPONENT_VISAVIS_ROTATION);
		renderDrawedCardAnimationsOfOpponentIfHasValue(2, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_RIGHT_POSITION, HAND_CARDS_OPPONENT_RIGHT_ROTATION, PLAY_CARDS_POSITION + glm::vec3(0, CardRenderer::HEIGHT / 2, -CardRenderer::HEIGHT), {PI, 0, 0});

		// render card stack
		cardRenderer.flush();
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		renderDrawCardStack();

		cardRenderer.flush();
		glDisable(GL_DEPTH_TEST);

		

		renderPlayCardStack();

	



		// render local player
		renderLocalPlayer();
		renderDrawnCardOverlay();
		renderChooseColorOverlay();
		//handleInput();

		// flush CardRenderer
		cardRenderer.flush();

		renderPlayerLabels(opponentsOrNoneInCwOrder);
		glEnable(GL_DEPTH_TEST);
	}

	void CardSceneRenderer::renderOpponentIfHasValue(std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, float maxWidthOfHandCards, bool renderInX) {
		auto& opp = opponentsOrNullInCwOrder[index];
		if(opp) {
			renderOpponent(opp->getCardStack(), handCardsPosition, handCardsRotation, maxWidthOfHandCards, renderInX);
		}
	}

	void CardSceneRenderer::renderOpponent(const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, float maxWidthOfHandCards, bool renderInX) {
		if(renderInX) {
			handCardRenderer.renderCardStackInX(handCardStack, handCardsPosition, handCardsRotation, projectionMatrix, viewport, maxWidthOfHandCards);
		} else {
			handCardRenderer.renderCardStackInZ(handCardStack, handCardsPosition, handCardsRotation, projectionMatrix, viewport, maxWidthOfHandCards);
		}
	}

	void CardSceneRenderer::renderDrawedCardAnimationsOfOpponentIfHasValue(std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation) {
		auto& opp = opponentsOrNullInCwOrder[index];
		if(opp) {
			renderDrawedCardAnimationsOfOpponent(opp->getCardStack(), handCardsPosition, handCardsRotation);
		}
	}

	void CardSceneRenderer::renderDrawedCardAnimationsOfOpponent(const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation) {
		for(auto animation : handCardStack.getCardAnimations()) {
			float cardStackHeightAddition = CardStackRenderer::ADDITION_PER_CARD * (game->getDrawStack().getSize());
			this->interpolateAndRender(animation,
									   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition, 0),
									   DRAW_CARDS_ROTATION,
									   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT/2, -CardRenderer::HEIGHT),
									   DRAW_CARDS_ROTATION + glm::vec3(PI / 4, 0, 0),
									   handCardsPosition,
									   handCardsRotation);
		}
	}

	void CardSceneRenderer::renderDrawedCardAnimationsOfOpponentIfHasValue(std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, glm::vec3 middlePosition, glm::vec3 middleRotation) {
		auto& opp = opponentsOrNullInCwOrder[index];
		if(opp) {
			renderDrawedCardAnimationsOfOpponent(opp->getCardStack(), handCardsPosition, handCardsRotation,middlePosition, middleRotation);
		}
	}

	void CardSceneRenderer::renderDrawedCardAnimationsOfOpponent(const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, glm::vec3 middlePosition, glm::vec3 middleRotation) {
		for(auto animation : handCardStack.getCardAnimations()) {
			float cardStackHeightAddition = CardStackRenderer::ADDITION_PER_CARD * (game->getDrawStack().getSize());
			this->interpolateAndRender(animation,
									   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition, 0),
									   DRAW_CARDS_ROTATION,
									   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT/2, -CardRenderer::HEIGHT),
									   DRAW_CARDS_ROTATION + glm::vec3(PI / 4, 0, 0),
									   middlePosition,
									   middleRotation,
									   handCardsPosition,
									   handCardsRotation);
		}

	}

	void CardSceneRenderer::renderPlayerLabels(std::array<std::shared_ptr<ProxyPlayer>, 3>& opponents) {
		if(opponents[0]) {
			playerLabelRenderer.renderLeft(opponents[0]->getWrappedParticipiant());
		}
		if(opponents[1]) {
			playerLabelRenderer.renderVisAVis(opponents[1]->getWrappedParticipiant());
		}
		if(opponents[2]) {
			playerLabelRenderer.renderRight(opponents[2]->getWrappedParticipiant());
		}
		playerLabelRenderer.flush();
	}

	void CardSceneRenderer::renderLocalPlayer() {
		auto& localPlayer = game->getLocalPlayer();

		// render hand cards
		handCardRenderer.renderCardStackInX(localPlayer->getCardStack(), HAND_CARDS_LOCAL_POSITION, HAND_CARDS_LOCAL_ROTATION, projectionMatrix, viewport, 2 * CardSceneBackgroundRenderer::TABLE_MAX_X);

		// render cards to hand cards
		// please note that we iterate over the set from the end to the begin (we use a reverse-iterator),
		// since we want to render animations added later also to be rendered later
		// this is important when we have multiple CardAnimations from the draw card stack to the hand card stack,
		// for example when the player was forced to draw 2 cards
		auto localPlayerAnimations = localPlayer->getCardStack().getCardAnimations();
		for(auto it = localPlayerAnimations.rbegin(); it != localPlayerAnimations.rend(); ++it) {
			auto& animation = *it;

			if(animation.source.get().equalsId(game->getDrawStack())) {
				// render cards from draw card stack to hand cards
				float cardStackHeightAddition = CardStackRenderer::ADDITION_PER_CARD * (game->getDrawStack().getSize());
				this->interpolateAndRender(animation,
										   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition, 0),
										   DRAW_CARDS_ROTATION,
										   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT, CardRenderer::HEIGHT * 1.5f),
										   DRAW_CARDS_ROTATION - glm::vec3(PI / 2, 0, 0),
										   HAND_CARDS_LOCAL_POSITION,
										   HAND_CARDS_LOCAL_ROTATION);
			} else if(animation.source.get().equalsId(localPlayer->getDrawnCardAsStack())) {
				// render cards from temporary card stack to hand cards
				this->interpolateAndRender(animation,
										   DrawnCardRenderer::POSITION,
										   DrawnCardRenderer::ROTATION,
										   HAND_CARDS_LOCAL_POSITION,
										   HAND_CARDS_LOCAL_ROTATION
				);
			}	
		}

		// render cards from draw card stack to temporary card stack
		for(auto& animation : localPlayer->getDrawnCardAsStack().getCardAnimations()) {
			this->interpolateAndRender(animation,
										DRAW_CARDS_POSITION + glm::vec3(0, CardStackRenderer::ADDITION_PER_CARD * (game->getDrawStack().getSize()), 0),
										DRAW_CARDS_ROTATION,
										DRAW_CARDS_POSITION + glm::vec3(0, CardStackRenderer::ADDITION_PER_CARD * (game->getDrawStack().getSize()), CardRenderer::HEIGHT),
										DRAW_CARDS_ROTATION - glm::vec3(PI / 4, 0, 0),
										DrawnCardRenderer::POSITION,
										DrawnCardRenderer::ROTATION
			);
		}
	}

	void CardSceneRenderer::renderDrawnCardOverlay() {
		auto& localPlayer = game->getLocalPlayer();

		static std::optional<Card> drawnCardInLastPass = std::nullopt;
		std::optional<Card> drawnCard = localPlayer->getDrawnCard();

		if(drawnCard.has_value()) {
			// if the overlay wasn't displayed in the last frame, we need to ensure that there
			// are no previous mouse events which are going to fire
			if(! drawnCardInLastPass.has_value()) drawnCardRenderer.clearPreviousMouseEvents();

			cardRenderer.flush();
			drawnCardRenderer.render(*drawnCard, game->canPlayDrawnCard(), game->canTakeDrawnCardIntoHandCards());
		}

		drawnCardInLastPass = drawnCard;
	}

	void CardSceneRenderer::renderChooseColorOverlay() {
		auto& localPlayer = game->getLocalPlayer();

		if(localPlayer->isWaitingForColorPick()) {
			cardRenderer.flush();
			chooseCardRenderer.render();
		}
	}
	void CardSceneRenderer::handleInput() {
		// check if player wants to play/draw a card
		auto intersectedCardInHand = checkIntersectionWithOwnHandCards();

		if(game->canDraw() && checkIntersectionWithDrawCardStack()) {
			game->drawCard();
		} else if(intersectedCardInHand.has_value() && game->canPlay(*intersectedCardInHand)) {
			game->playCard(*intersectedCardInHand);
		}
	}
	bool CardSceneRenderer::checkIntersectionWithDrawCardStack() {
		return cardStackIntersectionChecker.doesIntersect(game->getDrawStack(), DRAW_CARDS_POSITION, DRAW_CARDS_ROTATION, CardStackRenderer::ADDITION_PER_CARD, CardRenderer::WIDTH, CardRenderer::HEIGHT);
	}

	std::optional<int> CardSceneRenderer::checkIntersectionWithOwnHandCards() {
		return handCardIntersectionChecker.getIndexOfIntersectedCardInX(game->getLocalPlayer()->getCardStack(), HAND_CARDS_LOCAL_POSITION, HAND_CARDS_LOCAL_ROTATION, 2 * CardSceneBackgroundRenderer::TABLE_MAX_X, CardRenderer::WIDTH, CardRenderer::HEIGHT);
	}

	std::array<std::shared_ptr<ProxyPlayer>, 3> CardSceneRenderer::mapOpponentsToTablePositionsInCwOrder(const std::vector<std::shared_ptr<ProxyPlayer>> opponents) {
		std::array<std::shared_ptr<ProxyPlayer>, 3> orderedOpponents;

		if(opponents.size() == 1) {
			orderedOpponents = {
				nullptr, opponents[0], nullptr
			};
		} else if(opponents.size() == 2) {
			orderedOpponents = {
				nullptr, opponents[0], opponents[1]
			};
		} else if(opponents.size() == 3) {
			orderedOpponents = {
				opponents[0], opponents[1], opponents[2]
			};
		} else {
			throw std::runtime_error("Invalid size of opponents vector.");
		}
		
		return orderedOpponents;
	}

	void CardSceneRenderer::renderDrawCardStack() {
		cardStackRenderer.renderCardStack(game->getDrawStack(), DRAW_CARDS_POSITION, DRAW_CARDS_ROTATION, projectionMatrix, viewport);
	}

	void CardSceneRenderer::renderPlayCardStack() {
		auto& cardStack = game->getPlayStack();
		auto& localPlayer = game->getLocalPlayer();
		auto& opponents = game->getOpponents();
		auto cardStacksOrNoneInCwOrder = mapOpponentsToTablePositionsInCwOrder(opponents);

		cardStackRenderer.renderCardStack(cardStack, PLAY_CARDS_POSITION, PLAY_CARDS_ROTATION, projectionMatrix, viewport);

		glm::vec3 positionEnd = PLAY_CARDS_POSITION + glm::vec3(0, cardStack.getSize() * CardStackRenderer::ADDITION_PER_CARD, 0);
		glm::vec3 rotationEnd = PLAY_CARDS_ROTATION;


		for(auto animation : cardStack.getCardAnimations()) {
			if(animation.source.get().equalsId(localPlayer->getDrawnCardAsStack())) {
				interpolateAndRender(animation, 
									DrawnCardRenderer::POSITION, {PI, PI, 0}, 
									positionEnd, rotationEnd
				);
			} else if(animation.source.get().equalsId(localPlayer->getCardStack())) {
				interpolateAndRender(animation,
									 HAND_CARDS_LOCAL_POSITION, HAND_CARDS_LOCAL_ROTATION + glm::vec3(PI, PI, 0),
									 HAND_CARDS_LOCAL_POSITION + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), HAND_CARDS_LOCAL_ROTATION + glm::vec3(PI, PI, 0),
									 positionEnd, rotationEnd
				);
			} else if(cardStacksOrNoneInCwOrder[0] && animation.source.get().equalsId(cardStacksOrNoneInCwOrder[0]->getCardStack())) {
				interpolateAndRender(animation,
									 HAND_CARDS_OPPONENT_LEFT_POSITION, glm::vec3(PI, -PI / 2, 0),
									 HAND_CARDS_OPPONENT_LEFT_POSITION + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), glm::vec3(PI, -PI / 2, 0),
									 DRAW_CARDS_POSITION + glm::vec3(0, CardRenderer::HEIGHT * 0.25f + CardStackRenderer::ADDITION_PER_CARD * (game->getDrawStack().getSize()), 0), {1.5f * PI / 2, -PI, rotationEnd.z},
									 positionEnd, {rotationEnd.x, -PI, rotationEnd.z}
				);
			} else if(cardStacksOrNoneInCwOrder[1] && animation.source.get().equalsId(cardStacksOrNoneInCwOrder[1]->getCardStack())) {
				interpolateAndRender(animation,
									 HAND_CARDS_OPPONENT_VISAVIS_POSITION, HAND_CARDS_OPPONENT_VISAVIS_ROTATION + glm::vec3(-PI, PI, 0),
									 HAND_CARDS_OPPONENT_VISAVIS_POSITION + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), HAND_CARDS_OPPONENT_VISAVIS_ROTATION + glm::vec3(-PI, PI, 0),
									 positionEnd, rotationEnd
				);
			} else if(cardStacksOrNoneInCwOrder[2] && animation.source.get().equalsId(cardStacksOrNoneInCwOrder[2]->getCardStack())) {
				interpolateAndRender(animation,
									 HAND_CARDS_OPPONENT_RIGHT_POSITION, HAND_CARDS_OPPONENT_RIGHT_ROTATION,
									 HAND_CARDS_OPPONENT_RIGHT_POSITION + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), HAND_CARDS_OPPONENT_RIGHT_ROTATION,
									 positionEnd, rotationEnd
				);
			} else {
				throw std::runtime_error("Card isn't from an user card stack.");
			}
		}
	}

	void CardSceneRenderer::interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 middle1Position, glm::vec3 middle1Rotation, glm::vec3 middle2Position, glm::vec3 middle2Rotation, glm::vec3 endPosition, glm::vec3 endRotation) {
		glm::vec3 interpolatedPosition, interpolatedRotation;

		float x = float(getMilliseconds() - animation.animationStartTime);
		float x1 = 0;
		float x2 = animation.duration / 3.0f;
		float x3 = 2 * x2;
		float x4 = float(animation.duration);

		if(x < x2) {
			// part 1 of the animation
			interpolatedPosition = interpolateLinear(x, x1, startPosition, x2, middle1Position);
			interpolatedRotation = interpolateLinear(x, x1, startRotation, x2, middle1Rotation);
		} else if (x < x3) {
			// part 2 of the animation
			interpolatedPosition = interpolateLinear(x, x2, middle1Position, x3, middle2Position);
			interpolatedRotation = interpolateLinear(x, x2, middle1Rotation, x3, middle2Rotation);
		} else {
			// part 3 of the animation
			interpolatedPosition = interpolateLinear(x, x3, middle2Position, x4, endPosition);
			interpolatedRotation = interpolateLinear(x, x3, middle2Rotation, x4, endRotation);
		}

		cardRenderer.renderInNextPass({animation.mutatesTo, interpolatedPosition, interpolatedRotation}, projectionMatrix, viewport);
	}

	void CardSceneRenderer::interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 middlePosition, glm::vec3 middleRotation, glm::vec3 endPosition, glm::vec3 endRotation) {
		glm::vec3 interpolatedPosition, interpolatedRotation;

		float x = float(getMilliseconds() - animation.animationStartTime);
		float x1 = 0;
		float x2 = animation.duration / 2.0f;
		float x3 = float(animation.duration);

		if(x < x2) {
			// part 1 of the animation
			interpolatedPosition = interpolateLinear(x, x1, startPosition, x2, middlePosition);
			interpolatedRotation = interpolateLinear(x, x1, startRotation, x2, middleRotation);
		} else {
			// part 2 of the animation
			interpolatedPosition = interpolateLinear(x, x2, middlePosition, x3, endPosition);
			interpolatedRotation = interpolateLinear(x, x2, middleRotation, x3, endRotation);
		}

		cardRenderer.renderInNextPass({animation.mutatesTo, interpolatedPosition, interpolatedRotation}, projectionMatrix, viewport);
	}

	void CardSceneRenderer::interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 endPosition, glm::vec3 endRotation) {
		glm::vec3 interpolatedPosition, interpolatedRotation;

		float x = float(getMilliseconds() - animation.animationStartTime);
		float x1 = 0;
		float x2 = float(animation.duration);

		interpolatedPosition = interpolateLinear(x, x1, startPosition, x2, endPosition);
		interpolatedRotation = interpolateLinear(x, x1, startRotation, x2, endRotation);

		cardRenderer.renderInNextPass({animation.mutatesTo, interpolatedPosition, interpolatedRotation}, projectionMatrix, viewport);

	}
	
	
}