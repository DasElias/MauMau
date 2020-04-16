#include "CardSceneRendererWithBlur.h"
#include "CardSceneRendererWithBlur.h"
#include <egui/input/IOHandler.h>
#include "../renderingModel/FrameBufferObject.h"
#include <shared/utils/TimeUtils.h>
#include <shared/utils/Logger.h>
#include <iostream>

namespace card {
	CardSceneRendererWithBlur::CardSceneRendererWithBlur(CardSceneRenderer& cardSceneRenderer) :
			cardSceneRenderer(cardSceneRenderer),
			outputFbo(createFbo()),
			multisampleFbo(createFbo(SAMPLES)),
			lastFrameWindowWidth(egui::getDisplayHandler().getFramebufferWidth()),
			lastFrameWindowHeight(egui::getDisplayHandler().getFramebufferHeight()),
			lastTimeResizing(LLONG_MAX - RESIZING_FBO_DELAY_MS) {
	}
	void CardSceneRendererWithBlur::renderWithGaussianBlur(float deltaSeconds) {
		auto fboStatus = this->getFboStatus();
		if(fboStatus == FboStatus::DONT_RENDER) return;
		bool shouldUpdateFbos = fboStatus == FboStatus::SHOULD_UPDATE;

		if(shouldUpdateFbos) {
			outputFbo = createFbo();
		}
		
		renderImpl(deltaSeconds, shouldUpdateFbos, *outputFbo);
		blurPostProcessor.render(outputFbo->getColorTexture(), shouldUpdateFbos);
	}
	void CardSceneRendererWithBlur::render(float deltaSeconds) {
		auto fboStatus = getFboStatus();
		if(fboStatus != FboStatus::DONT_RENDER) {
			bool shouldUpdateFbos = fboStatus == FboStatus::SHOULD_UPDATE;
			renderImpl(deltaSeconds, shouldUpdateFbos, boost::none);
		}
	}
	CardSceneRendererWithBlur::FboStatus CardSceneRendererWithBlur::getFboStatus() {
		long long currentMs = getMilliseconds();
		int newWidth = egui::getDisplayHandler().getWidth();
		int newHeight = egui::getDisplayHandler().getHeight();

		if(newWidth != this->lastFrameWindowWidth || newHeight != this->lastFrameWindowHeight) {
			this->lastTimeResizing = currentMs;
			this->lastFrameWindowWidth = newWidth;
			this->lastFrameWindowHeight = newHeight;
		}

		if(this->lastTimeResizing + RESIZING_FBO_DELAY_MS < currentMs) {
			this->lastTimeResizing = LLONG_MAX - RESIZING_FBO_DELAY_MS;

			log(LogSeverity::DEBUG, "FBOs has been re-created");
			return FboStatus::SHOULD_UPDATE;
		} else if(this->lastTimeResizing <= currentMs) {
			return FboStatus::DONT_RENDER;
		} else {
			return FboStatus::RENDER_REGULARLY;
		}
	}
	void CardSceneRendererWithBlur::renderImpl(float deltaSeconds, bool shouldUpdateFbo, boost::optional<FrameBufferObject&> resolveTo) {
		// if the window's dimension has changed, we need to create a new FrameBufferObject
		if(shouldUpdateFbo) {
			multisampleFbo = createFbo(SAMPLES);
		}

		// bind FBO
		multisampleFbo->bind();
		multisampleFbo->clear();

		cardSceneRenderer.render(deltaSeconds);

		if(resolveTo.has_value()) multisampleFbo->resolveToFbo(*resolveTo);
		else multisampleFbo->resolveToScreen();
	}
	std::unique_ptr<FrameBufferObject> CardSceneRendererWithBlur::createFbo(int samples) {
		auto newFbo = std::make_unique<FrameBufferObject>(egui::getDisplayHandler().getFramebufferWidth(), egui::getDisplayHandler().getFramebufferHeight(), samples);
		return std::move(newFbo);
	}
}