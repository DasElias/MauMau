#pragma once
#include "Card.h"
#include <vector>
#include <set>
#include <initializer_list>
#include <memory>

namespace card {
	class CardCollection {
		public:
			typedef std::vector<Card>::iterator iterator;
			typedef std::vector<Card>::const_iterator const_iterator;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			std::vector<Card> cards;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CardCollection(std::vector<Card> cards = {});
			CardCollection(const CardCollection&) = delete;
			virtual ~CardCollection() = default;
			CardCollection& operator=(const CardCollection&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void addFromPlain(Card c, std::size_t amount = 1);
			void addFromPlain(std::vector<Card> cards);
			void addFromPlain(std::initializer_list<Card> cards);

			Card get(std::size_t index) const;
			Card getLast() const;
			iterator begin();
			const_iterator begin() const;
			const_iterator cbegin() const;
			iterator end();
			const_iterator end() const;
			const_iterator cend() const;
			std::vector<int> getCardNumbers() const;
			bool contains(Card c) const;
			
			std::size_t getSize() const;
			bool isEmpty() const;

			Card removeLast();
			Card remove(std::size_t index);
			Card remove(Card card);
			void clear();

		protected:
			virtual void reorder();

	};
}