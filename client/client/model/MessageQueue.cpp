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
	MessageQueue::MessageQueue(int msgDisplayDuration) :
			msgDisplayDuration(msgDisplayDuration) {
	}
	void MessageQueue::appendMessage(std::string message) {
		static MessageKey defaultKey = {};

		Message msgObj = {message, getMilliseconds(), defaultKey};
		if(messages.size() >= MAX_MSG_AMOUNT) {
			messages.pop_front();
		}
		messages.push_back(msgObj);
	}
	void MessageQueue::appendMessagePermanently(std::string message, const MessageKey& removeKey) {
		// we can't use LLONG_MAX since otherwise we would get an integer overflow on checking if the message should be visible
		long long infiniteAdditionTime = LLONG_MAX - msgDisplayDuration;
		Message msgObj = {message, infiniteAdditionTime, removeKey};
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
			if(msg.appendUnixTimeMs + msgDisplayDuration > getMilliseconds()) {
				lastVisibleMessages.push_back(msg);
			}
		}
		return lastVisibleMessages;
	}
}