#pragma once
#include "Direction.h"
#include <vector>
#include <stdexcept>
#include "../utils/MathUtils.h"

namespace card {
	template<typename T>
	class IngamePlayerList {
        public:
            typedef typename std::vector<T>::iterator iterator;
            typedef typename std::vector<T>::const_iterator const_iterator;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::vector<T> allPlayers;
            T playerOnTurn;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            IngamePlayerList(std::vector<T> allPlayers, T playerOnTurn);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            T getPlayerOnTurn() const;
            T getNextPlayerOnTurn(Direction direction) const;
            T getNextPlayerOnTurn(Direction direction, T playerOnTurn) const;
            bool isPlayerOnTurn(const T& otherPlayer) const;

            void setPlayerOnTurn(T playerOnTurn);
            void appendPlayer(T player);
            void removePlayer(T player);

            T getRandomPlayer() const;
            const std::vector<T>& getAllPlayers() const;
            std::size_t getAmountOfPlayers() const;
            iterator begin();
            const_iterator begin() const;
            const_iterator cbegin() const;
            iterator end();
            const_iterator end() const;
            const_iterator cend() const;

	};

    template<typename T>
    inline IngamePlayerList<T>::IngamePlayerList(std::vector<T> allPlayers, T playerOnTurn) :
            allPlayers(allPlayers),
            playerOnTurn(playerOnTurn) {

    }
    template<typename T>
    inline T IngamePlayerList<T>::getPlayerOnTurn() const {
        return playerOnTurn;
    }
    template<typename T>
    inline T IngamePlayerList<T>::getNextPlayerOnTurn(Direction direction) const {
        return getNextPlayerOnTurn(direction, getPlayerOnTurn());
    }
    template<typename T>
    inline T IngamePlayerList<T>::getNextPlayerOnTurn(Direction direction, T player) const {
        auto playerOnTurnIter = std::find(allPlayers.begin(), allPlayers.end(), player);

        if(direction == Direction::CW) {
            playerOnTurnIter++;
            if(playerOnTurnIter == allPlayers.end()) playerOnTurnIter = allPlayers.begin();
        } else {
            if(playerOnTurnIter == allPlayers.begin()) playerOnTurnIter = allPlayers.end();
            playerOnTurnIter--;
        }

        return *playerOnTurnIter;
    }
    template<typename T>
    inline bool IngamePlayerList<T>::isPlayerOnTurn(const T& otherPlayer) const {
        return playerOnTurn == otherPlayer;
    }
    template<typename T>
    inline void IngamePlayerList<T>::setPlayerOnTurn(T playerOnTurn) {
        this->playerOnTurn = playerOnTurn;
    }
    template<typename T>
    inline void IngamePlayerList<T>::appendPlayer(T player) {
        allPlayers.push_back(player);
    }
    template<typename T>
    inline void IngamePlayerList<T>::removePlayer(T player) {
        if(player == playerOnTurn) throw std::runtime_error("Can't remove player on turn!");
        allPlayers.erase(std::find(allPlayers.begin(), allPlayers.end(), player));
    }
    template<typename T>
    inline T IngamePlayerList<T>::getRandomPlayer() const {
        auto rand = randomInRange<std::size_t>(0, allPlayers.size() - 1);
        return allPlayers[rand];
    }
    template<typename T>
    inline const std::vector<T>& IngamePlayerList<T>::getAllPlayers() const {
        return allPlayers;
    }
    template<typename T>
    inline std::size_t IngamePlayerList<T>::getAmountOfPlayers() const {
        return allPlayers.size();
    }
    template<typename T>
    inline typename IngamePlayerList<T>::iterator IngamePlayerList<T>::begin() {
        return allPlayers.begin();
    }
    template<typename T>
    inline typename IngamePlayerList<T>::const_iterator IngamePlayerList<T>::begin() const {
        return allPlayers.begin();
    }
    template<typename T>
    inline typename IngamePlayerList<T>::const_iterator IngamePlayerList<T>::cbegin() const {
        return allPlayers.cbegin();
    }
    template<typename T>
    inline typename IngamePlayerList<T>::iterator IngamePlayerList<T>::end() {
        return allPlayers.end();
    }
    template<typename T>
    inline typename IngamePlayerList<T>::const_iterator IngamePlayerList<T>::end() const {
        return allPlayers.end();
    }
    template<typename T>
    inline typename IngamePlayerList<T>::const_iterator IngamePlayerList<T>::cend() const {
        return allPlayers.cend();
    }
}