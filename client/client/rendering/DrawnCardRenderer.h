#pragma once
#include "CardRenderer.h"
#include <shared/model/CardCollection.h>
#include <glm/glm.hpp>
#include <functional>
#include <optional>

#include <egui/model/scene/Scene.h>
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodes/Button.h>
#include "../gui/ColoredButton.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "../renderingModel/WorldToScreenConverter.h"

namespace card {
	class DrawnCardRenderer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static glm::vec3 const POSITION;
			static glm::vec3 const ROTATION;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			CardRenderer& cardRenderer;
			egui::MasterRenderer& eguiRenderer;
			ProjectionMatrix& projectionMatrix;
			Viewport& viewport;
			WorldToScreenConverter worldToScreenConverter;

			std::shared_ptr<egui::RelativePositioningOnScreen> buttonBarPositioning;
			egui::Scene scene;
			std::shared_ptr<ColoredButton> playCardButton;
			std::shared_ptr<ColoredButton> takeIntoHandCardsButton;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			DrawnCardRenderer(CardRenderer& cardRenderer, egui::MasterRenderer& eguiRenderer, ProjectionMatrix& projectionMatrix, Viewport& viewport);
			DrawnCardRenderer(const DrawnCardRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void clearPreviousMouseEvents();
			void render(Card card);
			void renderOnlyCard(Card card);

			void addPlayHandler(std::function<void(void)> handler);
			void addDrawHandler(std::function<void(void)> handler);

	};
}