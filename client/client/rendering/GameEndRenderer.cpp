#include "GameEndRenderer.h"
#include "GameEndRenderer.h"
#include "GameEndRenderer.h"
#include "GameEndRenderer.h"

#include "../renderingModel/SimpleTextureFactory.h"
#include "../utils/FileUtils.h"
#include "../renderingModel/particles/FireworkParticleSystem.h"
#include <egui/input/IOHandler.h>
#include <shared/utils/MathUtils.h>
#include <shared/utils/ThreadUtils.h>
#include <algorithm>

namespace card {
	GameEndRenderer::GameEndRenderer(ParticleRenderer& particleRenderer, egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D) :
			fireworkViewport(glm::vec3(0, 0, 10.0f), {0, 0, 0}),
			particleRenderer(particleRenderer),
			particleManager(),
			redParticleSystem(std::make_shared<FireworkParticleSystem>(loadTexture("fireworkRed.png"))),
			greenParticleSystem(std::make_shared<FireworkParticleSystem>(loadTexture("fireworkGreen.png"))),
			blueParticleSystem(std::make_shared<FireworkParticleSystem>(loadTexture("fireworkBlue.png"))),
			eguiRenderer(eguiRenderer),
			renderer2D(renderer2D),
			field_isAnimationActive(false),
			element(std::make_shared<GameEndElement>()),
			scene(element) {

		particleManager.add(redParticleSystem);
		particleManager.add(greenParticleSystem);
		particleManager.add(blueParticleSystem);

	}
	ParticleTexture GameEndRenderer::loadTexture(std::string fileName) {
		return ParticleTexture(
			SimpleTextureFactory().setMinFilter(TextureMinFilter::NEAREST).setMagFilter(TextureMagFilter::NEAREST).loadFromFile(getApplicationFolder() + "\\resources\\" + fileName),
			8,
			false
		);
	}
	void GameEndRenderer::updateAndRender(float deltaSec, std::string winnerUsername, ProjectionMatrix& projectionMatrix) {
		if(! field_isAnimationActive) return;

		element->update(deltaSec, winnerUsername);
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();

		particleManager.update(deltaSec, fireworkViewport.getPosition());
		particleRenderer.render(particleManager.getList(), fireworkViewport, projectionMatrix);
	}
	void GameEndRenderer::startAnimation() {
		field_isAnimationActive = true;
		particleManager.getList().clear();

		element->startAnimation();

		int fireworkDelay = GameEndElement::ANIMATIONS_COMPLETED_DURATION;
		threadUtils_invokeIn(fireworkDelay, [this]() {
			generateParticlesRecursive();
		});
	}
	void GameEndRenderer::endAnimation() {
		field_isAnimationActive = false;
	}
	bool GameEndRenderer::isAnimationActive() const {
		return field_isAnimationActive;
	}
	void GameEndRenderer::generateParticlesRecursive() {
		if(! isAnimationActive()) return;

		int delay = 0;
		generateParticlesInMs(redParticleSystem, {-0.4, 0.65}, delay);
		generateParticlesInMs(blueParticleSystem, {0.6, 0.7}, delay);
		generateParticlesInMs(greenParticleSystem, {-0.67, 0.14}, delay);
		generateParticlesInMs(redParticleSystem, {0.85, 0.3}, delay);
		generateParticlesInMs(greenParticleSystem, {-0.55, -0.17}, delay);
		generateParticlesInMs(blueParticleSystem, {0.7, -0.5}, delay);
		generateParticlesInMs(greenParticleSystem, {-0.4, -0.55}, delay);
		generateParticlesInMs(redParticleSystem, {0.4, -0.25}, delay);
		generateParticlesInMs(blueParticleSystem, {-0.8, 0.8}, delay);
		generateParticlesInMs(redParticleSystem, {0.35, 0.8}, delay);
		generateParticlesInMs(greenParticleSystem, {-0.1, 0.8}, delay);

		threadUtils_invokeIn(delay, [this]() {
			generateParticlesRecursive();
		});
	}
	void GameEndRenderer::generateParticlesInMs(std::shared_ptr<FireworkParticleSystem> ps, glm::vec2 systemCenter, int& delayMs) {
		float const MAX_CENTER_SHIFT = 0.1f;
		systemCenter.x += randomRealInRange<float>(-MAX_CENTER_SHIFT, MAX_CENTER_SHIFT);
		systemCenter.y += randomRealInRange<float>(-MAX_CENTER_SHIFT, MAX_CENTER_SHIFT);

		threadUtils_invokeIn(delayMs, [this, systemCenter, ps]() {
			float radiusMultiplicator = randomRealInRange<float>(0.5, 1.35);
			ps->explode(systemCenter, particleManager.getList(), radiusMultiplicator);
		});

		int const MIN_RAND_DELAY_ADDITION = 250;
		int const MAX_RAND_DELAY_ADDITION = 500;
		delayMs += randomInRange(MIN_RAND_DELAY_ADDITION, MAX_RAND_DELAY_ADDITION);
	}
}