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
#include <res/ingame/firework.png.h>

#define LOAD_BLUE_FIREWORK loadTexture(tex_firework_fireworkBlue, tex_firework_fireworkBlue_size)
#define LOAD_GOLD_FIREWORK loadTexture(tex_firework_fireworkGold, tex_firework_fireworkGold_size)
#define LOAD_GREEN_FIREWORK loadTexture(tex_firework_fireworkGreen, tex_firework_fireworkGreen_size)
#define LOAD_ORANGE_FIREWORK loadTexture(tex_firework_fireworkOrange, tex_firework_fireworkOrange_size)
#define LOAD_PURPLE_FIREWORK loadTexture(tex_firework_fireworkPurple, tex_firework_fireworkPurple_size)
#define LOAD_RED_FIREWORK loadTexture(tex_firework_fireworkRed, tex_firework_fireworkRed_size)
#define LOAD_TURQUOISE_FIREWORK loadTexture(tex_firework_fireworkTurquoise, tex_firework_fireworkTurquoise_size)
#define LOAD_WHITE_FIREWORK loadTexture(tex_firework_fireworkWhite, tex_firework_fireworkWhite_size)

namespace card {
	GameEndRenderer::GameEndRenderer(ParticleRenderer& particleRenderer, egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D, std::function<void(void)> continueHandler) :
			fireworkViewport(glm::vec3(0, 0, 10.0f), {0, 0, 0}),
			particleRenderer(particleRenderer),
			particleManager(),
			allParticleSystems({
				std::make_shared<FireworkParticleSystem>(LOAD_GREEN_FIREWORK, 20, 1.75f, 1.8f, 0.055f),
				std::make_shared<FireworkParticleSystem>(LOAD_BLUE_FIREWORK, 20, 1.5f, 2.0f),
				std::make_shared<FireworkParticleSystem>(LOAD_TURQUOISE_FIREWORK, 20, 1.5f, 2.0f),
				std::make_shared<FireworkParticleSystem>(LOAD_GOLD_FIREWORK, 20, 1.5f, 2.0f),
				std::make_shared<FireworkParticleSystem>(LOAD_WHITE_FIREWORK, 20, 1.5f, 2.0f),
				std::make_shared<FireworkParticleSystem>(LOAD_PURPLE_FIREWORK, 20, 1.5f, 2.0f),
				std::make_shared<FireworkParticleSystem>(LOAD_RED_FIREWORK, 20, 1.5f, 2.0f),
				std::make_shared<FireworkParticleSystem>(LOAD_ORANGE_FIREWORK, 20, 1.5f, 2.0f)

			}),
			eguiRenderer(eguiRenderer),
			renderer2D(renderer2D),
			field_isAnimationActive(false),
			element(std::make_shared<GameEndElement>()),
			scene(element) {

		element->getMouseClickedEventManager().addEventHandler({[continueHandler](egui::MouseEvent& e) {
			// we don't want that the callback is executed immediately, since afterwards references on the ProxyMauMauGame object in the rendering code
			// are invalid
			threadUtils_invokeIn(0, [continueHandler]() {
				continueHandler();
			});
		}});

		for(auto& ps : allParticleSystems) {
			particleManager.add(ps);
		}
	}
	ParticleTexture GameEndRenderer::loadTexture(const unsigned char* data, std::size_t size) {
		return ParticleTexture(
			SimpleTextureFactory().setMinFilter(TextureMinFilter::NEAREST).setMagFilter(TextureMagFilter::NEAREST).loadFromMemory(data, size),
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
		scene.discardMouseEvents();

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
		generateParticlesInMs(getRandomParticleSystem(), {-0.4, 0.65}, delay);
		generateParticlesInMs(getRandomParticleSystem(), {0.6, 0.7}, delay);
		generateParticlesInMs(getRandomParticleSystem(), {-0.67, 0.14}, delay);
		generateParticlesInMs(getRandomParticleSystem(), {0.85, 0.3}, delay);
		generateParticlesInMs(getRandomParticleSystem(), {-0.8, 0.8}, delay);
		generateParticlesInMs(getRandomParticleSystem(), {0.35, 0.8}, delay);
		generateParticlesInMs(getRandomParticleSystem(), {-0.1, 0.8}, delay);

		threadUtils_invokeIn(delay, [this]() {
			generateParticlesRecursive();
		});
	}
	std::shared_ptr<FireworkParticleSystem> GameEndRenderer::getRandomParticleSystem() {
		std::size_t index = randomInRange<std::size_t>(0, allParticleSystems.size() - 1);
		return allParticleSystems[index];
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
		int const MAX_RAND_DELAY_ADDITION = 600;
		delayMs += randomInRange(MIN_RAND_DELAY_ADDITION, MAX_RAND_DELAY_ADDITION);
	}
}