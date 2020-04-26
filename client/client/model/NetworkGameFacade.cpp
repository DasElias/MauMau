#include "NetworkGameFacade.h"
#include <boost/asio.hpp>
#include <shared/packet/stc/EnteringRoomSuccessReport_STCAnswerPacket.h>
#include <shared/model/EnteringRoomErrorCause.h>
#include <shared/utils/Logger.h>
#include "../network/NetworkErrorMessageGenerator.h"

namespace card {
	NetworkGameFacade::NetworkGameFacade(NetworkErrorHandler& errorHandler, std::unique_ptr<AbstractRoomLeaveHandler> roomLeaveHandler, std::unique_ptr<AbstractReturnBackToMenuHandler> gameEndHandler, std::string username, Avatar avatar) :
			avatar(avatar),
			isWaitingForResponse_field(true),
			errorMsgInPlainText(std::nullopt),
			usernameOfLocalPlayer(username),
			roomLeaveHandler(std::move(roomLeaveHandler)),
			gameEndHandler(std::move(gameEndHandler)),
			room(nullptr),
			handler_enteringRoomSuccessReport(std::bind(&NetworkGameFacade::listener_enteringRoomSuccessReport, this, std::placeholders::_1)) {

		this->conn = std::make_shared<ConnectionToServer>();
		this->conn->start([this](boost::system::system_error& e) {
			setErrorMsgOnConnectionFail(e.code());
			isWaitingForResponse_field = false;
		});

		this->packetTransmitter = std::make_shared<ClientPacketTransmitter>(conn, errorHandler);
		this->packetTransmitter->addListenerForServerPkt(EnteringRoomSuccessReport_STCAnswerPacket::PACKET_ID, handler_enteringRoomSuccessReport);
	}
	NetworkGameFacade::~NetworkGameFacade() {
		if(conn) conn->close();

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
		if(casted.getStatusCode() == EnteringRoomErrorCause::SUCCESS_STATUS) {
			room = std::make_unique<ProxyRoom>(packetTransmitter, *roomLeaveHandler, *gameEndHandler, casted.getRoomCode(), casted.getUsernamesOfOtherParticipants(), casted.getAvatarsOfOtherParticipants(), casted.areOtherParticipantsAiPlayers(),usernameOfLocalPlayer, avatar, casted.getRoomLeader(), casted.getOptions());
		} else {
			setErrorMsgForSuccessReport(casted.getStatusCode());
		}
	}
	void NetworkGameFacade::setErrorMsgOnConnectionFail(boost::system::error_code ec) {
		std::string errorMsg = NetworkErrorMessageGenerator::getMsg(ec);
		this->errorMsgInPlainText = errorMsg;
	}
	void NetworkGameFacade::setErrorMsgForSuccessReport(int statusCode) {
		switch(statusCode) {	
			case EnteringRoomErrorCause::SUCCESS_STATUS:
				this->errorMsgInPlainText = std::nullopt;
				break;
			case EnteringRoomErrorCause::ROOM_NOT_FOUND_STATUS:
				this->errorMsgInPlainText = "Der gewünschte Raum existiert nicht.";
				break;
			case EnteringRoomErrorCause::ROOM_FULL_STATUS:
				this->errorMsgInPlainText = "Der gewünschte Raum ist bereits voll.";
				break;
			case EnteringRoomErrorCause::USERNAME_TAKEN_STATUS:
				this->errorMsgInPlainText = "Der gewünschte Nutzername ist bereits vergeben.";
				break;
			case EnteringRoomErrorCause::ALREADY_IN_ROOM_STATUS:
				this->errorMsgInPlainText = "Du bist bereits in einem Raum.";
				break;
			case EnteringRoomErrorCause::PROTOCOL_VERSION_MISMATCH_STATUS:
				this->errorMsgInPlainText = "Deine Version von MauMau ist leider nicht kompatibel mit dem Server. Bitte update deine App auf die neueste Version.";
				break;
			case EnteringRoomErrorCause::UNKNOWN_ERROR_STATUS:
			default:
				this->errorMsgInPlainText = "Ein unbekannter Fehler ist aufgetreten.";
				log(LogSeverity::ERR, "Unknown error in EnteringRoomSuccessReport: " + std::to_string(statusCode));
		}
	}
}