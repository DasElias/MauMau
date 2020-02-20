#include "ParticipantsOverviewState.h"
#include "ParticipantsOverviewState.h"
#include "ParticipantsOverviewState.h"
#include "ParticipantsOverviewState.h"
#include <iostream>
#include <shared/utils/MathUtils.h>
#include <egui\input\IOHandler.h>
#include <shared/utils/ThreadUtils.h>
#include <shared/utils/Logger.h>

namespace card {
	ParticipantsOverviewState::ParticipantsOverviewState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer) :
			State(stateManager),
			eguiRenderer(eguiRenderer),
			element(std::make_shared<ParticipantsOverviewElement>(avatarTextures)),
			scene(element) {

		element->addAiPlayerJoinHandler([this]() {
			auto& room = getRoom();
			if(room.canAiPlayerJoin()) {
				room.requestAiPlayerJoin();
			} else {
				log(LogSeverity::WARNING, "AI-player can't join.");
			}
		});
		element->addOptionsChangeEventHandler([this](RoomOptions options) {
			auto& room = getRoom();
			room.requestOptionChange(options);
		});
		element->addLeaveRoomHandler([this]() {
		/*	auto& room = getRoom();
			auto localParticipant = room.getLocalParticipant();
			if(room.canBeKicked(localParticipant)) {
				room.requestKickPlayer(localParticipant);
			} else {
				log(LogSeverity::WARNING, "Can't kick the local player even though the particular button was pressed.");
			}*/
			auto& stateManager = getStateMananger();
			stateManager.setGameFacade(nullptr);
			stateManager.changeState("MainMenuState");
		});
		element->setKickEventHandler([this](std::shared_ptr<ParticipantOnClient> participantToKick) {
			auto& room = getRoom();
			if(room.canBeKicked(participantToKick)) {
				room.requestKickPlayer(participantToKick);
			} else {
				log(LogSeverity::WARNING, "Can't kick player " + participantToKick->getUsername() + " even though the particular button was pressed.");
			}
		});
		element->setPromoteEventHandler([this](std::shared_ptr<ParticipantOnClient> newRoomLeader) {
			auto& room = getRoom();
			if(room.canBeRoomLeader(newRoomLeader)) {
				room.requestRoomLeaderChange(newRoomLeader);
			} else {
				log(LogSeverity::WARNING, "Can't promote player " + newRoomLeader->getUsername() + " to room leader even though the particular button was pressed.");
			}
		});
	}

	void ParticipantsOverviewState::updateAndRender(float delta) {
		auto& gameFacade = getStateMananger().getGameFacade();
		auto& room = getRoom();

		element->update(room);
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();

		
		if(room.canStartGame() && egui::getInputHandler().isKeyDown(KEY_A)) {
			room.requestGameStart();
		}

		if(gameFacade->isGameRunning()) {
			stateManager.changeState("IngameState");
		}
	}

	void ParticipantsOverviewState::onStateEnter() {
		State::onStateEnter();

		
	}

	ProxyRoom& ParticipantsOverviewState::getRoom() {
		auto& gameFacade = getStateMananger().getGameFacade();
		return gameFacade->getRoom();
	}

}