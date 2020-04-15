#pragma once
#include <shared/model/IngamePlayerList.h>
#include "LocalPlayer.h"

namespace card {
	class ClientPlayerList {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            IngamePlayerList<ProxyPlayer> playerList;
            std::vector<std::shared_ptr<ProxyPlayer>> opponents;
            std::shared_ptr<LocalPlayer> localPlayer;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ClientPlayerList(std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, std::shared_ptr<ParticipantOnClient> localParticipant);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void removeOpponentLocal(const ProxyPlayer& player);
            void setPlayerOnTurn(const ProxyPlayer& p);

            ProxyPlayer& lookupPlayer(std::string username);
            ProxyPlayer& lookupOpponent(std::string username);

            bool isOpponent(std::string username) const;
            bool isPlayer(std::string username) const;
            bool isLocalPlayer(const ProxyPlayer& p) const;
            bool isPlayerOnTurn(const ProxyPlayer& p) const;
            bool isLocalPlayerOnTurn() const;

            LocalPlayer& getLocalPlayer();
            const LocalPlayer& getLocalPlayer() const;
            ProxyPlayer& getPlayerOnTurn();
            std::vector<std::shared_ptr<ProxyPlayer>> getOpponents();
            IngamePlayerList<ProxyPlayer>& getPlayers();
            const IngamePlayerList<ProxyPlayer>& getPlayers() const;
            std::size_t getAmountOfPlayers() const;
            std::size_t getAmountOfOpponents() const;


	};
}