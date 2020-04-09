#pragma once
#include <shared/model/Direction.h>
#include <shared/utils/MathUtils.h>
#include <vector>
#include <stdexcept>
#include <memory>
#include <shared/utils/SharedPointerComparisonUtils.h>

namespace card {
	template<typename T>
	class TemporaryIngamePlayerList {
        public:
            struct iterator {
                using value_type = T;
                using reference = T&;
                using pointer = T*;
                using difference_type = std::ptrdiff_t;
                using iterator_category = std::forward_iterator_tag;

                reference operator*() { return **curr; }
                pointer operator->() { return &**this; }
                iterator& operator++() { ++curr; return *this; }
                iterator operator++(int) { const auto temp(*this); ++*this; return temp; }
                bool operator==(const iterator& other) const { return curr == other.curr; }
                bool operator!=(const iterator& other) const { return !(*this == other); }

                typename std::vector<std::shared_ptr<T>>::iterator curr;
                typename std::vector<std::shared_ptr<T>>::iterator end;
            };
            struct const_iterator {
                using value_type = T;
                using reference = const T&;
                using pointer = const T*;
                using difference_type = std::ptrdiff_t;
                using iterator_category = std::forward_iterator_tag;

                reference operator*() { return **curr; }
                pointer operator->() { return &**this; }
                const_iterator& operator++() { ++curr; return *this; }
                const_iterator operator++(int) { const auto temp(*this); ++* this; return temp; }
                bool operator==(const const_iterator& other) const { return curr == other.curr; }
                bool operator!=(const const_iterator& other) const { return !(*this == other); }

                typename std::vector<std::shared_ptr<T>>::const_iterator curr;
                typename std::vector<std::shared_ptr<T>>::const_iterator end;
            };

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::vector<std::shared_ptr<T>> allPlayers;
            std::shared_ptr<T> playerOnTurn;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            TemporaryIngamePlayerList(std::vector<std::shared_ptr<T>> allPlayers, std::shared_ptr<T> playerOnTurn);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            T& getPlayerOnTurn();
            const T& getPlayerOnTurn() const;
            T& getNextPlayerOnTurn(Direction direction);
            T& getNextPlayerOnTurn(Direction direction, const T& playerOnTurn);
            bool isPlayerOnTurn(const T& otherPlayer) const;

            void setPlayerOnTurn(const T& playerOnTurn);
            void appendPlayer(std::shared_ptr<T> player);
            void removePlayer(const T& player);

            T& getRandomPlayer();
            const T& getRandomPlayer() const;
            const std::vector<std::shared_ptr<T>>& getAllPlayers() const;
            std::size_t getAmountOfPlayers() const;
            iterator begin();
            const_iterator begin() const;
            const_iterator cbegin() const;
            iterator end();
            const_iterator end() const;
            const_iterator cend() const;

	};
    template<typename T>
    inline TemporaryIngamePlayerList<T>::TemporaryIngamePlayerList(std::vector<std::shared_ptr<T>> allPlayers, std::shared_ptr<T> playerOnTurn) :
            allPlayers(allPlayers),
            playerOnTurn(playerOnTurn) {
    }
    template<typename T>
    inline T& TemporaryIngamePlayerList<T>::getPlayerOnTurn() {
       return *playerOnTurn;
    }
    template<typename T>
    inline const T& TemporaryIngamePlayerList<T>::getPlayerOnTurn() const {
        return *playerOnTurn;
    }
    template<typename T>
    inline T& TemporaryIngamePlayerList<T>::getNextPlayerOnTurn(Direction direction) {
        return getNextPlayerOnTurn(direction, getPlayerOnTurn());
    }
    template<typename T>
    inline T& TemporaryIngamePlayerList<T>::getNextPlayerOnTurn(Direction direction, const T& player) {
        auto playerOnTurnIter = std::find(allPlayers.begin(), allPlayers.end(), player);

        if(direction == Direction::CW) {
            playerOnTurnIter++;
            if(playerOnTurnIter == allPlayers.end()) playerOnTurnIter = allPlayers.begin();
        } else {
            if(playerOnTurnIter == allPlayers.begin()) playerOnTurnIter = allPlayers.end();
            playerOnTurnIter--;
        }

        return **playerOnTurnIter;
    }
    template<typename T>
    inline bool TemporaryIngamePlayerList<T>::isPlayerOnTurn(const T& otherPlayer) const {
        return otherPlayer == playerOnTurn;
    }
    template<typename T>
    inline void TemporaryIngamePlayerList<T>::setPlayerOnTurn(const T& playerOnTurn) {
        // find shared_ptr to playerOnTurn
        auto playerOnTurnIter = std::find(allPlayers.begin(), allPlayers.end(), playerOnTurn);
        if(playerOnTurnIter == allPlayers.end()) throw std::runtime_error("Can't set playerOnTurn to a non-existing player.");
        this->playerOnTurn = *playerOnTurnIter;
    }
    template<typename T>
    inline void TemporaryIngamePlayerList<T>::appendPlayer(std::shared_ptr<T> player) {
        allPlayers.push_back(player);
    }
    template<typename T>
    inline void TemporaryIngamePlayerList<T>::removePlayer(const T& player) {
        auto playerIter = std::find(allPlayers.begin(), allPlayers.end(), player);
        if(playerIter == allPlayers.end()) throw std::runtime_error("Can't remove a non-existing player.");
        if(player == playerOnTurn) throw std::runtime_error("Can't remove the player on turn");
        allPlayers.erase(playerIter);
    }
    template<typename T>
    inline T& TemporaryIngamePlayerList<T>::getRandomPlayer() {
        auto rand = randomInRange<std::size_t>(0, allPlayers.size() - 1);
        return *allPlayers[rand];
    }
    template<typename T>
    inline const T& TemporaryIngamePlayerList<T>::getRandomPlayer() const {
        auto rand = randomInRange<std::size_t>(0, allPlayers.size() - 1);
        return *allPlayers[rand];
    }
    template<typename T>
    inline const std::vector<std::shared_ptr<T>>& TemporaryIngamePlayerList<T>::getAllPlayers() const {
        return allPlayers;
    }
    template<typename T>
    inline std::size_t TemporaryIngamePlayerList<T>::getAmountOfPlayers() const {
        return allPlayers.size();
    }
    template<typename T>
    inline typename TemporaryIngamePlayerList<T>::iterator TemporaryIngamePlayerList<T>::begin() {
        return {allPlayers.begin(), allPlayers.end()};
    }
    template<typename T>
    inline typename TemporaryIngamePlayerList<T>::const_iterator TemporaryIngamePlayerList<T>::begin() const {
        return {allPlayers.begin(), allPlayers.end()};
    }
    template<typename T>
    inline typename TemporaryIngamePlayerList<T>::const_iterator TemporaryIngamePlayerList<T>::cbegin() const {
        return {allPlayers.begin(), allPlayers.end()};
    }
    template<typename T>
    inline typename TemporaryIngamePlayerList<T>::iterator TemporaryIngamePlayerList<T>::end() {
        return {allPlayers.end(), allPlayers.end()};
    }
    template<typename T>
    inline typename TemporaryIngamePlayerList<T>::const_iterator TemporaryIngamePlayerList<T>::end() const {
        return {allPlayers.end(), allPlayers.end()};
    }
    template<typename T>
    inline typename TemporaryIngamePlayerList<T>::const_iterator TemporaryIngamePlayerList<T>::cend() const {
        return {allPlayers.end(), allPlayers.end()};
    }
}