#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <egui/input/IOHandler.h>
#include "CardSceneRenderer.h"
#include <iostream>
#include "../utils/VectorMathUtils.h"
#include <shared/utils/MathUtils.h>
#include "CardStackPositions.h"

#include <shared/utils/TimeUtils.h>
#include <shared/utils/Logger.h>
#include <shared/utils/ThreadUtils.h>
#include <shared/model/CardAnimationDuration.h>

namespace card {
	CardSceneRenderer::CardSceneRenderer(ProjectionMatrix& projectionMatrix, Viewport& viewport, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer) :
			projectionMatrix(projectionMatrix),
			viewport(viewport),
			eguiRenderer(eguiRenderer),
			misalignmentGenerator(Card::MAX_CARDS),
			cardRenderer(renderer3d, cardTextures),
			cardStackRenderer(cardRenderer, misalignmentGenerator),
			handCardRenderer(cardRenderer),
			localPlayerRenderer(projectionMatrix, viewport, cardRenderer),
			opponentRenderer(cardRenderer, projectionMatrix, viewport),
			cardInterpolator(cardRenderer, projectionMatrix, viewport),
			bgRenderer(renderer2d, renderer3d),
			drawnCardRenderer(cardRenderer, eguiRenderer, projectionMatrix, viewport,
				[this]() {
					auto& game = room->getGame();
					auto& accessor = game.getAccessorFromClient();
					if(accessor.canPlayDrawnCard()) {
						chooseCardRenderer.discardPreviousMouseEvents();
						accessor.playDrawnCard();
					} else {
						log(LogSeverity::WARNING, "Card was drawn but can't be played.");
					}
				},
				[this]() {
					auto& game = room->getGame();
					auto& accessor = game.getAccessorFromClient();
					if(accessor.canTakeDrawnCardIntoHandCards()) {
						accessor.takeDrawnCardIntoHandCards();
					} else {
						log(LogSeverity::WARNING, "Card was drawn but can't be taken into hand cards.");
					}
				}
			),
			chooseCardRenderer(eguiRenderer,cardIndexTextures,
				[this](CardIndex c) {
					auto& game = room->getGame();
					game.getAccessorFromClient().chooseColor(c);
				}
			),
			circleSectorRenderer(),
			playerLabelRenderer(eguiRenderer, avatarTextures, renderer2d, circleSectorRenderer),
			playerLabelOverlayRenderer(renderer2d, PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN, avatarTextures.getAspectRatio()),
			cardIndexRenderer(renderer2d, cardIndexTextures),
			particleRenderer(),
			fireworkRenderer(particleRenderer, eguiRenderer, renderer2d, [this]() {
				room->returnBackToMenu(ReturnBackToMenuCause::GAME_HAS_ENDED);
			}),
			mauMauButtonRenderer(eguiRenderer,
				[this]() {
					auto& game = room->getGame();
					game.getAccessorFromClient().mau();
				}
			),
			passButtonRenderer(eguiRenderer,
				 [this]() {
					 auto& game = room->getGame();
					 game.getAccessorFromClient().pass();
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
		this->room = room;
		shouldRenderGameEndScreen = false;
		egui::getInputHandler().getMouseBtnEventManager().addEventHandler(onMouseClicked);
	}

	void CardSceneRenderer::onSceneExit() {
		egui::getInputHandler().getMouseBtnEventManager().removeEventHandler(onMouseClicked);
		fireworkRenderer.endAnimation();
		shouldRenderGameEndScreen = false;
	}

	void CardSceneRenderer::render(float deltaSeconds) {
		bgRenderer.render(projectionMatrix, viewport);

		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		auto& game = room->getGame();
		auto opponents = game.getOpponents();
		auto opponentsOrNoneInCwOrder = mapOpponentsToTablePositionsInCwOrder(opponents);

		// render opponents
		opponentRenderer.renderOpponentIfHasValue_z(0, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_LEFT_POSITION, HAND_CARDS_OPPONENT_LEFT_ROTATION);
		opponentRenderer.renderOpponentIfHasValue_x(1, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_VISAVIS_POSITION, HAND_CARDS_OPPONENT_VISAVIS_ROTATION);
		opponentRenderer.renderOpponentIfHasValue_z(2, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_RIGHT_POSITION, HAND_CARDS_OPPONENT_RIGHT_ROTATION);

		cardRenderer.flush();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
		glDepthMask(GL_TRUE);

		// render animations of the opponents
		opponentRenderer.renderDrawedCardAnimationsOfOpponentIfHasValue(game, 0, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_LEFT_POSITION, HAND_CARDS_OPPONENT_LEFT_ROTATION);
		opponentRenderer.renderDrawedCardAnimationsOfOpponentIfHasValue(game, 1, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_VISAVIS_POSITION, HAND_CARDS_OPPONENT_VISAVIS_ROTATION);
		opponentRenderer.renderDrawedCardAnimationsOfOpponentIfHasValue(game, 2, opponentsOrNoneInCwOrder, HAND_CARDS_OPPONENT_RIGHT_POSITION, HAND_CARDS_OPPONENT_RIGHT_ROTATION, PLAY_CARDS_POSITION + glm::vec3(0, CardRenderer::HEIGHT / 2, -CardRenderer::HEIGHT), {PI, 0, 0});

		// render card stack
		cardRenderer.flush();
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		renderDrawCardStack();

		cardRenderer.flush();
		glDisable(GL_DEPTH_TEST);

		

		renderPlayCardStack();


		// render local player
		localPlayerRenderer.render(game);
		cardRenderer.flush(true);
		renderPlayerLabels(opponentsOrNoneInCwOrder);
		renderCardIndexForNextCardIfGameHasntEnded();
		renderClickableOverlaysIfGameHasntEnded();

		// flush CardRenderer
		cardRenderer.flush();

		renderGameEndScreenIfGameHasEnded(deltaSeconds);
		messageRenderer.render(game.getGameData().getMessageQueue());

		glEnable(GL_DEPTH_TEST);

	}

	void CardSceneRenderer::renderPlayerLabels(std::array<std::shared_ptr<ProxyPlayer>, 3>& opponents) {
		auto& game = room->getGame();
		const auto& localPlayer = game.getLocalPlayer();

		if(opponents[0]) playerLabelRenderer.renderLeft(opponents[0]);
		if(opponents[1]) playerLabelRenderer.renderVisAVis(opponents[1]);
		if(opponents[2]) playerLabelRenderer.renderRight(opponents[2]);
		playerLabelRenderer.renderLocal(game.getLocalPlayer());
		playerLabelRenderer.flush();

		if(opponents[0]) playerLabelOverlayRenderer.render(PlayerLabelRenderer::LEFT_PLAYER_POSITION, opponents[0]->getPercentOfSkipAnimationOrNone(), opponents[0]->isInSkipState(), opponents[0]->getPercentOfMauAnimationOrNone());
		if(opponents[1]) playerLabelOverlayRenderer.render(PlayerLabelRenderer::VIS_A_VIS_PLAYER_POSITION, opponents[1]->getPercentOfSkipAnimationOrNone(), opponents[1]->isInSkipState(), opponents[1]->getPercentOfMauAnimationOrNone());
		if(opponents[2]) playerLabelOverlayRenderer.render(PlayerLabelRenderer::RIGHT_PLAYER_POSITION, opponents[2]->getPercentOfSkipAnimationOrNone(), opponents[2]->isInSkipState(), opponents[2]->getPercentOfMauAnimationOrNone());
		playerLabelOverlayRenderer.render(PlayerLabelRenderer::LOCAL_PLAYER_POSITION, localPlayer->getPercentOfSkipAnimationOrNone(), localPlayer->isInSkipState(), localPlayer->getPercentOfMauAnimationOrNone());
	}

	void CardSceneRenderer::renderClickableOverlaysIfGameHasntEnded() {
		auto& game = room->getGame();
		if(game.hasGameEnded()) return;

		auto& clientGameAccessor = game.getAccessorFromClient();
		auto& localPlayer = game.getLocalPlayer();

		bool shouldRenderColorChooseOverlay = clientGameAccessor.isWaitingForColorChoose();
		bool shouldRenderDrawnCardOverlay = localPlayer->getCardInTempStack().has_value();

		bool shouldSuppressMauButtonClick = shouldRenderColorChooseOverlay || shouldRenderDrawnCardOverlay;
		bool shouldSuppressDrawnCardOverlayClick = shouldRenderColorChooseOverlay;

		renderMauButton(shouldSuppressMauButtonClick);
		renderPassButton();
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
		static MessageKey messageKey = {};
		auto& game = room->getGame();
		auto& messageQueue = game.getMessageQueue();

		static bool wasWaitingForColorChooseInLastPass = false;
		bool isWaitingForColorChoose = game.getAccessorFromClient().isWaitingForColorChoose();
		if(isWaitingForColorChoose) {
			if(!wasWaitingForColorChooseInLastPass) {
				chooseCardRenderer.discardPreviousMouseEvents();
				messageQueue.appendMessagePermanently(u8"Wähle die zu spielende Kartenfarbe aus.", messageKey);
			}

			cardRenderer.flush();
			chooseCardRenderer.render();
		} else if(wasWaitingForColorChooseInLastPass) {
			messageQueue.removeMessagesWithKey(messageKey);
		}

		wasWaitingForColorChooseInLastPass = isWaitingForColorChoose;
	}
	void CardSceneRenderer::renderMauButton(bool suppressMouseClick) {
		auto& game = room->getGame();
		auto& clientGameAccessor = game.getAccessorFromClient();

		bool canMau = clientGameAccessor.canMau();
		bool canClickMauButton = canMau && !suppressMouseClick;
		mauMauButtonRenderer.render(canClickMauButton);
	}
	void CardSceneRenderer::renderPassButton() {
		auto& game = room->getGame();
		auto& clientGameAccessor = game.getAccessorFromClient();

		if(clientGameAccessor.canPass()) {
			passButtonRenderer.render();
		}
	}
	void CardSceneRenderer::handleInput() {
		auto& game = room->getGame();
		auto intersectedCardIndex = localPlayerRenderer.checkIntersectionWithHandCards(game);
		auto& clientGameAccessor = game.getAccessorFromClient();
		
		if(clientGameAccessor.canDraw() && checkIntersectionWithDrawCardStack()) {
			clientGameAccessor.drawCard();
		} else if(intersectedCardIndex.has_value() && clientGameAccessor.canPlay(*intersectedCardIndex)) {
			clientGameAccessor.playCard(*intersectedCardIndex);
		}
	}
	bool CardSceneRenderer::checkIntersectionWithDrawCardStack() {
		auto& game = room->getGame();
		return cardStackIntersectionChecker.doesIntersect(game.getDrawStack(), DRAW_CARDS_POSITION, DRAW_CARDS_ROTATION, CardStackRenderer::ADDITION_PER_CARD, CardRenderer::WIDTH, CardRenderer::HEIGHT);
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
		auto& game = room->getGame();
		auto& drawCardStack = game.getDrawStack();
		bool shouldDisable = game.getLocalPlayer()->isInSkipState();
		cardStackRenderer.renderCardStack(drawCardStack, DRAW_CARDS_POSITION, DRAW_CARDS_ROTATION, projectionMatrix, viewport, shouldDisable);

		auto& animations = drawCardStack.getCardAnimations();
		for(int i = 0; i < animations.size(); i++) {
			auto animation = animations[i];

			glm::vec3 heightAddition(0, i * CardStackRenderer::ADDITION_PER_CARD, 0);
			cardInterpolator.interpolateAndRender(animation,
								 PLAY_CARDS_POSITION + heightAddition, DRAW_CARDS_ROTATION,
								 DRAW_CARDS_POSITION + heightAddition, DRAW_CARDS_ROTATION
			);
		}
	}

	void CardSceneRenderer::renderPlayCardStack() {
		HandCardStackPositionGenerator handCardStackPositionGenerator;
		auto& game = room->getGame();
		auto& playCardStack = game.getPlayStack();
		auto& drawCardStack = game.getDrawStack();
		auto& localPlayer = game.getLocalPlayer();
		auto& opponents = game.getOpponents();
		auto cardStacksOrNoneInCwOrder = mapOpponentsToTablePositionsInCwOrder(opponents);

		cardStackRenderer.renderCardStackWithMisalignment(playCardStack, PLAY_CARDS_POSITION, PLAY_CARDS_ROTATION, projectionMatrix, viewport);

		int animationCounter = 0;
		for(auto animation : playCardStack.getCardAnimations()) {
			glm::vec3 positionEnd = PLAY_CARDS_POSITION + glm::vec3(0, playCardStack.getSize() * CardStackRenderer::ADDITION_PER_CARD, 0);
			glm::vec3 rotationEnd = PLAY_CARDS_ROTATION + misalignmentGenerator.computeRotationMisalignment(playCardStack.getSize() + animationCounter);
			CardAnimator& sourceStack = animation.source.get();

			if(sourceStack.equalsId(localPlayer->getTempCardStack())) {
				cardInterpolator.interpolateAndRender(animation,
									DrawnCardRenderer::POSITION, {PI, PI, 0}, 
									positionEnd, rotationEnd
				);
			} else if(sourceStack.equalsId(localPlayer->getCardStack())) {
				glm::vec3 startPosition = glm::vec3(0, LocalPlayerRenderer::HOVERED_CARD_Y_ADDITION, 0) + handCardStackPositionGenerator.getPositionOfCard_cardStackX(animation.indexInSourceStack - 1, sourceStack.getSize(), HAND_CARDS_LOCAL_POSITION, FRONT_BACK_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
				cardInterpolator.interpolateAndRender(animation,
									 startPosition, HAND_CARDS_LOCAL_ROTATION + glm::vec3(PI, PI, 0),
									 startPosition + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), HAND_CARDS_LOCAL_ROTATION + glm::vec3(PI, PI, 0),
									 positionEnd, rotationEnd
				);
			} else if(cardStacksOrNoneInCwOrder[0] && sourceStack.equalsId(cardStacksOrNoneInCwOrder[0]->getCardStack())) {
				glm::vec3 startPosition = handCardStackPositionGenerator.getPositionOfCard_cardStackZ(animation.indexInSourceStack - 1, sourceStack.getSize(), HAND_CARDS_OPPONENT_LEFT_POSITION, LEFT_RIGHT_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
				cardInterpolator.interpolateAndRender(animation,
									 startPosition, glm::vec3(PI, -PI / 2, 0),
									 startPosition + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), glm::vec3(PI, -PI / 2, 0),
									 DRAW_CARDS_POSITION + glm::vec3(0, CardRenderer::HEIGHT * 0.25f + CardStackRenderer::ADDITION_PER_CARD * (drawCardStack.getSize()), 0), {1.5f * PI / 2, -PI, rotationEnd.z},
									 positionEnd, {rotationEnd.x, -PI, rotationEnd.z}
				);
			} else if(cardStacksOrNoneInCwOrder[1] && sourceStack.equalsId(cardStacksOrNoneInCwOrder[1]->getCardStack())) {
				glm::vec3 startPosition = handCardStackPositionGenerator.getPositionOfCard_cardStackX(animation.indexInSourceStack - 1, sourceStack.getSize(), HAND_CARDS_OPPONENT_VISAVIS_POSITION, FRONT_BACK_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
				cardInterpolator.interpolateAndRender(animation,
									 startPosition, HAND_CARDS_OPPONENT_VISAVIS_ROTATION + glm::vec3(-PI, PI, 0),
									 startPosition + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), HAND_CARDS_OPPONENT_VISAVIS_ROTATION + glm::vec3(-PI, PI, 0),
									 positionEnd, rotationEnd
				);
			} else if(cardStacksOrNoneInCwOrder[2] && sourceStack.equalsId(cardStacksOrNoneInCwOrder[2]->getCardStack())) {
				glm::vec3 startPosition = handCardStackPositionGenerator.getPositionOfCard_cardStackZ(animation.indexInSourceStack - 1, sourceStack.getSize(), HAND_CARDS_OPPONENT_RIGHT_POSITION, LEFT_RIGHT_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
				cardInterpolator.interpolateAndRender(animation,
									 HAND_CARDS_OPPONENT_RIGHT_POSITION, HAND_CARDS_OPPONENT_RIGHT_ROTATION,
									 HAND_CARDS_OPPONENT_RIGHT_POSITION + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), HAND_CARDS_OPPONENT_RIGHT_ROTATION,
									 positionEnd, rotationEnd
				);
			} else if (sourceStack.equalsId(drawCardStack)) {
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

		auto& game = room->getGame();
		float drawCardStackHeightAddition = CardStackRenderer::ADDITION_PER_CARD * (game.getDrawStack().getSize());

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
			cardInterpolator.interpolateAndRender(secondAnimationPart,
								 DRAW_CARDS_POSITION + glm::vec3(CardRenderer::WIDTH / 2, drawCardStackHeightAddition + CardRenderer::WIDTH / 2, 0), 
								 glm::vec3(DRAW_CARDS_ROTATION.x, -PI / 2, DRAW_CARDS_ROTATION.z),
								 endPosition, 
								 glm::vec3(endRotation.x, -PI, endRotation.z)
			);
		}
	}

	void CardSceneRenderer::renderCardIndexForNextCardIfGameHasntEnded() {	
		auto& game = room->getGame();
		std::optional<CardIndex> cardIndexForNextCardOrNone = game.getGameData().getCardIndexForNextCardOrNone();

		const RoomOptions& options = game.getGameData().getOptions();
		bool doesColorIndexChangeOnJack = options.getOption(Options::CHOOSE_COLOR_ON_JACK);
		bool isGameRunning = !game.hasGameEnded();
		if(cardIndexForNextCardOrNone.has_value() && isGameRunning && doesColorIndexChangeOnJack) {
			cardIndexRenderer.renderCardIndexForNextCard(*cardIndexForNextCardOrNone);
		}
	}

	void CardSceneRenderer::renderGameEndScreenIfGameHasEnded(float delta) {
		updateRenderGameEndScreenFlag();

		if(shouldRenderGameEndScreen) {
			auto& game = room->getGame();
			auto winner = game.getWinnerOrNull();
			if(! winner) {
				// if there is no winner, but the game end screen should be rendered,
				// we can conclude that a new game must have been started
				onSceneExit();
				onSceneEnter(*room);
				return;
			}
			std::string winnerUsername = winner->getUsername();
			fireworkRenderer.updateAndRender(delta, winnerUsername, projectionMatrix);
		}
	}

	void CardSceneRenderer::updateRenderGameEndScreenFlag() {
		static bool hasGameEndedLastFrame = false;
		auto& game = room->getGame();
		bool hasGameEnded = game.hasGameEnded();

		if(hasGameEnded && !hasGameEndedLastFrame) {
			threadUtils_invokeIn(DRAW_DURATION_MS, [this]() {
				fireworkRenderer.startAnimation();
				shouldRenderGameEndScreen = true;
			});
		}
		hasGameEndedLastFrame = hasGameEnded;
	}
	
}