#include "MessageQueue.h"
#include <shared/utils/TimeUtils.h>
#include <limits>
#include <algorithm>

namespace card {
	MessageKey::MessageKey() {
		static std::uint64_t counter = 0;
		this->value = counter++;
	}
	bool MessageKey::operator==(const MessageKey& other) const {
		return value == other.value;
	}
	MessageQueue::MessageQueue(int defaultMsgDisplayDuration) :
		defaultMsgDisplayDuration(defaultMsgDisplayDuration) {
	}
	void MessageQueue::appendMessage(std::string message) {
		appendMessage(message, defaultMsgDisplayDuration);

	}
	void MessageQueue::appendMessage(std::string message, int displayDuration) {
		static MessageKey defaultKey = {};

		long long removeTime = getMilliseconds() + displayDuration;
		Message msgObj = {message, removeTime, defaultKey};
		if(messages.size() >= MAX_MSG_AMOUNT) {
			messages.pop_front();
		}
		messages.push_back(msgObj);
	}
	void MessageQueue::appendMessagePermanently(std::string message, const MessageKey& removeKey) {
		Message msgObj = {message, LLONG_MAX, removeKey};
		if(messages.size() >= MAX_MSG_AMOUNT) {
			messages.pop_front();
		}
		messages.push_back(msgObj);
	}
	void MessageQueue::removeMessagesWithKey(const MessageKey& removeKey) {
		messages.erase(std::remove_if(messages.begin(), messages.end(), [removeKey](const Message& val) {
			auto removeKeyOfMsg = val.removeKey;
			return removeKey == removeKeyOfMsg;
		}), messages.end());
	}
	void MessageQueue::clear() {
		messages.clear();
	}
	std::vector<Message> MessageQueue::getLastVisibleMessages() const {
		std::vector<Message> lastVisibleMessages;
		for(auto& msg : messages) {
			if(msg.removeUnixTimeMs > getMilliseconds()) {
				lastVisibleMessages.push_back(msg);
			}
		}
		return lastVisibleMessages;
	}
}