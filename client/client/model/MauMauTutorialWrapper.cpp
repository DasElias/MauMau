#include "MauMauTutorialWrapper.h"
#include <functional>
#include "ShouldDisplayTutorial.h"
#include <shared/utils/ThreadUtils.h>

namespace card {
	MauMauTutorialWrapper::MauMauTutorialWrapper(ProxyMauMauGameData& gameData, MauMauGameAccessorFromClient& accessorFromClient) :
			tutorial(gameData, accessorFromClient, shouldDisplayTutorial) {

		gameData.getTurnStartEventManager().addEventHandler([&](ProxyPlayer& p) {
			threadUtils_invokeIn(0, [&]() {
				tutorial.onTurnStart(p);
			});
		});
		gameData.getTurnEndEventManager().addEventHandler([&](ProxyPlayer& p) {
			threadUtils_invokeIn(0, [&]() {
				tutorial.onTurnEnd(p);
			});
		});
		gameData.getPlayEventManager().addEventHandler([&](Card& c) {
			threadUtils_invokeIn(0, [this, playedCard = c]() {
				tutorial.onPlay(playedCard);
			});
		});

		tutorial.onGameStart();
	}
}