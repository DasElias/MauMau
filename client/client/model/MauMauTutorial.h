#pragma once
#include "ProxyMauMauGameData.h"
#include "MauMauGameAccessorFromClient.h"
#include "ShouldDisplayTutorialInterface.h"

namespace card {
	class MauMauTutorial {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            ProxyMauMauGameData& gameData;
            MauMauGameAccessorFromClient& accessorFromClient;
            MessageQueue& gameMessageQueue;
            ShouldDisplayTutorialInterface& shouldDisplayTutorialInterface;

            MessageKey turnStartKey;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            MauMauTutorial(ProxyMauMauGameData& gameData, MauMauGameAccessorFromClient& accessorFromClient, ShouldDisplayTutorialInterface& shouldDisplayTutorialInterface);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void onGameStart();
            void onTurnStart(ProxyPlayer& playerWhichTurnWasStarted);
            void onTurnEnd(ProxyPlayer& playerWhichTurnHasEnded);

        private:
            bool canLocalPlayerPlayAnyCard();
	};
}