#pragma once
#include <string>
#include <vector>
#include <list>

namespace card {
    struct Message {
        std::string content;
        long long appendUnixTimeMs;
    };

	class MessageQueue {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        public:
            static int const MAX_MSG_AMOUNT = 5;

        private:
            static int const DEFAULT_MSG_DISPLAY_DURATION = 5 * 1000;

     
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
           int msgDisplayDuration;
           std::list<Message> messages;
           
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            MessageQueue(int msgDisplayDuration = DEFAULT_MSG_DISPLAY_DURATION);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void appendMessage(std::string message);
            void clear();
            std::vector<Message> getLastVisibleMessages() const;


	};
}