#include "CreditsState.h"

namespace card {
	CreditsState::CreditsState(StateManager& stateManager, egui::MasterRenderer& renderer) :
			State(stateManager),
			element(std::make_shared<CreditsElement>()),
			scene(element),
			renderer(renderer) {

		element->setOnBackBtn({[this](egui::ActionEvent&) {
			getStateMananger().changeState("MainMenuState");
		}});
	
	}
	void CreditsState::updateAndRender(float delta) {
		renderer.beginFrame();
		scene.render(renderer);
		renderer.endFrame();
	}
}