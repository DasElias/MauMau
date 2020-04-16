#pragma once
#include "BasicRoomCreationElement.h"
#include "LabeledInputField.h"
#include "AvatarChooser.h"
#include "EditOptionsElement.h"

namespace card {
	class CreateOnlineRoomElement : public BasicRoomCreationElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<egui::UnorganizedParentElement> lockElement;
			std::shared_ptr<AvatarChooser> avatarChooser;
			std::shared_ptr<EditOptionsElement> optionsElement;
			std::shared_ptr<LabeledInputField> usernameInputField;

			RoomOptions options;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreateOnlineRoomElement(AvatarTextures& avatarTextures);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getUsernameInput() const;
			Avatar getSelectedAvatar() const;
			RoomOptions getOptions() const;

			void setUsernameInput(std::string username);
			void setSelectedAvatar(Avatar a);

			void lockInput(bool shouldLock);
	};
}