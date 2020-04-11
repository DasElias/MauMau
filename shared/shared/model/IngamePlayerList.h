#pragma once
#include "Direction.h"
#include <vector>
#include <stdexcept>
#include "../utils/MathUtils.h"
#include "../utils/SharedPointerComparisonUtils.h"

namespace card {
	template<typename T>
	class IngamePlayerList {
        public:
            struct iterator {
                using value_type = T;
                using reference = T&;
                using pointer = T*;
                using difference_type = std::ptrdiff_t;
                using iterator_category = std::random_access_iterator_tag;

                reference operator*() { return **curr; }
                pointer operator->() { return &**this; }
                iterator& operator++() { ++curr; return *this; }
                iterator operator++(int) { const auto temp(*this); ++*this; return temp; }
                iterator& operator+=(const difference_type off) { curr += off; return *this; }
                iterator operator+(const difference_type off) const { auto temp(*this); temp += off; return temp; }
                iterator& operator--() { --curr; return *this; }
                iterator operator--(int) { const auto temp(*this); --* this; return temp; }
                iterator& operator-=(const difference_type off) { curr -= off; return *this; }
                iterator operator-(const difference_type off) const { auto temp(*this); temp -= off; return temp; }
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
                using iterator_category = std::random_access_iterator_tag;

                reference operator*() { return **curr; }
                pointer operator->() { return &**this; }
                const_iterator& operator++() { ++curr; return *this; }
                const_iterator operator++(int) { const auto temp(*this); ++* this; return temp; }
                const_iterator& operator+=(const difference_type off) { curr += off; return *this; }
                const_iterator operator+(const difference_type off) const { auto temp(*this); temp += off; return temp; }
                const_iterator& operator--() { --curr; return *this; }
                const_iterator operator--(int) { const auto temp(*this); --* this; return temp; }
                const_iterator& operator-=(const difference_type off) { curr -= off; return *this; }
                const_iterator operator-(const difference_type off) const { auto temp(*this); temp -= off; return temp; }
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
            IngamePlayerList(std::vector<std::shared_ptr<T>> allPlayers, std::shared_ptr<T> playerOnTurn);

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

        private:
            typename std::vector<std::shared_ptr<T>>::iterator findInAllPlayers(const T& val);

	};
    template<typename T>
    inline IngamePlayerList<T>::IngamePlayerList(std::vector<std::shared_ptr<T>> allPlayers, std::shared_ptr<T> playerOnTurn) :
            allPlayers(allPlayers),
            playerOnTurn(playerOnTurn) {
    }
    template<typename T>
    inline T& IngamePlayerList<T>::getPlayerOnTurn() {
       return *playerOnTurn;
    }
    template<typename T>
    inline const T& IngamePlayerList<T>::getPlayerOnTurn() const {
        return *playerOnTurn;
    }
    template<typename T>
    inline T& IngamePlayerList<T>::getNextPlayerOnTurn(Direction direction) {
        return getNextPlayerOnTurn(direction, getPlayerOnTurn());
    }
    template<typename T>
    inline T& IngamePlayerList<T>::getNextPlayerOnTurn(Direction direction, const T& player) {
        auto playerOnTurnIter = findInAllPlayers(player);

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
    inline bool IngamePlayerList<T>::isPlayerOnTurn(const T& otherPlayer) const {
        return otherPlayer == playerOnTurn;
    }
    template<typename T>
    inline void IngamePlayerList<T>::setPlayerOnTurn(const T& playerOnTurn) {
        // find shared_ptr to playerOnTurn
        auto playerOnTurnIter = findInAllPlayers(playerOnTurn);
        if(playerOnTurnIter == allPlayers.end()) throw std::runtime_error("Can't set playerOnTurn to a non-existing player.");
        this->playerOnTurn = *playerOnTurnIter;
    }
    template<typename T>
    inline void IngamePlayerList<T>::appendPlayer(std::shared_ptr<T> player) {
        allPlayers.push_back(player);
    }
    template<typename T>
    inline void IngamePlayerList<T>::removePlayer(const T& player) {
        auto playerIter = findInAllPlayers(player);
        if(playerIter == allPlayers.end()) throw std::runtime_error("Can't remove a non-existing player.");
        if(player == playerOnTurn) throw std::runtime_error("Can't remove the player on turn");
        allPlayers.erase(playerIter);
    }
    template<typename T>
    inline T& IngamePlayerList<T>::getRandomPlayer() {
        auto rand = randomInRange<std::size_t>(0, allPlayers.size() - 1);
        return *allPlayers[rand];
    }
    template<typename T>
    inline const T& IngamePlayerList<T>::getRandomPlayer() const {
        auto rand = randomInRange<std::size_t>(0, allPlayers.size() - 1);
        return *allPlayers[rand];
    }
    template<typename T>
    inline const std::vector<std::shared_ptr<T>>& IngamePlayerList<T>::getAllPlayers() const {
        return allPlayers;
    }
    template<typename T>
    inline std::size_t IngamePlayerList<T>::getAmountOfPlayers() const {
        return allPlayers.size();
    }
    template<typename T>
    inline typename IngamePlayerList<T>::iterator IngamePlayerList<T>::begin() {
        return {allPlayers.begin(), allPlayers.end()};
    }
    template<typename T>
    inline typename IngamePlayerList<T>::const_iterator IngamePlayerList<T>::begin() const {
        return {allPlayers.begin(), allPlayers.end()};
    }
    template<typename T>
    inline typename IngamePlayerList<T>::const_iterator IngamePlayerList<T>::cbegin() const {
        return {allPlayers.begin(), allPlayers.end()};
    }
    template<typename T>
    inline typename IngamePlayerList<T>::iterator IngamePlayerList<T>::end() {
        return {allPlayers.end(), allPlayers.end()};
    }
    template<typename T>
    inline typename IngamePlayerList<T>::const_iterator IngamePlayerList<T>::end() const {
        return {allPlayers.end(), allPlayers.end()};
    }
    template<typename T>
    inline typename IngamePlayerList<T>::const_iterator IngamePlayerList<T>::cend() const {
        return {allPlayers.end(), allPlayers.end()};
    }
    template<typename T>
    inline typename std::vector<std::shared_ptr<T>>::iterator IngamePlayerList<T>::findInAllPlayers(const T& val) {
        return std::find_if(allPlayers.begin(), allPlayers.end(), [&](const std::shared_ptr<T>& p) {
            return p == val;
        });
    }
}