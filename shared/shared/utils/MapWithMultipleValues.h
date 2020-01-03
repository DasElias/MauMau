#pragma once

#include <map>
#include <vector>

namespace card {
	template<class K, class V>
	class MapWithMultipleValues {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::map<K, std::vector<V>> backboneMap;


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			inline bool doesKeyExist(const K& key) const {
				return backboneMap.find(key) != backboneMap.end();
			}
			inline void insertElement(const K& key, const V& value) {
				if(doesKeyExist(key)) {
					backboneMap.at(key).push_back(value);
				} else {
					std::vector<V> values;
					values.push_back(value);
					backboneMap.insert(std::make_pair(key, values));
				}
			}
			inline void removeElement(const K& key, const V& value) {
				if(backboneMap.find(key) == backboneMap.end()) return;

				auto& list = backboneMap.at(key);
				list.erase(std::find(list.begin(), list.end(), value));

				if(list.empty()) backboneMap.erase(key);
			}
			inline std::vector<V> getValuesOrEmptyVector(const K& key) const {
				if(doesKeyExist(key)) return backboneMap.at(key);
				else return {};
			}
			inline const std::vector<V>& getValues(const K& key) const {
				return backboneMap.at(key);
			}

	};
}
