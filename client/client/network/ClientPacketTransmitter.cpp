#include "ClientPacketTransmitter.h"
#include <shared/packet/STCPacketConstructorFromJson.h>
#include <shared/utils/ThreadUtils.h>
#include "NetworkErrorMessageGenerator.h"

namespace card {
	ClientPacketTransmitter::ClientPacketTransmitter(std::shared_ptr<GeneralTCPTransmitterWithKeepalive> conn, NetworkErrorHandler& errorHandler) :
			conn(conn),
			errorHandler(errorHandler) {

		conn->setOnReceiveFunc([this](std::string& jsonMsg) {
			onReceiveHandler(jsonMsg);
		});
		conn->setOnErrorFunc([this](boost::system::error_code ec) {
			if(ec != boost::asio::error::operation_aborted) {
				threadUtils_invokeIn(0, [this, ec]() {
					if(ec == boost::asio::error::eof) {
						onKickHandler();
					} else {
						onErrorHandler(ec);
					}
				});		
			}
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
		std::string errorMsg = NetworkErrorMessageGenerator::getMsg(ec);
		errorHandler.resetAndShowError(errorMsg, "Netzwerkfehler");
	}
	void ClientPacketTransmitter::onKickHandler() {
		errorHandler.onKick();
	}
	void ClientPacketTransmitter::sendPacketToServer(ClientToServerPacket& p) {
		std::string msg = p.getJson();
		conn->send(msg);
	}
}