#include "ClickableOverlayRenderer.h"
#include <optional>

namespace card {
	ClickableOverlayRenderer::ClickableOverlayRenderer(egui::MasterRenderer& eguiRenderer, CardRenderer& cardRenderer, ProjectionMatrix& pm, Viewport& vp, CardIndexTextures& cardIndexTextures) :
			cardRenderer(cardRenderer),
			mauMauButtonRenderer(eguiRenderer, pm, vp),
			passButtonRenderer(eguiRenderer, pm, vp),
			drawnCardRenderer(cardRenderer, eguiRenderer, pm, vp),
			chooseCardIndexRenderer(eguiRenderer, cardIndexTextures) {
	}
	void ClickableOverlayRenderer::renderClickableOverlays(ProxyMauMauGame& game) {
		auto& clientGameAccessor = game.getAccessorFromClient();
		auto& localPlayer = game.getLocalPlayer();

		bool shouldRenderColorChooseOverlay = clientGameAccessor.isWaitingForColorChoose();
		bool shouldRenderDrawnCardOverlay = localPlayer->getCardInTempStack().has_value();
		bool shouldSuppressMauButtonClick = shouldRenderColorChooseOverlay || shouldRenderDrawnCardOverlay;
		bool shouldSuppressDrawnCardOverlayClick = shouldRenderColorChooseOverlay;

		renderMauBtnIfGameHasntEnded(game, shouldSuppressMauButtonClick);
		renderPassBtnIfGameHasntEnded(game);
		renderDrawnCardOverlayIfGameHasntEnded(game, shouldSuppressDrawnCardOverlayClick);
		renderChooseColorOverlayIfGameHasntEnded(game);
	}
	void ClickableOverlayRenderer::renderMauBtnIfGameHasntEnded(ProxyMauMauGame& game, bool suppressMouseClick) {
		if(game.hasGameEnded()) return;

		auto& clientGameAccessor = game.getAccessorFromClient();
		auto& options = game.getGameData().getOptions();
		if(!options.getOption(Options::HAVE_TO_MAU)) return;

		bool canMau = clientGameAccessor.canMau();
		bool canClickMauButton = canMau && !suppressMouseClick;
		mauMauButtonRenderer.render(canClickMauButton);
	}
	void ClickableOverlayRenderer::renderPassBtnIfGameHasntEnded(ProxyMauMauGame& game) {
		auto& clientGameAccessor = game.getAccessorFromClient();

		if(clientGameAccessor.canPass() && !game.hasGameEnded()) {
			passButtonRenderer.render();
		}
	}
	void ClickableOverlayRenderer::renderDrawnCardOverlayIfGameHasntEnded(ProxyMauMauGame& game, bool suppressMouseClick) {
		if(game.hasGameEnded()) return;

		static std::optional<Card> drawnCardInLastPass = std::nullopt;
		std::optional<Card> drawnCardOrNone = game.getLocalPlayer()->getCardInTempStack();

		auto& clientGameAccessor = game.getAccessorFromClient();
		if(drawnCardOrNone.has_value()) {
			// if the overlay wasn't displayed in the last frame, we need to ensure that there
			// are no previous mouse events which are going to fire
			if(!drawnCardInLastPass.has_value() || suppressMouseClick) drawnCardRenderer.clearPreviousMouseEvents();
			cardRenderer.flush();

			if(clientGameAccessor.isWaitingForColorChoose()) {
				drawnCardRenderer.renderOnlyCard(*drawnCardOrNone);
			} else {
				drawnCardRenderer.render(*drawnCardOrNone);
			}
		}

		drawnCardInLastPass = drawnCardOrNone;
	}
	void ClickableOverlayRenderer::renderChooseColorOverlayIfGameHasntEnded(ProxyMauMauGame& game) {
		static MessageKey messageKey = {};
		auto& messageQueue = game.getMessageQueue();

		static bool wasWaitingForColorChooseInLastPass = false;
		bool isWaitingForColorChoose = !game.hasGameEnded() && game.getAccessorFromClient().isWaitingForColorChoose();
		if(isWaitingForColorChoose) {
			if(!wasWaitingForColorChooseInLastPass) {
				chooseCardIndexRenderer.discardPreviousMouseEvents();
				messageQueue.appendMessagePermanently(u8"Wähle die zu spielende Kartenfarbe aus.", messageKey);
			}

			cardRenderer.flush();
			chooseCardIndexRenderer.render();
		} else if(wasWaitingForColorChooseInLastPass) {
			messageQueue.removeMessagesWithKey(messageKey);
		}

		wasWaitingForColorChooseInLastPass = isWaitingForColorChoose;

	}
	void ClickableOverlayRenderer::discardMouseEventsForChooseIndexRenderer() {	
		chooseCardIndexRenderer.discardPreviousMouseEvents();
	}
	void ClickableOverlayRenderer::addMauBtnClickHandler(std::function<void(void)> handler) {
		mauMauButtonRenderer.addClickHandler(handler);
	}
	void ClickableOverlayRenderer::addPassBtnClickHandler(std::function<void(void)> handler) {
		passButtonRenderer.addClickHandler(handler);
	}
	void ClickableOverlayRenderer::addPlayDrawnCardClickHandler(std::function<void(void)> handler) {
		drawnCardRenderer.addPlayHandler(handler);
	}
	void ClickableOverlayRenderer::addTakeDrawnCardIntoHandCardsClickHandler(std::function<void(void)> handler) {
		drawnCardRenderer.addDrawHandler(handler);
	}
	void ClickableOverlayRenderer::addChooseCardIndexHandler(std::function<void(CardIndex)> handler) {
		chooseCardIndexRenderer.addChooseIndexHandler(handler);
	}
}