#include "GameEndRenderer.h"

#include "../renderingModel/SimpleTextureFactory.h"
#include "../utils/FileUtils.h"
#include "../model/particles/FireworkParticleSystem.h"
#include <egui/input/IOHandler.h>
#include <shared/utils/MathUtils.h>
#include <shared/utils/ThreadUtils.h>
#include <algorithm>

namespace card {
	float const GameEndRenderer::START_BG_OPACITY = 0;
	float const GameEndRenderer::END_BG_OPACITY = 0.75f;

	GameEndRenderer::GameEndRenderer(ParticleRenderer& particleRenderer, egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D) :
			fireworkViewport(glm::vec3(0, 0, 10.0f), {0.5f, 0, 0}),
			particleRenderer(particleRenderer),
			particleManager(),
			redParticleSystem(std::make_shared<FireworkParticleSystem>(loadTexture("fireworkRed.png"))),
			greenParticleSystem(std::make_shared<FireworkParticleSystem>(loadTexture("fireworkGreen.png"))),
			eguiRenderer(eguiRenderer),
			renderer2D(renderer2D),
			isAnimationActive(false),
			bgOpacity(START_BG_OPACITY),
			bgElement(std::make_shared<egui::UnorganizedParentElement>()),
			bgElementColor(std::make_shared<egui::ColoredBackground>()),
			scene(bgElement) {

		bgElement->setBackground(bgElementColor);

		particleManager.add(redParticleSystem);
		particleManager.add(greenParticleSystem);

	}
	ParticleTexture GameEndRenderer::loadTexture(std::string fileName) {
		return ParticleTexture(
			SimpleTextureFactory(getApplicationFolder() + "\\resources\\"+ fileName).setMinFilter(TextureMinFilter::NEAREST).setMagFilter(TextureMagFilter::NEAREST).generateTexture(),
			8,
			false
		);
	}
	void GameEndRenderer::updateAndRender(float deltaSec, ProjectionMatrix& projectionMatrix) {
	static bool flag = false;
		if(flag && egui::getInputHandler().isKeyDown(KEY_U)) {
			startAnimation();
			flag = false;
		} else if(egui::getInputHandler().isKeyDown(KEY_T)) {
			flag = true;
		}

		if(! isAnimationActive) return;

		float deltaMs = deltaSec * 1000;
		bgOpacity += (END_BG_OPACITY - START_BG_OPACITY) / BG_OPACITY_ANIMATION_DURATION_MS * deltaMs;
		bgOpacity = clamp<float>(bgOpacity, START_BG_OPACITY, END_BG_OPACITY);

		egui::Color c(0, 0, 0, bgOpacity);
		bgElementColor->setBackgroundColor(c);

		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();

		particleManager.update(deltaSec, fireworkViewport.getPosition());

		particleRenderer.render(particleManager.getList(), fireworkViewport, projectionMatrix);
	}
	void GameEndRenderer::startAnimation() {
		bgOpacity = START_BG_OPACITY;
		isAnimationActive = true;

		float DELAY_RANDOM_ADDITION = 100;
		int delay = BG_OPACITY_ANIMATION_DURATION_MS;
		for(int i = 0; i < 100; i++) {
			generateParticlesInMs(redParticleSystem, {-4, 2, 0}, delay);
			delay += 100 + randomRealInRange(0.0f, DELAY_RANDOM_ADDITION);
			generateParticlesInMs(redParticleSystem, {-6, 0, 0}, delay);
			delay += 100 + randomRealInRange(0.0f, DELAY_RANDOM_ADDITION);
			generateParticlesInMs(greenParticleSystem, {4.5, -0.5f, 0}, delay);
			delay += 200 + randomRealInRange(0.0f, DELAY_RANDOM_ADDITION);
			generateParticlesInMs(greenParticleSystem, {3.5, 3, 0}, delay);
			delay += 250 + randomRealInRange(0.0f, DELAY_RANDOM_ADDITION);
			generateParticlesInMs(greenParticleSystem, {5, -2, 0}, delay);
			delay += 300 + randomRealInRange(0.0f, DELAY_RANDOM_ADDITION);
			generateParticlesInMs(greenParticleSystem, {2, -1, 0}, delay);
			delay += 120 + randomRealInRange(0.0f, DELAY_RANDOM_ADDITION);
			generateParticlesInMs(greenParticleSystem, {-7, 3.5, 0}, delay);
			delay += 120 + randomRealInRange(0.0f, DELAY_RANDOM_ADDITION);
			generateParticlesInMs(greenParticleSystem, {6.5, 4, 0}, delay);
			delay += 120 + randomRealInRange(0.0f, DELAY_RANDOM_ADDITION);
		}

		



	}
	void GameEndRenderer::generateParticlesInMs(std::shared_ptr<FireworkParticleSystem> ps, glm::vec3 center, int delayMs) {
		center.x += randomRealInRange<float>(-2, 2);
		center.y += randomRealInRange<float>(-2, 2);

		threadUtils_invokeIn(delayMs, [this, center, ps]() {
			
			ps->explode(center, particleManager.getList());
		});
	}
}