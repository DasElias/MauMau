#include "NetworkGameFacade.h"
#include <boost/asio.hpp>
#include <shared/packet/stc/EnteringRoomSuccessReport_STCAnswerPacket.h>
#include <shared/utils/Logger.h>

namespace card {
	NetworkGameFacade::NetworkGameFacade(NetworkErrorHandler& errorHandler, std::string username, Avatar avatar) :
			avatar(avatar),
			isWaitingForResponse_field(true),
			errorMsgInPlainText(std::nullopt),
			usernameOfLocalPlayer(username),
			room(nullptr),
			handler_enteringRoomSuccessReport(std::bind(&NetworkGameFacade::listener_enteringRoomSuccessReport, this, std::placeholders::_1)) {

		try {
			this->conn = std::make_shared<ConnectionToServer>();
			this->conn->start();

			this->packetTransmitter = std::make_shared<ClientPacketTransmitter>(conn, errorHandler);
			this->packetTransmitter->addListenerForServerPkt(EnteringRoomSuccessReport_STCAnswerPacket::PACKET_ID, handler_enteringRoomSuccessReport);
		} catch(boost::system::system_error e) {
			setErrorMsgOnConnectionFail(e.code());
			isWaitingForResponse_field = false;
		}
	}
	NetworkGameFacade::~NetworkGameFacade() {
		if(packetTransmitter) {
			// will fail silently if the packet listener has never been registered due to an exception in the constructor
			packetTransmitter->removeListenerForServerPkt(EnteringRoomSuccessReport_STCAnswerPacket::PACKET_ID, handler_enteringRoomSuccessReport);
		}
	}
	bool NetworkGameFacade::hasErrorOccuredOnEstablishingConnection() const {
		return errorMsgInPlainText.has_value();
	}
	bool NetworkGameFacade::isWaitingForResponse() const {
		return isWaitingForResponse_field;
	}
	std::optional<std::string> NetworkGameFacade::getErrorMsgOnEstablishingConnectionInPlainText() {
		return errorMsgInPlainText;
	}
	bool NetworkGameFacade::isGameRunning() const {
		if(! isRoomAvailable()) return false;
		else return room->isGameRunning();
	}
	ProxyMauMauGame& NetworkGameFacade::getGame() {
		if(! isGameRunning()) throw std::runtime_error("There is no active game at the moment.");
		return room->getGame();
	}
	bool NetworkGameFacade::isRoomAvailable() const {
		return room != nullptr;
	}
	ProxyRoom& NetworkGameFacade::getRoom() {
		if(! isRoomAvailable()) throw std::runtime_error("There is no active room at the moment.");
		return *room;
	}
	std::shared_ptr<CTSPacketTransmitter> NetworkGameFacade::getPacketTransmitter() {
		return packetTransmitter;
	}
	void NetworkGameFacade::listener_enteringRoomSuccessReport(Packet& p) {
		if(! isWaitingForResponse_field) throw std::runtime_error("Unexpected EnteringRoomSuccessReport_STCAnswerPacket.");
		this->isWaitingForResponse_field = false;

		auto& casted = dynamic_cast<EnteringRoomSuccessReport_STCAnswerPacket&>(p);
		if(casted.getStatusCode() == EnteringRoomSuccessReport_STCAnswerPacket::SUCCESS_STATUS) {
			room = std::make_unique<ProxyRoom>(packetTransmitter, casted.getUsernamesOfOtherParticipants(), casted.getAvatarsOfOtherParticipants(), casted.areOtherParticipantsAiPlayers(),usernameOfLocalPlayer, avatar, casted.getRoomLeader(), casted.getOptions());
		} else {
			setErrorMsgForSuccessReport(casted.getStatusCode());
		}
	}
	void NetworkGameFacade::setErrorMsgOnConnectionFail(boost::system::error_code ec) {
		switch(ec.value()) {
			case boost::asio::error::host_unreachable:
				errorMsgInPlainText = "Keine Internetverbindung.";
				break;
			case boost::asio::error::connection_refused:
				errorMsgInPlainText = "Der Server ist zurzeit nicht verfügbar. Bitte versuche es später erneut.";
				break;
			case boost::asio::error::timed_out:
				errorMsgInPlainText = "Der Server benötigte zu lange zum Antworten. Bitte versuche es später erneut.";
				break;
			default:
				errorMsgInPlainText = ec.message();
		}
	}
	void NetworkGameFacade::setErrorMsgForSuccessReport(int statusCode) {
		switch(statusCode) {	
			case EnteringRoomSuccessReport_STCAnswerPacket::SUCCESS_STATUS:
				this->errorMsgInPlainText = std::nullopt;
				break;
			case EnteringRoomSuccessReport_STCAnswerPacket::ROOM_NOT_FOUND_STATUS:
				this->errorMsgInPlainText = "Der gewünschte Raum existiert nicht.";
				break;
			case EnteringRoomSuccessReport_STCAnswerPacket::ROOM_FULL_STATUS:
				this->errorMsgInPlainText = "Der gewünschte Raum ist bereits voll.";
				break;
			case EnteringRoomSuccessReport_STCAnswerPacket::USERNAME_TAKEN_STATUS:
				this->errorMsgInPlainText = "Der gewünschte Nutzername ist bereits vergeben.";
				break;
			case EnteringRoomSuccessReport_STCAnswerPacket::UNKNOWN_ERROR_STATUS:
			default:
				this->errorMsgInPlainText = "Ein unbekannter Fehler ist aufgetreten.";
				log(LogSeverity::ERR, "Unknown error in EnteringRoomSuccessReport: " + std::to_string(statusCode));
		}
	}
}