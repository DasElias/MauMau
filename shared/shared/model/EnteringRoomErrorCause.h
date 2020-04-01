#pragma once

namespace card {
	typedef int EnteringRoomErrorCause_t;

	namespace EnteringRoomErrorCause {
		EnteringRoomErrorCause_t const SUCCESS_STATUS = 0;
		EnteringRoomErrorCause_t const UNKNOWN_ERROR_STATUS = 1;
		EnteringRoomErrorCause_t const ROOM_NOT_FOUND_STATUS = 2;
		EnteringRoomErrorCause_t const USERNAME_TAKEN_STATUS = 3;
		EnteringRoomErrorCause_t const ROOM_FULL_STATUS = 4;
		EnteringRoomErrorCause_t const ALREADY_IN_ROOM_STATUS = 5;
	}
}