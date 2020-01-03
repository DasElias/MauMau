#include "ClientPacketTransmitter.h"
#include "ClientPacketTransmitter.h"
#include "ClientPacketTransmitter.h"
#include <shared/packet/STCPacketConstructorFromJson.h>
#include <shared/utils/ThreadUtils.h>

namespace card {
	ClientPacketTransmitter::ClientPacketTransmitter(std::shared_ptr<GeneralTCPTransmitter> conn, NetworkErrorHandler& errorHandler) :
			conn(conn),
			errorHandler(errorHandler) {

		conn->setOnReceiveFunc([this](std::string& jsonMsg) {
			onReceiveHandler(jsonMsg);
		});
		conn->setOnErrorFunc([this](boost::system::error_code ec) {
			onErrorHandler(ec);
		});
	}
	void ClientPacketTransmitter::onReceiveHandler(std::string& jsonMsg) {
		threadUtils_invokeIn(0, [this, jsonMsg]() {
			std::string copiedJsonMsg = jsonMsg;
			constructAndProcesSTCPacketFromJson(copiedJsonMsg, [this](Packet& p) {
				onPackageReceiveFromServer(p);
			});
		});
	}
	void ClientPacketTransmitter::onErrorHandler(boost::system::error_code ec) {
		errorHandler.resetAndShowError(ec.message(), "Netzwerkfehler");
	}
	void ClientPacketTransmitter::sendPacketToServer(ClientToServerPacket& p) {
		std::string msg = p.getJson();
		conn->send(msg);
	}
}