#include "ClientPlayerList.h"
#include <shared\model\PlayerNotFoundException.h>

namespace card {
	static void setLocalPlayerAtTheBeginOfVector(std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, std::shared_ptr<ParticipantOnClient> localParticipant) {
		while(allParticipantsInclLocal[0] != localParticipant) {
			allParticipantsInclLocal.push_back(allParticipantsInclLocal[0]);
			allParticipantsInclLocal.erase(allParticipantsInclLocal.begin());
		}
	}

	ClientPlayerList::ClientPlayerList(std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, std::shared_ptr<ParticipantOnClient> localParticipant) :
			playerList({}, nullptr) {	// will be initialized correctly

		setLocalPlayerAtTheBeginOfVector(allParticipantsInclLocal, localParticipant);

		// initialize players
		for(auto& o : allParticipantsInclLocal) {
			if(o == localParticipant) {
				this->localPlayer = std::make_shared<LocalPlayer>(localParticipant);
				playerList.appendPlayer(localPlayer);
			} else {
				auto player = std::make_shared<ProxyPlayer>(o);
				this->opponents.push_back(player);
				playerList.appendPlayer(player);
			}
		}
	}
	void ClientPlayerList::removeOpponentLocal(const ProxyPlayer& player) {
		auto& opponentIterator = std::find(opponents.begin(), opponents.end(), player);
		if(opponentIterator == opponents.end()) throw std::runtime_error("The player isn't an opponent.");

		opponents.erase(opponentIterator);
		playerList.removePlayer(player);
	}
	void ClientPlayerList::setPlayerOnTurn(const ProxyPlayer& p) {
		playerList.setPlayerOnTurn(p);
	}
	ProxyPlayer& ClientPlayerList::lookupPlayer(std::string username) {
		for(auto& o : playerList) {
			if(o.getUsername() == username) return o;
		}

		throw PlayerNotFoundException("\"" + username + "\" is not a player.");
	}
	ProxyPlayer& ClientPlayerList::lookupOpponent(std::string username) {
		for(auto& o : opponents) {
			if(o->getUsername() == username) return *o;
		}

		throw PlayerNotFoundException("\"" + username + "\" is not an opponent.");
	}
	bool ClientPlayerList::isOpponent(std::string username) const {
		for(auto& o : opponents) {
			if(o->getUsername() == username) return true;
		}
		return false;
	}
	bool ClientPlayerList::isPlayer(std::string username) const {
		for(auto& o : playerList) {
			if(o.getUsername() == username) return true;
		}
		return false;
	}
	bool ClientPlayerList::isLocalPlayer(const ProxyPlayer& p) const {
		std::shared_ptr<ProxyPlayer> localPlayerCasted = static_cast<std::shared_ptr<ProxyPlayer>>(localPlayer);
		return localPlayerCasted == p;
	}
	bool ClientPlayerList::isPlayerOnTurn(const ProxyPlayer& p) const {
		return playerList.isPlayerOnTurn(p);
	}
	bool ClientPlayerList::isLocalPlayerOnTurn() const {
		return playerList.isPlayerOnTurn(*localPlayer);
	}
	LocalPlayer& ClientPlayerList::getLocalPlayer() {
		return *localPlayer;
	}
	const LocalPlayer& ClientPlayerList::getLocalPlayer() const {
		return *localPlayer;
	}
	ProxyPlayer& ClientPlayerList::getPlayerOnTurn() {
		return playerList.getPlayerOnTurn();
	}
	std::vector<std::shared_ptr<ProxyPlayer>> ClientPlayerList::getOpponents() {
		return opponents;
	}
	IngamePlayerList<ProxyPlayer>& ClientPlayerList::getPlayers() {
		return playerList;
	}
	const IngamePlayerList<ProxyPlayer>& ClientPlayerList::getPlayers() const {
		return playerList;
	}
	std::size_t ClientPlayerList::getAmountOfPlayers() const {
		return playerList.getAmountOfPlayers();
	}
	std::size_t ClientPlayerList::getAmountOfOpponents() const {
		return opponents.size();
	}
}