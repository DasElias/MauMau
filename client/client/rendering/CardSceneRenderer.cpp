#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <egui/input/IOHandler.h>
#include "CardSceneRenderer.h"
#include <iostream>
#include "../utils/VectorMathUtils.h"
#include <shared/utils/MathUtils.h>

#include <shared/utils/TimeUtils.h>
#include <shared/utils/Logger.h>
#include <shared/utils/ThreadUtils.h>
#include <shared/model/CardAnimationDuration.h>

namespace card {
	const glm::vec3 CardSceneRenderer::DRAW_CARDS_POSITION = {-0.5, 0, 0};
	const glm::vec3 CardSceneRenderer::PLAY_CARDS_POSITION = {0.5, 0, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_LOCAL_POSITION = {0, -CardRenderer::HEIGHT / 2, 1};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_VISAVIS_POSITION = {0, CardRenderer::HEIGHT / 2, -1};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_RIGHT_POSITION = {2, CardRenderer::HEIGHT / 2, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_LEFT_POSITION = {-2, CardRenderer::HEIGHT / 2, 0};

	const glm::vec3 CardSceneRenderer::DRAW_CARDS_ROTATION = {PI / 2, 0, 0};
	const glm::vec3 CardSceneRenderer::PLAY_CARDS_ROTATION = {PI / 2, PI, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_LOCAL_ROTATION = {0, 0, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_VISAVIS_ROTATION = {PI, 0, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_RIGHT_ROTATION = {PI, PI / 2, 0};
	const glm::vec3 CardSceneRenderer::HAND_CARDS_OPPONENT_LEFT_ROTATION = {PI, -PI / 2, 0};

	const float CardSceneRenderer::LEFT_RIGHT_OPPONENT_CARDS_WIDTH = 2;
	const float CardSceneRenderer::FRONT_BACK_OPPONENT_CARDS_WIDTH = 4;

	CardSceneRenderer::CardSceneRenderer(ProjectionMatrix& projectionMatrix, Viewport& viewport, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer) :
			projectionMatrix(projectionMatrix),
			viewport(viewport),
			eguiRenderer(eguiRenderer),
			misalignmentGenerator(Card::MAX_CARDS),
			cardRenderer(renderer3d, cardTextures),
			cardStackRenderer(cardRenderer, misalignmentGenerator),
			handCardRenderer(cardRenderer),
			bgRenderer(renderer2d, renderer3d),
			drawnCardRenderer(cardRenderer, eguiRenderer, projectionMatrix, viewport,
				[this]() {
					auto& accessor = game->getAccessorFromClient();
					if(accessor.canPlayDrawnCard()) {
						chooseCardRenderer.discardPreviousMouseEvents();
						accessor.playDrawnCard();
					} else {
						log(LogSeverity::WARNING, "Card was drawn but can't be played.");
					}
				},
				[this]() {
					auto& accessor = game->getAccessorFromClient();
					if(accessor.canTakeDrawnCardIntoHandCards()) {
						accessor.takeDrawnCardIntoHandCards();
					} else {
						log(LogSeverity::WARNING, "Card was drawn but can't be taken into hand cards.");
					}
				}
			),
			chooseCardRenderer(eguiRenderer,cardIndexTextures,
				[this](CardIndex c) {
					game->getAccessorFromClient().chooseColor(c);
				}
			),
			circleSectorRenderer(),
			playerLabelRenderer(eguiRenderer, avatarTextures, renderer2d, circleSectorRenderer),
			playerLabelOverlayRenderer(renderer2d, PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN),
			cardIndexRenderer(renderer2d, cardIndexTextures),
			particleRenderer(),
			fireworkRenderer(particleRenderer, eguiRenderer, renderer2d, [this]() {
				room->returnBackToMenu();
			}),
			mauMauButtonRenderer(eguiRenderer,
				[this]() {
					game->getAccessorFromClient().mau();
				}
			),
			messageRenderer(eguiRenderer),
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

	void CardSceneRenderer::onSceneEnter(ProxyRoom& room) {
		this->game = room.getGame();
		this->room = room;
		shouldRenderGameEndScreen = false;
		egui::getInputHandler().getMouseBtnEventManager().addEventHandler(onMouseClicked);
	}

	void CardSceneRenderer::onSceneExit() {
		egui::getInputHandler().getMouseBtnEventManager().removeEventHandler(onMouseClicked);
		fireworkRenderer.endAnimation();
		shouldRenderGameEndScreen = false;
	}

	void CardSceneRenderer::setGame(ProxyRoom& room) {
		this->game = room.getGame();
		this->room = room;
	}

	void CardSceneRenderer::render(float deltaSeconds) {
		bgRenderer.render(projectionMatrix, viewport);

		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		auto opponents = game->getOpponents();
		auto opponentsOrNoneInCwOrder = mapOpponentsToTablePositionsInCwOrder(opponents);

		// render opponents
		renderOpponentIfHasValue(0, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_LEFT_POSITION, HAND_CARDS_OPPONENT_LEFT_ROTATION, LEFT_RIGHT_OPPONENT_CARDS_WIDTH, false);
		renderOpponentIfHasValue(1, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_VISAVIS_POSITION, HAND_CARDS_OPPONENT_VISAVIS_ROTATION, FRONT_BACK_OPPONENT_CARDS_WIDTH, true);
		renderOpponentIfHasValue(2, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_RIGHT_POSITION, HAND_CARDS_OPPONENT_RIGHT_ROTATION, LEFT_RIGHT_OPPONENT_CARDS_WIDTH, false);

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
		cardRenderer.flush(true);
		renderPlayerLabels(opponentsOrNoneInCwOrder);
		renderCardIndexForNextCardIfGameHasntEnded();
		renderClickableOverlaysIfGameHasntEnded();

		// flush CardRenderer
		cardRenderer.flush();

		renderGameEndScreenIfGameHasEnded(deltaSeconds);
		messageRenderer.render(game->getGameData().getMessageQueue());

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
									   handCardsRotation,
									   0.33f, 0.66f);
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
									   handCardsRotation,
									   0.33f, 0.33f);
		}

	}

	void CardSceneRenderer::renderPlayerLabels(std::array<std::shared_ptr<ProxyPlayer>, 3>& opponents) {
		if(opponents[0]) playerLabelRenderer.renderLeft(opponents[0]);
		if(opponents[1]) playerLabelRenderer.renderVisAVis(opponents[1]);
		if(opponents[2]) playerLabelRenderer.renderRight(opponents[2]);
		playerLabelRenderer.renderLocal(game->getLocalPlayer());
		playerLabelRenderer.flush();

		if(opponents[0]) playerLabelOverlayRenderer.render(PlayerLabelRenderer::LEFT_PLAYER_POSITION, opponents[0]->getPercentOfSkipAnimationOrNone(), opponents[0]->getPercentOfMauAnimationOrNone());
		if(opponents[1]) playerLabelOverlayRenderer.render(PlayerLabelRenderer::VIS_A_VIS_PLAYER_POSITION, opponents[1]->getPercentOfSkipAnimationOrNone(), opponents[1]->getPercentOfMauAnimationOrNone());
		if(opponents[2]) playerLabelOverlayRenderer.render(PlayerLabelRenderer::RIGHT_PLAYER_POSITION, opponents[2]->getPercentOfSkipAnimationOrNone(), opponents[2]->getPercentOfMauAnimationOrNone());
		playerLabelOverlayRenderer.render(PlayerLabelRenderer::LOCAL_PLAYER_POSITION, game->getLocalPlayer()->getPercentOfSkipAnimationOrNone(), game->getLocalPlayer()->getPercentOfMauAnimationOrNone());
	}

	void CardSceneRenderer::renderLocalPlayer() {
		auto& localPlayer = game->getLocalPlayer();

		// render hand cards
		handCardRenderer.renderCardStackInX(localPlayer->getCardStack(), HAND_CARDS_LOCAL_POSITION, HAND_CARDS_LOCAL_ROTATION, projectionMatrix, viewport, FRONT_BACK_OPPONENT_CARDS_WIDTH);

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
										   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT / 4, CardRenderer::HEIGHT * 0.75f),
										   DRAW_CARDS_ROTATION - glm::vec3(PI / 4, 0, 0),
										   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT, CardRenderer::HEIGHT * 1.5f),
										   DRAW_CARDS_ROTATION - glm::vec3(PI / 2, 0, 0),
										   HAND_CARDS_LOCAL_POSITION,
										   HAND_CARDS_LOCAL_ROTATION);
			} else if(animation.source.get().equalsId(localPlayer->getTempCardStack())) {
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
		for(auto& animation : localPlayer->getTempCardStack().getCardAnimations()) {
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

	void CardSceneRenderer::renderClickableOverlaysIfGameHasntEnded() {
		if(game->hasGameEnded()) return;

		auto& clientGameAccessor = game->getAccessorFromClient();
		auto& localPlayer = game->getLocalPlayer();

		bool shouldRenderColorChooseOverlay = clientGameAccessor.isWaitingForColorChoose();
		bool shouldRenderDrawnCardOverlay = localPlayer->getCardInTempStack().has_value();

		bool shouldSuppressMauButtonClick = shouldRenderColorChooseOverlay || shouldRenderDrawnCardOverlay;
		bool shouldSuppressDrawnCardOverlayClick = shouldRenderColorChooseOverlay;

		renderMauButton(shouldSuppressMauButtonClick);
		tryRenderDrawnCardOverlay(localPlayer->getCardInTempStack(), shouldSuppressDrawnCardOverlayClick);
		tryRenderChooseColorOverlay();
	}

	void CardSceneRenderer::tryRenderDrawnCardOverlay(std::optional<Card> drawnCardOrNone, bool suppressMouseClick) {
		static std::optional<Card> drawnCardInLastPass = std::nullopt;

		if(drawnCardOrNone.has_value()) {
			// if the overlay wasn't displayed in the last frame, we need to ensure that there
			// are no previous mouse events which are going to fire
			if(! drawnCardInLastPass.has_value() || suppressMouseClick) drawnCardRenderer.clearPreviousMouseEvents();

			cardRenderer.flush();
			drawnCardRenderer.render(*drawnCardOrNone);
		}

		drawnCardInLastPass = drawnCardOrNone;
	}

	void CardSceneRenderer::tryRenderChooseColorOverlay() {
		static bool wasWaitingForColorChooseInLastPass = false;

		bool isWaitingForColorChoose = game->getAccessorFromClient().isWaitingForColorChoose();
		if(isWaitingForColorChoose) {
			if(! wasWaitingForColorChooseInLastPass) chooseCardRenderer.discardPreviousMouseEvents();

			cardRenderer.flush();
			chooseCardRenderer.render();
		}

		wasWaitingForColorChooseInLastPass = isWaitingForColorChoose;
	}
	void CardSceneRenderer::renderMauButton(bool suppressMouseClick) {
		auto& clientGameAccessor = game->getAccessorFromClient();

		bool canMau = clientGameAccessor.canMau();
		bool canClickMauButton = canMau && !suppressMouseClick;
		mauMauButtonRenderer.render(canClickMauButton);
	}
	void CardSceneRenderer::handleInput() {
		// check if player wants to play/draw a card
		auto intersectedCardIndex = checkIntersectionWithOwnHandCards();
		auto& clientGameAccessor = game->getAccessorFromClient();
		
		if(clientGameAccessor.canDraw() && checkIntersectionWithDrawCardStack()) {
			clientGameAccessor.drawCard();
		} else if(intersectedCardIndex.has_value() && clientGameAccessor.canPlay(*intersectedCardIndex)) {
			clientGameAccessor.playCard(*intersectedCardIndex);
		}
	}
	bool CardSceneRenderer::checkIntersectionWithDrawCardStack() {
		return cardStackIntersectionChecker.doesIntersect(game->getDrawStack(), DRAW_CARDS_POSITION, DRAW_CARDS_ROTATION, CardStackRenderer::ADDITION_PER_CARD, CardRenderer::WIDTH, CardRenderer::HEIGHT);
	}

	std::optional<int> CardSceneRenderer::checkIntersectionWithOwnHandCards() {
		return handCardIntersectionChecker.getIndexOfIntersectedCardInX(game->getLocalPlayer()->getCardStack(), HAND_CARDS_LOCAL_POSITION, HAND_CARDS_LOCAL_ROTATION, FRONT_BACK_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH, CardRenderer::HEIGHT);
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
		}
		
		return orderedOpponents;
	}

	void CardSceneRenderer::renderDrawCardStack() {
		auto& drawCardStack = game->getDrawStack();
		cardStackRenderer.renderCardStack(drawCardStack, DRAW_CARDS_POSITION, DRAW_CARDS_ROTATION, projectionMatrix, viewport);

		auto& animations = drawCardStack.getCardAnimations();
		for(int i = 0; i < animations.size(); i++) {
			auto animation = animations[i];

			glm::vec3 heightAddition(0, i * CardStackRenderer::ADDITION_PER_CARD, 0);
			interpolateAndRender(animation,
								 PLAY_CARDS_POSITION + heightAddition, DRAW_CARDS_ROTATION,
								 DRAW_CARDS_POSITION + heightAddition, DRAW_CARDS_ROTATION
			);
		}
	}

	void CardSceneRenderer::renderPlayCardStack() {
		HandCardStackPositionGenerator handCardStackPositionGenerator;
		auto& cardStack = game->getPlayStack();
		auto& localPlayer = game->getLocalPlayer();
		auto& opponents = game->getOpponents();
		auto cardStacksOrNoneInCwOrder = mapOpponentsToTablePositionsInCwOrder(opponents);

		cardStackRenderer.renderCardStackWithMisalignment(cardStack, PLAY_CARDS_POSITION, PLAY_CARDS_ROTATION, projectionMatrix, viewport);

		int animationCounter = 0;
		for(auto animation : cardStack.getCardAnimations()) {
			glm::vec3 positionEnd = PLAY_CARDS_POSITION + glm::vec3(0, cardStack.getSize() * CardStackRenderer::ADDITION_PER_CARD, 0);
			glm::vec3 rotationEnd = PLAY_CARDS_ROTATION + misalignmentGenerator.computeRotationMisalignment(cardStack.getSize() + animationCounter);
			CardAnimator& sourceStack = animation.source.get();

			if(sourceStack.equalsId(localPlayer->getTempCardStack())) {
				interpolateAndRender(animation, 
									DrawnCardRenderer::POSITION, {PI, PI, 0}, 
									positionEnd, rotationEnd
				);
			} else if(sourceStack.equalsId(localPlayer->getCardStack())) {
				glm::vec3 startPosition = handCardStackPositionGenerator.getPositionOfCard_cardStackX(animation.indexInSourceStack - 1, sourceStack.getSize(), HAND_CARDS_LOCAL_POSITION, FRONT_BACK_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
				interpolateAndRender(animation,
									 startPosition, HAND_CARDS_LOCAL_ROTATION + glm::vec3(PI, PI, 0),
									 startPosition + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), HAND_CARDS_LOCAL_ROTATION + glm::vec3(PI, PI, 0),
									 positionEnd, rotationEnd
				);
			} else if(cardStacksOrNoneInCwOrder[0] && sourceStack.equalsId(cardStacksOrNoneInCwOrder[0]->getCardStack())) {
				glm::vec3 startPosition = handCardStackPositionGenerator.getPositionOfCard_cardStackZ(animation.indexInSourceStack - 1, sourceStack.getSize(), HAND_CARDS_OPPONENT_LEFT_POSITION, LEFT_RIGHT_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
				interpolateAndRender(animation,
									 startPosition, glm::vec3(PI, -PI / 2, 0),
									 startPosition + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), glm::vec3(PI, -PI / 2, 0),
									 DRAW_CARDS_POSITION + glm::vec3(0, CardRenderer::HEIGHT * 0.25f + CardStackRenderer::ADDITION_PER_CARD * (game->getDrawStack().getSize()), 0), {1.5f * PI / 2, -PI, rotationEnd.z},
									 positionEnd, {rotationEnd.x, -PI, rotationEnd.z}
				);
			} else if(cardStacksOrNoneInCwOrder[1] && sourceStack.equalsId(cardStacksOrNoneInCwOrder[1]->getCardStack())) {
				glm::vec3 startPosition = handCardStackPositionGenerator.getPositionOfCard_cardStackX(animation.indexInSourceStack - 1, sourceStack.getSize(), HAND_CARDS_OPPONENT_VISAVIS_POSITION, FRONT_BACK_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
				interpolateAndRender(animation,
									 startPosition, HAND_CARDS_OPPONENT_VISAVIS_ROTATION + glm::vec3(-PI, PI, 0),
									 startPosition + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), HAND_CARDS_OPPONENT_VISAVIS_ROTATION + glm::vec3(-PI, PI, 0),
									 positionEnd, rotationEnd
				);
			} else if(cardStacksOrNoneInCwOrder[2] && sourceStack.equalsId(cardStacksOrNoneInCwOrder[2]->getCardStack())) {
				glm::vec3 startPosition = handCardStackPositionGenerator.getPositionOfCard_cardStackZ(animation.indexInSourceStack - 1, sourceStack.getSize(), HAND_CARDS_OPPONENT_RIGHT_POSITION, LEFT_RIGHT_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
				interpolateAndRender(animation,
									 HAND_CARDS_OPPONENT_RIGHT_POSITION, HAND_CARDS_OPPONENT_RIGHT_ROTATION,
									 HAND_CARDS_OPPONENT_RIGHT_POSITION + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), HAND_CARDS_OPPONENT_RIGHT_ROTATION,
									 positionEnd, rotationEnd
				);
			} else if (sourceStack.equalsId(game->getDrawStack())) {
				renderAnimationFromDrawToPlayStack(animation, positionEnd, rotationEnd);
			} else {
				throw std::runtime_error("Card isn't from an user card stack.");
			}
			
			animationCounter++;
		}
	}

	void CardSceneRenderer::renderAnimationFromDrawToPlayStack(const CardAnimation& cardAnimation, glm::vec3 endPosition, glm::vec3 endRotation) {
		CardAnimation firstAnimationPart = cardAnimation;
		firstAnimationPart.duration /= 2;
		CardAnimation secondAnimationPart = firstAnimationPart;
		secondAnimationPart.animationStartTime += firstAnimationPart.duration;

		float drawCardStackHeightAddition = CardStackRenderer::ADDITION_PER_CARD * (game->getDrawStack().getSize());

		float x = float(getMilliseconds() - firstAnimationPart.animationStartTime);
		float x1 = 0;
		float x2 = float(firstAnimationPart.duration);
		float x3 = x2 + float(secondAnimationPart.duration);

		if(x < x2) {
			float arc = interpolateLinear(x, x1, 0, x2, PI/2);
			float xAddition = cos(arc) * CardRenderer::WIDTH/2;
			float yAddition = sin(arc) * CardRenderer::WIDTH/2;

			//glm::vec2 interpolatedAddition = interpolateInCircle(firstAnimation, {-CardRenderer::WIDTH / 2, 0}, {0, CardRenderer::WIDTH / 2}, startArc, endArc);
			cardRenderer.renderInNextPass({firstAnimationPart.mutatesTo,
								DRAW_CARDS_POSITION + glm::vec3(-xAddition + CardRenderer::WIDTH / 2, drawCardStackHeightAddition + yAddition, 0),
								glm::vec3(DRAW_CARDS_ROTATION.x, -arc, DRAW_CARDS_ROTATION.z)},
								projectionMatrix, viewport);
		} else {

			interpolateAndRender(secondAnimationPart,
								 DRAW_CARDS_POSITION + glm::vec3(CardRenderer::WIDTH / 2, drawCardStackHeightAddition + CardRenderer::WIDTH / 2, 0), 
								 glm::vec3(DRAW_CARDS_ROTATION.x, -PI / 2, DRAW_CARDS_ROTATION.z),
								 endPosition, 
								 glm::vec3(endRotation.x, -PI, endRotation.z)
			);
		}
	}

	void CardSceneRenderer::renderCardIndexForNextCardIfGameHasntEnded() {	
		std::optional<CardIndex> cardIndexForNextCardOrNone = game->getGameData().getCardIndexForNextCardOrNone();

		const RoomOptions& options = game->getGameData().getOptions();
		bool doesColorIndexChangeOnJack = options.getOption(Options::CHOOSE_COLOR_ON_JACK);
		bool isGameRunning = !game->hasGameEnded();
		if(cardIndexForNextCardOrNone.has_value() && isGameRunning && doesColorIndexChangeOnJack) {
			cardIndexRenderer.renderCardIndexForNextCard(*cardIndexForNextCardOrNone);
		}
	}

	void CardSceneRenderer::renderGameEndScreenIfGameHasEnded(float delta) {
		updateRenderGameEndScreenFlag();

		if(shouldRenderGameEndScreen) {
			auto winner = game->getWinnerOrNull();
			assert(winner);

			std::string winnerUsername = winner->getUsername();
			fireworkRenderer.updateAndRender(delta, winnerUsername, projectionMatrix);
		}
	}

	void CardSceneRenderer::updateRenderGameEndScreenFlag() {
		static bool hasGameEndedLastFrame = false;
		bool hasGameEnded = game->hasGameEnded();

		if(hasGameEnded && !hasGameEndedLastFrame) {
			threadUtils_invokeIn(DRAW_DURATION_MS, [this]() {
				fireworkRenderer.startAnimation();
				shouldRenderGameEndScreen = true;
			});
		}
		hasGameEndedLastFrame = hasGameEnded;
	}

	void CardSceneRenderer::interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 middle1Position, glm::vec3 middle1Rotation, glm::vec3 middle2Position, glm::vec3 middle2Rotation, glm::vec3 endPosition, glm::vec3 endRotation,
												 float period1Ratio, float period2Ratio, float period3Ratio) {
		float ratioSum = period1Ratio + period2Ratio + period3Ratio;
		float TOLERANCE = 0.02f;
		if(ratioSum < 1 - TOLERANCE || ratioSum > 1 + TOLERANCE) {
			throw std::runtime_error("ratio arguments doesn't sum up to 1");
		}

		glm::vec3 interpolatedPosition, interpolatedRotation;

		float x = float(getMilliseconds() - animation.animationStartTime);
		float x1 = 0;
		float x2 = animation.duration * period1Ratio;
		float x3 = x2 + animation.duration * period2Ratio;
		float x4 = x3 + animation.duration * period3Ratio;

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

	void CardSceneRenderer::interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 middlePosition, glm::vec3 middleRotation, glm::vec3 endPosition, glm::vec3 endRotation, float period1Ratio, float period2Ratio) {
		float ratioSum = period1Ratio + period2Ratio;
		float TOLERANCE = 0.02f;
		if(ratioSum < 1 - TOLERANCE || ratioSum > 1 + TOLERANCE) {
			throw std::runtime_error("ratio arguments doesn't sum up to 1");
		}

		glm::vec3 interpolatedPosition, interpolatedRotation;

		float x = float(getMilliseconds() - animation.animationStartTime);
		float x1 = 0;
		float x2 = animation.duration * period1Ratio;
		float x3 = x2 + animation.duration * period2Ratio;
		
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