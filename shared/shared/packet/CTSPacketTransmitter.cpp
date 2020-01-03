#include "CTSPacketTransmitter.h"
#include <stdexcept>
#include "stc/OperationSuccessful_STCAnswerPacket.h"

namespace card {
	CTSPacketTransmitter::CTSPacketTransmitter() {
		this->addListenerForServerPkt(OperationSuccessful_STCAnswerPacket::PACKET_ID, {[this](Packet& p) {
			auto& casted = dynamic_cast<OperationSuccessful_STCAnswerPacket&>(p);
			if(casted.wasSuccessful()) return;

			throw std::runtime_error("Assertion failed!");
		}});
	}

	void CTSPacketTransmitter::addListenerForServerPkt(int packetId, ServerPacketListenerCallback callback) {
		if(permanentListeners.find(packetId) == permanentListeners.end()) {
			// no listener for packetId has been registered yet

			std::vector<ServerPacketListenerCallback> v;
			v.push_back(callback);
			permanentListeners.insert(std::make_pair(packetId, v));
		} else {
			permanentListeners.at(packetId).push_back(callback);
		}
	}

	void CTSPacketTransmitter::removeListenerForServerPkt(int packetId, ServerPacketListenerCallback callback) {
		if(permanentListeners.find(packetId) == permanentListeners.end()) return;

		std::vector<ServerPacketListenerCallback>& list = permanentListeners.at(packetId);
		list.erase(std::find(list.begin(), list.end(), callback));

		if(list.empty()) permanentListeners.erase(packetId);
	}

	void CTSPacketTransmitter::onPackageReceiveFromServer(Packet& p) {
		int packetType = p.getPacketType();

		if(permanentListeners.find(packetType) != permanentListeners.end()) {
			for(auto& handler : permanentListeners.at(packetType)) {
				handler(p);
			}
		}
	}
}