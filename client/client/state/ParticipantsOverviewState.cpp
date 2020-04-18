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

		element->addGameStartHandler([this]() {
			auto& room = getRoom();
			if(room.isWaitingForResponse()) return;

			if(room.canStartGame()) {
				room.requestGameStart();
			} else {
				log(LogSeverity::WARNING, "Can't start game even though the particular button was pressed.");
			}
		});
		element->addAiPlayerJoinHandler([this]() {
			auto& room = getRoom();
			if(room.isWaitingForResponse()) return;

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
			auto& room = getRoom();
			room.requestLeave();
		});
		element->setKickEventHandler([this](std::shared_ptr<ParticipantOnClient> participantToKick) {
			auto& room = getRoom();
			if(room.isWaitingForResponse()) return;

			if(room.canBeKicked(participantToKick)) {
				room.requestKickPlayer(participantToKick);
			} else {
				log(LogSeverity::WARNING, "Can't kick player " + participantToKick->getUsername() + " even though the particular button was pressed.");
			}
		});
		element->setPromoteEventHandler([this](std::shared_ptr<ParticipantOnClient> newRoomLeader) {
			auto& room = getRoom();
			if(room.isWaitingForResponse()) return;

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

		/*
		 * We know that the local player has to be a participant of the running game, since otherwise the flag wouldn't be set.
		 */
		if(room.isGameRunning()) {
			stateManager.changeState("IngameState");
		}
	}

	void ParticipantsOverviewState::onStateEnter() {
		State::onStateEnter();
		scene.discardMouseEvents();
		
	}

	ProxyRoom& ParticipantsOverviewState::getRoom() {
		auto& gameFacade = getStateMananger().getGameFacade();
		return gameFacade->getRoom();
	}

}