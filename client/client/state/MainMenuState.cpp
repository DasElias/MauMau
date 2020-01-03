#include "MainMenuState.h"

namespace card {
	MainMenuState::MainMenuState(StateManager& stateManager, egui::MasterRenderer& renderer) :
			State(stateManager),
			element(std::make_shared<MainMenuElement>()),
			scene(element),
			renderer(renderer) {
		
		element->addOnSinglePlayerBtnHandler({[this](egui::ActionEvent&) {
			getStateMananger().changeState("CreateLocalRoomState");
		}});
		element->addOnJoinOnlineRoomHandler({[this](egui::ActionEvent&) {
			getStateMananger().changeState("JoinRoomState");
		}});
		element->addOnCreateOnlineRoomHandler({[this](egui::ActionEvent&) {
			getStateMananger().changeState("CreateRoomState");
		}});
	}
	void MainMenuState::updateAndRender(float delta) {
		renderer.beginFrame();
		scene.render(renderer);
		renderer.endFrame();
	}
}