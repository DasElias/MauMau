#pragma once
#include "BasicRoomJoinElement.h"
#include "LabeledInputField.h"
#include "AvatarChooser.h"
#include <shared/model/RoomOptions.h>

namespace card {
	class JoinOnlineRoomElement : public BasicRoomJoinElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<egui::UnorganizedParentElement> lockElement;
			std::shared_ptr<AvatarChooser> avatarChooser;
			std::shared_ptr<LabeledInputField> usernameInputField;
			std::shared_ptr<LabeledInputField> roomCodeInputField;

			RoomOptions options;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			JoinOnlineRoomElement(AvatarTextures& avatarTextures, std::size_t roomCodeMaxLength);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getUsernameInput() const;
			std::string getRoomCodeInput() const;
			Avatar getSelectedAvatar() const;

			void lockInput(bool shouldLock);
	};
}