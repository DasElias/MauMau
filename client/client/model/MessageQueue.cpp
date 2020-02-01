#include "MessageQueue.h"
#include <shared/utils/TimeUtils.h>

namespace card {
	MessageQueue::MessageQueue(int msgDisplayDuration) :
			msgDisplayDuration(msgDisplayDuration) {
	}
	void MessageQueue::appendMessage(std::string message) {
		Message msgObj = {message, getMilliseconds()};
		if(messages.size() >= MAX_MSG_AMOUNT) {
			messages.pop_front();
		}
		messages.push_back(msgObj);
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