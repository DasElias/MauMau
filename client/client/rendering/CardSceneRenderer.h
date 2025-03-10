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
#include "ChooseCardIndexRenderer.h"
#include "PlayerLabelRenderer.h"
#include "MauMauPlayerLabelOverlayRenderer.h"
#include "CircleSectorRenderer.h"
#include "CardIndexRenderer.h"
#include "GameEndRenderer.h"
#include "MessageRenderer.h"
#include "LocalPlayerRenderer.h"
#include "CardInterpolator.h"
#include "OpponentRenderer.h"
#include "../renderingModel/CardStackIntersectionChecker.h"
#include "../renderingModel/HandCardIntersectionChecker.h"
#include "../renderingModel/MauMauCardStackMisalignmentGenerator.h"
#include "../renderingModel/CardIndexTextures.h"
#include "PlayerLabelPositionGenerator.h"
#include "DrawCardStackRenderer.h"
#include "ClickableOverlayRenderer.h"
#include "BottomMessageRenderer.h"

namespace card {
	class CardSceneRenderer {
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
			CardInterpolator cardInterpolator;
			CardStackRenderer cardStackRenderer;
			HandCardStackRenderer handCardRenderer;
			LocalPlayerRenderer localPlayerRenderer;
			OpponentRenderer opponentRenderer;
			CardSceneBackgroundRenderer bgRenderer;
			CircleSectorRenderer circleSectorRenderer;
			PlayerLabelRenderer playerLabelRenderer;
			MauMauPlayerLabelOverlayRenderer playerLabelOverlayRenderer;
			CardIndexRenderer cardIndexRenderer;
			ParticleRenderer particleRenderer;
			GameEndRenderer fireworkRenderer;
			BottomMessageRenderer bottomMessageRenderer;
			DrawCardStackRenderer drawCardStackRenderer;
			ClickableOverlayRenderer clickableOverlayRenderer;
			CardStackIntersectionChecker cardStackIntersectionChecker;
			HandCardIntersectionChecker handCardIntersectionChecker;
			PlayerLabelPositionGenerator playerLabelPositionGenerator;

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

			void renderPlayerLabels(std::array<std::shared_ptr<ProxyPlayer>, 3>& opponents);
			void renderDrawCardStack();
			void renderPlayCardStack();
			void renderAnimationFromDrawToPlayStack(const CardAnimation& cardAnimation, glm::vec3 endPosition, glm::vec3 endRotation);
			void renderCardIndexForNextCardIfGameHasntEnded();
			void renderGameEndScreenIfGameHasEnded(float delta);
			void updateRenderGameEndScreenFlag();
			void handleInput();
			bool checkIntersectionWithDrawCardStack();

			std::array<std::shared_ptr<ProxyPlayer>, 3> mapOpponentsToTablePositionsInCwOrder(const std::vector<std::shared_ptr<ProxyPlayer>> opponents);

			
			
			
	};
}