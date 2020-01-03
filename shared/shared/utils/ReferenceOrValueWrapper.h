#pragma once
#include <boost/optional.hpp>
#include <stdexcept>
#include <memory>

namespace card {
	template<typename T>
	class ReferenceOrValueWrapper {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::unique_ptr<T> value;
            boost::optional<T&> reference;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ReferenceOrValueWrapper(T& reference) :
                    value(nullptr),
                    reference(reference) {

            }
            template<typename... Args>
            ReferenceOrValueWrapper(Args&&... args) :
                    value(std::make_unique<T>(args...)),
                    reference(boost::none) {

            }


        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            bool isStoredAsValue() const {
                return value.get() != nullptr;
            }
            bool isStoredAsReference() const {
                return reference.has_value();
            }
            T& get() {
                assert(isStoredAsValue() != isStoredAsReference());

                if(isStoredAsValue()) return *value;
                else return *reference;
            }
            T& operator*() {
                return get();
            }

	};
}