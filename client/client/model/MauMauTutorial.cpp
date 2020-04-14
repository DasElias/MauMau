#include "MauMauTutorial.h"
#include <iostream>

namespace card {
	MauMauTutorial::MauMauTutorial(ProxyMauMauGameData& gameData, MauMauGameAccessorFromClient& accessorFromClient, ShouldDisplayTutorialInterface& shouldDisplayTutorialInterface) :
			gameData(gameData),
			accessorFromClient(accessorFromClient),
			shouldDisplayTutorialInterface(shouldDisplayTutorialInterface),
			gameMessageQueue(gameData.getMessageQueue()) {
	}
	void MauMauTutorial::onGameStart() {
		if(shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::GAME_START)) {
			gameMessageQueue.appendMessage("Herzlich willkommen bei MauMau 3D! Das Spielziel ist, als erster alle seine Karten auszuspielen.");
			gameMessageQueue.appendMessage(u8"Immer wenn du am Zug bist, kannst du eine Karte spielen, bei der entweder der Wert oder die Farbe mit der obersten Karte des Spielstapels (rechts) übereinstimmt.");
		}
	}
	void MauMauTutorial::onTurnStart(ProxyPlayer& playerWhichTurnWasStarted) {
		if(gameData.getLocalPlayer() == playerWhichTurnWasStarted) {
			bool canLocalPlayerPlayAnyCard = this->canLocalPlayerPlayAnyCard();
			std::cout << "ON TURN START " << canLocalPlayerPlayAnyCard << std::endl;

			if(playerWhichTurnWasStarted.isInSkipState() && canLocalPlayerPlayAnyCard) {
				if(shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::SKIP_STATE_AND_CAN_PLAY)) {
					gameMessageQueue.appendMessagePermanently(u8"Spiele nun eine Karte mit dem Wert 8, indem du auf eine passende Handkarte klickst, um den nächsten Spieler auszulassen.", turnStartKey);
				}
			} else if(playerWhichTurnWasStarted.isInSkipState()) {
				if(shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::SKIP_STATE)) {
					gameMessageQueue.appendMessagePermanently(u8"Leider hast du keine Karte mit dem Wert 8, um das Aussetzen weiterzureichen. Drücke deshalb den Passen-Knopf rechts.", turnStartKey);
				}
			} else if(gameData.isInDrawTwoState() && canLocalPlayerPlayAnyCard) {
				if(shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::DRAW_TWO_STATE_AND_CAN_PLAY)) {
					gameMessageQueue.appendMessagePermanently(u8"Spiele nun eine Karte mit dem Wert 7, indem du auf eine passende Handkarte klickst.", turnStartKey);
				}
			} else if(gameData.isInDrawTwoState()) {
				if(shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::DRAW_TWO_STATE)) {
					gameMessageQueue.appendMessagePermanently(u8"Leider hast du keine Karte mit dem Wert 7. Drücke deshalb den Passen-Knopf rechts.", turnStartKey);
				}
			} else if(canLocalPlayerPlayAnyCard) {
				if(shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::PLAY_CARD_STATE)) {
					gameMessageQueue.appendMessagePermanently(u8"Spiele nun eine Karte, indem du auf eine passende Handkarte klickst.", turnStartKey);
				}
			} else {
				if(shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::DRAW_CARD_STATE)) {
					gameMessageQueue.appendMessagePermanently(u8"Leider hast du nun keine passende Karte, welche du spielen könntest. Klicke deshalb auf den Ziehstapel, um eine Karte zu ziehen.", turnStartKey);
				}
			}

			if(playerWhichTurnWasStarted.getCardStack().getSize() == 2 && canLocalPlayerPlayAnyCard) {
				if(shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::MAU_REMEMBER)) {
					gameMessageQueue.appendMessage(u8"Denke daran, BEVOR du die vorletzte Karte spielst, den Mau-Button auf der rechten Seite zu betätigen.");
				}
			}
		}
	}
	void MauMauTutorial::onPlay(Card c) {
		std::cout << "ON PLAY " << std::endl;
		if(gameData.canSkipPlayer(c) && shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::SKIP_CARD_EXPLANATION)) {
			gameMessageQueue.appendMessage(u8"Das Spielen einer Karte mit dem Wert 8 führt dazu, dass der nächste Spieler ausgelassen wird, es sei denn, er kann ebenfalls eine Karte mit dem Wert 8 spielen.");
		} else if(gameData.getAmountsOfCardsToDrawForNextPlayer(c) > 0 && shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::DRAW_TWO_CARD_EXPLANATION)) {
			gameMessageQueue.appendMessage(u8"Das Spielen einer Karte mit dem Wert 7 führt dazu, dass der nächste Spieler zwei Karten ziehen muss, es sei denn, er kann ebenfalls eine Karte mit dem Wert 7 spielen.");
		}
	}
	void MauMauTutorial::onTurnEnd(ProxyPlayer& playerWhichTurnHasEnded) {
		if(gameData.getLocalPlayer() == playerWhichTurnHasEnded) {
			gameMessageQueue.removeMessagesWithKey(turnStartKey);
			if(shouldDisplayTutorialInterface.shouldDisplayAndClear(TutorialMessage::TURN_END)) {
				gameMessageQueue.appendMessage(u8"Achte darauf, dass du deinen Zug fertig ausführst, bevor deine Zeit abgelaufen ist. Ansonsten musst du zwei Strafkarten ziehen.");
			}
		}
	}
	bool MauMauTutorial::canLocalPlayerPlayAnyCard() {
		int amountsToPlay = 0;
		for(auto& card : gameData.getLocalPlayer().getCardStack()) {
			if(accessorFromClient.canPlay(card)) amountsToPlay++;
		}
		std::cout << "TO PLAY: " << amountsToPlay << std::endl;
		return amountsToPlay > 0;
	}
}