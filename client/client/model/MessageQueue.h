#pragma once
#include <string>
#include <vector>
#include <list>

namespace card {
    class MessageKey {
        public:
            MessageKey();
            bool operator==(const MessageKey&) const;

        private:
            std::uint64_t value;

        friend class MessageQueue;
    };

    struct Message {
        std::string content;
        long long removeUnixTimeMs;
        MessageKey removeKey;
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
           int defaultMsgDisplayDuration;
           std::list<Message> messages;
           
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            MessageQueue(int defaultMsgDisplayDuration = DEFAULT_MSG_DISPLAY_DURATION);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void appendMessage(std::string message);
            void appendMessage(std::string message, int displayDuration);
            void appendMessagePermanently(std::string message, const MessageKey& removeKey);
            void removeMessagesWithKey(const MessageKey& removeKey);
            void clear();
            std::vector<Message> getLastVisibleMessages() const;


	};
}