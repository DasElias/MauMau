#pragma once
#include "../model/ProxyMauMauGame.h"
#include "../model/ProxyRoom.h"

#include "../renderingModel/CardTextures.h"
#include "../renderingModel/ProjectionMatrix.h"
#include "../renderingModel/Viewport.h"

#include <boost/optional.hpp>

#include "Renderer2D.h"
#include "Renderer3D.h"
#include "CardRenderer.h"
#include "CardStackRenderer.h"
#include "HandCardStackRenderer.h"
#include "CardSceneBackgroundRenderer.h"
#include "DrawnCardRenderer.h"
#include "ChooseCardRenderer.h"
#include "PlayerLabelRenderer.h"
#include "MauMauPlayerLabelOverlayRenderer.h"
#include "CircleSectorRenderer.h"
#include "CardIndexRenderer.h"
#include "GameEndRenderer.h"
#include "MauMauButtonRenderer.h"
#include "MessageRenderer.h"
#include "../renderingModel/CardStackIntersectionChecker.h"
#include "../renderingModel/HandCardIntersectionChecker.h"
#include "../renderingModel/MauMauCardStackMisalignmentGenerator.h"
#include "../renderingModel/CardIndexTextures.h"

namespace card {
	class CardSceneRenderer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static glm::vec3 const DRAW_CARDS_POSITION;
			static glm::vec3 const PLAY_CARDS_POSITION;
			static glm::vec3 const HAND_CARDS_LOCAL_POSITION;
			static glm::vec3 const HAND_CARDS_OPPONENT_VISAVIS_POSITION;
			static glm::vec3 const HAND_CARDS_OPPONENT_RIGHT_POSITION;
			static glm::vec3 const HAND_CARDS_OPPONENT_LEFT_POSITION;

			static glm::vec3 const DRAW_CARDS_ROTATION;
			static glm::vec3 const PLAY_CARDS_ROTATION;
			static glm::vec3 const HAND_CARDS_LOCAL_ROTATION;
			static glm::vec3 const HAND_CARDS_OPPONENT_VISAVIS_ROTATION;
			static glm::vec3 const HAND_CARDS_OPPONENT_RIGHT_ROTATION;
			static glm::vec3 const HAND_CARDS_OPPONENT_LEFT_ROTATION;

			static float const LEFT_RIGHT_OPPONENT_CARDS_WIDTH;
			static float const FRONT_BACK_OPPONENT_CARDS_WIDTH;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ProjectionMatrix& projectionMatrix;
			Viewport& viewport;
			egui::MasterRenderer& eguiRenderer;
			CardTextures cardTextures;
			CardIndexTextures cardIndexTextures;
			MauMauCardStackMisalignmentGenerator misalignmentGenerator;
			Renderer2D renderer2d;
			Renderer3D renderer3d;
			CardRenderer cardRenderer;
			CardStackRenderer cardStackRenderer;
			HandCardStackRenderer handCardRenderer;
			CardSceneBackgroundRenderer bgRenderer;
			DrawnCardRenderer drawnCardRenderer;
			ChooseCardRenderer chooseCardRenderer;
			CircleSectorRenderer circleSectorRenderer;
			PlayerLabelRenderer playerLabelRenderer;
			MauMauPlayerLabelOverlayRenderer playerLabelOverlayRenderer;
			CardIndexRenderer cardIndexRenderer;
			ParticleRenderer particleRenderer;
			GameEndRenderer fireworkRenderer;
			MauMauButtonRenderer mauMauButtonRenderer;
			MessageRenderer messageRenderer;
			CardStackIntersectionChecker cardStackIntersectionChecker;
			HandCardIntersectionChecker handCardIntersectionChecker;

			boost::optional<ProxyRoom&> room;
			bool shouldRenderGameEndScreen = false;

			egui::FunctionWrapper<egui::MouseEvent> onMouseClicked;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CardSceneRenderer(ProjectionMatrix& projectionMatrix, Viewport& viewport, AvatarTextures& avatarTextures,
					egui::MasterRenderer& eguiRenderer);
			~CardSceneRenderer();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void onSceneEnter(ProxyRoom& room);
			void onSceneExit();
			void render(float deltaSeconds);

		private:
			egui::FunctionWrapper<egui::MouseEvent> genOnMouseClickedHandler();

			void renderOpponentIfHasValue(std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, float maxWidthOfHandCards, bool renderInX);
			void renderOpponent(const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, float maxWidthOfHandCards, bool renderInX);
			void renderDrawedCardAnimationsOfOpponentIfHasValue(std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation);
			void renderDrawedCardAnimationsOfOpponent(const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation);
			void renderDrawedCardAnimationsOfOpponentIfHasValue(std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, glm::vec3 middlePosition, glm::vec3 middleRotation);
			void renderDrawedCardAnimationsOfOpponent(const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, glm::vec3 middlePosition, glm::vec3 middleRotation);
			void renderPlayerLabels(std::array<std::shared_ptr<ProxyPlayer>, 3>& opponents);
			void renderLocalPlayer();
			void renderClickableOverlaysIfGameHasntEnded();
			void tryRenderDrawnCardOverlay(std::optional<Card> drawnCardOrNone, bool suppressMouseClick);
			void tryRenderChooseColorOverlay();
			void renderMauButton(bool suppressMouseClick);
			void renderDrawCardStack();
			void renderPlayCardStack();
			void renderAnimationFromDrawToPlayStack(const CardAnimation& cardAnimation, glm::vec3 endPosition, glm::vec3 endRotation);
			void renderCardIndexForNextCardIfGameHasntEnded();
			void renderGameEndScreenIfGameHasEnded(float delta);
			void updateRenderGameEndScreenFlag();
			void handleInput();
			bool checkIntersectionWithDrawCardStack();
			std::optional<int> checkIntersectionWithOwnHandCards();

			std::array<std::shared_ptr<ProxyPlayer>, 3> mapOpponentsToTablePositionsInCwOrder(const std::vector<std::shared_ptr<ProxyPlayer>> opponents);

			void interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 middle1Position, glm::vec3 middle1Rotation, glm::vec3 middle2Position, glm::vec3 middle2Rotation, glm::vec3 endPosition, glm::vec3 endRotation, float period1Ratio = 0.33f, float period2Ratio = 0.33f, float period3Ratio = 0.33f);
			void interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 middlePosition, glm::vec3 middleRotation, glm::vec3 endPosition, glm::vec3 endRotation, float period1Ratio = 0.5f, float period2Ratio = 0.5f);
			void interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 endPosition, glm::vec3 endRotation);
			
			
	};
}